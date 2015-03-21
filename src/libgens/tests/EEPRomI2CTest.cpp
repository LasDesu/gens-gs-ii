/***************************************************************************
 * libgens/tests: Gens Emulation Library. (Test Suite)                     *
 * EEPRomI2CTest.cpp: EEPRomI2C test.                                      *
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

// Google Test
#include "gtest/gtest.h"

// LibGens.
#include "lg_main.hpp"

// EEPRomI2C class.
#include "Save/EEPRomI2C.hpp"

// ARRAY_SIZE(x)
#include "macros/common.h"

// Test EEPROM data.
#include "EEPRomI2CTest_data.h"

// C includes. (C++ namespace)
#include <cstdio>

namespace LibGens { namespace Tests {

class EEPRomI2CTest : public ::testing::Test
{
	protected:
		EEPRomI2CTest()
			: m_eeprom(nullptr) { }
		virtual ~EEPRomI2CTest() { }

		virtual void SetUp(void) override;
		virtual void TearDown(void) override;

	protected:
		EEPRomI2C *m_eeprom;

		/**
		 * Ensure we're in a STOP condition.
		 */
		void doStop(void);

		/**
		 * Send an 8-bit data word to the EEPROM.
		 * @param data 8-bit data word.
		 * @return 0 if ACK; 1 if NACK.
		 */
		uint8_t sendData(uint8_t data);
};

/**
 * Set up the EEPROM for testing.
 * TODO: Read EEPROM parameters and set it up here?
 */
void EEPRomI2CTest::SetUp(void)
{
	// Initialize the EEPROM.
	m_eeprom = new EEPRomI2C();
}

/**
 * Tear down the EEPROM.
 */
void EEPRomI2CTest::TearDown(void)
{
	delete m_eeprom;
	m_eeprom = nullptr;
}

/**
 * Ensure we're in a STOP condition.
 */
void EEPRomI2CTest::doStop(void)
{
	m_eeprom->dbg_setSCL(0);
	m_eeprom->dbg_setSDA(0);
	m_eeprom->dbg_setSCL(1);
	m_eeprom->dbg_setSDA(1);
}

/**
 * Send an 8-bit data word to the EEPROM.
 * @param data 8-bit data word.
 * @return 0 if ACK; 1 if NACK.
 */
uint8_t EEPRomI2CTest::sendData(uint8_t data)
{
	for (int i = 8; i > 0; i--, data <<= 1) {
		// Data is written when SCL=0.
		m_eeprom->dbg_setSCL(0);
		m_eeprom->dbg_setSDA(!!(data & 0x80));
		m_eeprom->dbg_setSCL(1);
	}

	// Data has been written.
	// ACK or NACK is received on the 9th clock cycle.
	m_eeprom->dbg_setSCL(0);
	m_eeprom->dbg_setSDA(1);	// Release SDA.
	m_eeprom->dbg_setSCL(1);

	// Get the ACK or NACK.
	m_eeprom->dbg_getSDA(&data);
	return data;
}

/**
 * I2C notes:
 * - START condition: SCL high, SDA high-to-low
 * - STOP condition: SCL high, SDA low-to-high
 *
 * - Other than these two, SDA should only be modified
 *   by the master when SCL is low.
 */

/**
 * X24C01: Test sequential reading of an empty EEPROM.
 */
TEST_F(EEPRomI2CTest, X24C01_seqReadEmpty)
{
	// Set the EEPROM as X24C01.
	// TODO: Move to SetUp().
	m_eeprom->dbg_setEEPRomType(EEPRomI2C::EPR_X24C01);
	unsigned int eepromSize;
	m_eeprom->dbg_getEEPRomSize(&eepromSize);
	EXPECT_EQ(128U, eepromSize) << "X24C01 should be 128 bytes.";

	// Make sure we're in a STOP condition at first.
	doStop();

	// EEPROM response.
	uint8_t response;

	// START an I2C transfer.
	// We'll request a READ from address 0.
	// Mode 1 word address: [A6 A5 A4 A3 A2 A1 A0 RW]
	m_eeprom->dbg_setSDA(0);	// START
	response = sendData(0x01);	// Addr=0, RW=1

	// Check for ACK.
	m_eeprom->dbg_getSDA(&response);
	EXPECT_EQ(0, response) << "NACK received; expected ACK.";
}

} }

int main(int argc, char *argv[])
{
	fprintf(stderr, "LibGens test suite: EEPRomI2C tests.\n\n");

	::testing::InitGoogleTest(&argc, argv);
	LibGens::Init();
	return RUN_ALL_TESTS();
}