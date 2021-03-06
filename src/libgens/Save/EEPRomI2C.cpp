/***************************************************************************
 * libgens: Gens Emulation Library.                                        *
 * EEPRomI2C.cpp: I2C Serial EEPROM handler.                               *
 *                                                                         *
 * Copyright (c) 2015 by David Korth.                                      *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.           *
 ***************************************************************************/

/**
 * Based on cart_hw/eeprom.c from Genesis Plus GX.
 */

#include "EEPRomI2C.hpp"

#include "macros/common.h"
#include "macros/log_msg.h"

// C includes (C++ namespace).
#include <cassert>
#include <cstring>

#include "EEPRomI2C_p.hpp"
namespace LibGens {

/** EEPRomI2CPrivate **/

EEPRomI2CPrivate::EEPRomI2CPrivate(EEPRomI2C *q)
	: q(q)
	, dirty(false)
	, framesElapsed(0)
{
	// Clear the EEPRom chip specification.
	memset(&eprChip, 0, sizeof(eprChip));

	// Reset the EEPRom.
	reset();
}

/**
 * Clear the EEPRom and initialize settings.
 * This function does NOT reset the EEPRom type!
 */
void EEPRomI2CPrivate::reset(void)
{
	// EEProm is initialized with 0xFF.
	memset(eeprom, 0xFF, sizeof(eeprom));
	memset(page_cache, 0xFF, sizeof(page_cache));
	clearDirty();

	// Reset the clock and data line states.
	scl = 1;
	sda_out = 1;
	sda_in = 1;
	scl_prev = 1;
	sda_out_prev = 1;
	sda_in_prev = 1;

	// Reset the internal registers.
	address = 0;
	rw = 0;
	dev_addr = 0;

	// Reset the state.
	doStandby();
}

/**
 * Go into standby mode.
 */
void EEPRomI2CPrivate::doStandby(void)
{
	sda_out = 1;	// release /SDA
	counter = 0;	// reset the counter
	data_buf = 0;	// clear the data buffer
	shift_rw = 0;	// shift in
	state = EPR_STANDBY;
}

/**
 * Start an EPR_WRITE_DATA operation.
 * Sets internal variables and initializes the page cache.
 */
void EEPRomI2CPrivate::startWriteData(void)
{
	// Partial page writes do NOT erase unwritten data.
	// To implement that, we read the entire page into
	// the page cache before doing any writes.

	// This also works for BYTE WRITE, but may be slightly
	// slower because we're always copying the entire page.
	// Caching the last page address is probably not helpful,
	// since it's unlikely that the host device will be
	// rewriting the same page multiple times in a row.

	// Initialize the page cache.
	const unsigned int pgSize = eprChip.pg_mask + 1;
	const unsigned int pgAddress = address & ~eprChip.pg_mask;
	memcpy(page_cache, &eeprom[pgAddress], pgSize);

	// Set the internal variables for EPR_WRITE_DATA.
	counter = 0;
	data_buf = 0;
	shift_rw = 0;	// Shifting in.
	state = EPR_WRITE_DATA;
}

/**
 * Write the page cache to the EEPROM.
 * This is done when a STOP condition is received
 * after a byte is written during EPR_WRITE_DATA.
 */
void EEPRomI2CPrivate::stopWriteData(void)
{
	const unsigned int pgAddress = address & ~eprChip.pg_mask;
	const unsigned int pgSize = eprChip.pg_mask + 1;

	for (unsigned int i = 0; i < pgSize; i++) {
		const unsigned int byte_address = pgAddress | i;
		if (page_cache[i] != eeprom[byte_address]) {
			// Byte has changed.
			eeprom[byte_address] = page_cache[i];
			setDirty();
		}
	}
}

/**
 * Process a shifted-in data word.
 */
void EEPRomI2CPrivate::processI2CShiftIn(void)
{
	// Determine what to do based on the current state.
	switch (state) {
		case EPR_MODE1_WORD_ADDRESS:
			// Mode 1: Word address. (X24C01)
			// Format: [A6 A5 A4 A3 A2 A1 A0 RW]
			address = (data_buf >> 1) & 0x7F;
			rw = (data_buf & 1);
			counter = 0;
			if (rw) {
				// Read data.
				data_buf = eeprom[address];
				shift_rw = 1;	// Shifting out.
				state = EPR_READ_DATA;
			} else {
				// Write data.
				startWriteData();
			}
			LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
				"EPR_MODE1_WORD_ADDRESS: address=%02X, rw=%d, data_buf=%02X",
				address, rw, data_buf);
			break;

		case EPR_WRITE_DATA: {
			// Write the data byte to the page cache.
			page_cache[address & eprChip.pg_mask] = data_buf;

			// Next byte in the page.
			const uint16_t prev_address = address;
			uint16_t addr_low_tmp = address;
			addr_low_tmp++;
			addr_low_tmp &= eprChip.pg_mask;
			address = ((address & ~eprChip.pg_mask) | addr_low_tmp);

			data_buf = 0;
			LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
				"EPR_WRITE_DATA: %02X -> [%02X]; address=%02X",
				eeprom[prev_address], prev_address, address);
			break;
		}

		case EPR_MODE2_DEVICE_ADDRESS: {
			// Modes 2, 3: Device address.
			// Format: [ 1  0  1  0 A2 A1 A0 RW]
			// A2-A0 may be device select for smaller chips,
			// or address bits A11-A8 for larger chips.
			const uint8_t dev_type = (data_buf >> 4) & 0xF;
			if (dev_type != 0xA) {
				// Not an EEPROM command.
				// Go back to standby.
				LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
					"EPR_MODE2_DEVICE_ADDRESS: %02X: dev_type=%1X, ignoring",
					data_buf, dev_type);
				doStandby();
				break;
			}

			// Verify the device address.
			dev_addr = ((data_buf >> 1) & 0x07);
			if (eprChip.epr_mode == EEPRomI2C::EPR_MODE2) {
				// Mode 2. Part of the device address may be
				// high bits of the word address.
				dev_addr &= ~(eprChip.sz_mask >> 8);
			}

			// TODO: Mask eprChip.dev_addr?
			if (dev_addr != eprChip.dev_addr) {
				// Incorrect device address.
				// Ignore this request.
				// TODO: Split into doStandby() function?
				LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
					"EPR_MODE2_DEVICE_ADDRESS: dev_type=%1X, dev_addr=%1X; my dev_addr=%1X, ignoring",
					dev_type, dev_addr, eprChip.dev_addr);
				doStandby();
				break;
			}

			rw = (data_buf & 1);
			counter = 0;

			// Mode 2. Part of the device address may be
			// high bits of the word address.
			if (eprChip.epr_mode == EEPRomI2C::EPR_MODE2) {
				uint16_t m2_hiaddr = (data_buf << 7) & 0x700;
				address = m2_hiaddr | (address & 0xFF);
				address &= eprChip.sz_mask;
			}

			LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
				"EPR_MODE2_DEVICE_ADDRESS: %02X: type=%1X, dev=%02X, address=%04X, rw=%d",
				data_buf, dev_type, dev_addr, address, rw);

			if (rw) {
				// Current address read.
				data_buf = eeprom[address];
				shift_rw = 1;	// Shifting out.
				state = EPR_READ_DATA;
			} else {
				// Write data.
				// Needs another command word to set
				// Word Address bits A7-A0.
				data_buf = 0;
				shift_rw = 0;	// Shifting in.
				state = (eprChip.epr_mode == EEPRomI2C::EPR_MODE2
					? EPR_MODE2_WORD_ADDRESS_LOW
					: EPR_MODE3_WORD_ADDRESS_HIGH);
			}
			break;
		}

		case EPR_MODE2_WORD_ADDRESS_LOW:
			// Modes 2, 3: Word address, low byte.
			// Format: [A7 A6 A5 A4 A3 A2 A1 A0]
			address = (address & ~0xFF) | data_buf;
			address &= eprChip.sz_mask;
			LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
				"EPR_MODE2_WORD_ADDRESS_LOW: %02X: dev_addr=%02X, address=%04X",
				data_buf, dev_addr, address);

			// Write data.
			startWriteData();
			break;

		case EPR_MODE3_WORD_ADDRESS_HIGH:
			// Modes 2, 3: Word address, high byte.
			// Format: [A15 A14 A13 A12 A11 A10 A9 A8]
			address = (address & ~0xFF00) | (data_buf << 8);
			address &= eprChip.sz_mask;
			LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
				"EPR_MODE3_WORD_ADDRESS_HIGH: %02X: dev_addr=%02X, address=%04X",
				data_buf, dev_addr, address);

			// Get the low byte of the word address.
			counter = 0;
			data_buf = 0;
			shift_rw = 0;	// Shifting in.
			state = EPR_MODE2_WORD_ADDRESS_LOW;
			break;

		default:
			// Unknown.
			// Go back to standby.
			doStandby();
			break;
	}
}

/**
 * Determine what to do after shifting out a data word.
 */
void EEPRomI2CPrivate::processI2CShiftOut(void)
{
	// Determine what to do based on the current state.
	switch (state) {
		case EPR_READ_DATA:
			// Go to the next byte.
			// NOTE: Page mask does NOT apply to reads.
			address++;
			address &= eprChip.sz_mask;
			data_buf = eeprom[address];
			counter = 0;
			LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
				"EPR_READ_DATA: ACK received: address=%04X, data_buf=%02X",
				address, data_buf);
			break;

		default:
			// Unknown.
			// Go back to standby.
			doStandby();
			break;
	}
}

/**
 * Process an I2C bit.
 */
void EEPRomI2CPrivate::processI2Cbit(void)
{
	// Save the current /SDA out.
	sda_out_prev = sda_out;

	if (eprChip.epr_mode == EEPRomI2C::EPR_NONE) {
		// No EEPRom.
		goto done;
	}

	// Check for a STOP condition.
	if (checkStop()) {
		// STOP condition reached.
		if (state == EPR_WRITE_DATA) {
			// Complete the EPR_WRITE_DATA operation.
			stopWriteData();
		}

		LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
			"Received STOP condition.");
		doStandby();
		goto done;
	}

	// Has a START condition been issued?
	if (checkStart()) {
		// START condition.
		LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
			"Received START condition.");
		counter = 0;
		sda_out = 1;
		data_buf = 0;
		shift_rw = 0;	// shift in
		if (eprChip.epr_mode == EEPRomI2C::EPR_MODE1) {
			// Mode 1. (X24C01)
			state = EPR_MODE1_WORD_ADDRESS;
		} else {
			// Mode 2 or 3.
			state = EPR_MODE2_DEVICE_ADDRESS;
		}
		goto done;
	}

	if (state == EPR_STANDBY) {
		// Nothing to do right now.
		goto done;
	}

	// Check if we're shifting in or shifting out.
	if (!shift_rw) {
		// Shifting in.
		// Master device is writing an 8-bit data word.

		// Check for SCL low-to-high.
		if (checkSCL_LtoH()) {
			if (counter >= 8) {
				// Finished receiving the data word.
				counter++;

				// Process the data word.
				processI2CShiftIn();
			} else {
				// Data bit is valid.
				data_buf <<= 1;
				data_buf |= getSDA();
				counter++;
			}
		} else if (checkSCL_HtoL()) {
			// Release the data line.
			sda_out = 1;
			if (counter == 8) {
				// Finished receiving the data word.
				// Acknowledge it.
				// NOTE: NBA Jam expects the ACK to be
				// done here, since it checks the /SDA
				// line while /SCL=0.
				sda_out = 0;
			} else if (counter >= 9) {
				// Acknowledged.
				// Reset the counter.
				counter = 0;
			}
		}
	} else {
		// Shifting out.
		// Master device is reading an 8-bit data word.
		// NOTE: This is usually EPR_READ_DATA.

		// Check for SCL high-to-low.
		if (checkSCL_LtoH()) {
			if (counter >= 9) {
				// Is this an acknowlege?
				if (!getSDA()) {
					// Acknowledged by master.
					// Determine what should be shifted out next.
					processI2CShiftOut();
				}
			}
		} else if (checkSCL_HtoL()) {
			if (counter < 8) {
				// Send a data bit to the host.
				// NOTE: MSB is out first.
				sda_out = !!(data_buf & 0x80);
				data_buf <<= 1;
				counter++;
			} else if (counter == 8) {
				// Release the data line.
				sda_out = 1;
				counter++;
			}

			// TODO: Not necessarily EPR_READ_DATA.
			if (counter == 8) {
				LOG_MSG(eeprom_i2c, LOG_MSG_LEVEL_DEBUG2,
					"EPR_READ_DATA: all 8 bits read: address=%04X",
					address);
			}
		}
	}

done:
	// Save the current /SCL and /SDA.
	scl_prev = scl;
	sda_in_prev = sda_in;
}

/** EEPRom **/

/**
 * Initialize the EEPRom chip.
 */
EEPRomI2C::EEPRomI2C()
	: d(new EEPRomI2CPrivate(this))
{
	// Clear the EEPRom mapper.
	memset(&eprMapper, 0, sizeof(eprMapper));
}

EEPRomI2C::~EEPRomI2C()
{
	delete d;
}

/**
 * Clear the EEPRom and initialize settings.
 * This function does NOT reset the EEPRom type!
 */
void EEPRomI2C::reset(void)
{
	d->reset();
}

/**
 * Set the EEPRom type.
 * @param type EEPRom type. (Specify a negative number to clear)
 * @return 0 on success; non-zero on error.
 */
int EEPRomI2C::setEEPRomType(int type)
{
	if (type < 0) {
		// Negative type ID. Reset the EEPRom.
		d->reset();
		return 0;
	} else if (type >= ARRAY_SIZE(d->rom_db)) {
		// Type ID is out of range.
		return 1;
	}

	// Set the EEPRom type.
	memcpy(&eprMapper, &d->rom_db[type].mapper, sizeof(eprMapper));
	memcpy(&d->eprChip, &d->rom_db[type].epr_chip, sizeof(d->eprChip));

	// Make sure the chip is valid.
	assert(d->eprChip.sz_mask > 0 && d->eprChip.sz_mask+1 <= sizeof(d->eeprom));
	assert(d->eprChip.pg_mask > 0 && d->eprChip.pg_mask+1 <= sizeof(d->page_cache));
	return 0;
}

/**
 * Determine if the EEPRom type is set.
 * @return True if the EEPRom type is set; false if not.
 */
bool EEPRomI2C::isEEPRomTypeSet(void) const
{
	return (d->eprChip.sz_mask != 0);
}

/**
 * Check if the EEPRom is dirty.
 * @return True if EEPRom has been modified since the last save; false otherwise.
 */
bool EEPRomI2C::isDirty(void) const
	{ return d->isDirty(); }

/**
 * Read the specified port. (byte-wide)
 * @param address Address.
 * @return Port value.
 */
uint8_t EEPRomI2C::readByte(uint32_t address)
{
	if (address != eprMapper.sda_out_adr) {
		// Wrong address.
		return 0xFF;
	}

	// Return /SDA, shifted over to the appropriate position.
	// TODO: Other bits should be prefetch?
	return (d->getSDA() << eprMapper.sda_out_bit);
}


/**
 * Read the specified port. (word-wide)
 * @param address Address.
 * @return Port value.
 */
uint16_t EEPRomI2C::readWord(uint32_t address)
{
	// TODO: address probably doesn't need to be masked,
	// since M68K is word-aligned...
	if ((address & ~1) != (eprMapper.sda_out_adr & ~1)) {
		// Wrong address.
		return 0xFFFF;
	}

	// Return /SDA, shifted over to the appropriate position.
	// TODO: Other bits should be prefetch?
	uint8_t sda_out_bit = eprMapper.sda_out_bit;
	sda_out_bit += (!(eprMapper.sda_out_adr & 1) * 8);
	return (d->getSDA() << sda_out_bit);
}

/**
 * Write to the specified port. (byte-wide)
 * @param address Address.
 * @param data Data.
 */
void EEPRomI2C::writeByte(uint32_t address, uint8_t data)
{
	if (address != eprMapper.scl_adr &&
	    address != eprMapper.sda_in_adr)
	{
		// Invalid address.
		return;
	}

	// Check if this is the clock line. (/SCL)
	if (address == eprMapper.scl_adr) {
		d->scl = (data >> (eprMapper.scl_bit)) & 1;
	} else {
		d->scl = d->scl_prev;
	}

	// Check if this is the data line. (/SDA)
	if (address == eprMapper.sda_in_adr) {
		d->sda_in = (data >> (eprMapper.sda_in_bit)) & 1;
	} else {
		d->sda_in = d->sda_in_prev;
	}

	// Process the I2C command.
	// TODO: Only if /SDA or /SCL has changed?
	d->processI2Cbit();
}

/**
 * Write to the specified port. (word-wide)
 * @param address Address.
 * @param data Data.
 */
void EEPRomI2C::writeWord(uint32_t address, uint16_t data)
{
	// TODO: Verify that the address is valid.

	// Mask off the address LSB.
	address &= ~1;

	// Check if this is the clock line. (/SCL)
	if (address == eprMapper.scl_adr) {
		d->scl = (data >> (eprMapper.scl_bit + 8)) & 1;
	} else if ((address | 1) == eprMapper.scl_adr) {
		d->scl = (data >> (eprMapper.scl_bit)) & 1;
	} else {
		d->scl = d->scl_prev;
	}

	// Check if this is the data line. (/SDA)
	if (address == eprMapper.sda_in_adr) {
		d->sda_in = (data >> (eprMapper.sda_in_bit + 8)) & 1;
	} else if ((address | 1) == eprMapper.sda_in_adr) {
		d->sda_in = (data >> (eprMapper.sda_in_bit)) & 1;
	} else {
		d->sda_in = d->sda_in_prev;
	}

	// Process the I2C bit.
	// TODO: Only if /SDA or /SCL has changed?
	d->processI2Cbit();
}

}
