/***************************************************************************
 * libzomg: Zipped Original Memory from Genesis.                           *
 * zomg_z80.h: ZOMG save definitions for the Zilog Z80 emulator.           *
 *                                                                         *
 * Copyright (c) 2008-2015 by David Korth                                  *
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

#ifndef __LIBGENS_SAVE_ZOMG_ZOMG_Z80_H__
#define __LIBGENS_SAVE_ZOMG_ZOMG_Z80_H__

#include "zomg_common.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Z80 memory save struct.
 * ZOMG file: common/Z80_mem.bin
 */
#pragma pack(1)
typedef struct PACKED _Zomg_Z80MemSave_t {
	union {
		uint8_t mem_sg1000[2048];	// 8-bit: Z80 memory. (SG-1000)
		uint8_t mem_cv[1024];		// 8-bit: Z80 memory. (ColecoVision);
		uint8_t mem_mk3[8192];		// 8-bit: Z80 memory. (Sega Mark III (SMS) / Mega Drive)
	};
} Zomg_Z80MemSave_t;

/**
 * Z80 CPU status bits.
 */
enum Zomg_Z80CPUStatus_t {
	// CPU is halted.
	// Will resume on the next interrupt.
	ZOMG_Z80_STATUS_HALTED  = (1 << 0),

	// CPU has locked up.
	// This usually occurs on Mega Drive if the Z80 accesses
	// invalid area in the 68000 banking area.
	ZOMG_Z80_STATUS_FAULTED = (1 << 1),

	// Interrupt is pending.
	ZOMG_Z80_STATUS_INT_PENDING = (1 << 2),

	// NMI is pending.
	ZOMG_Z80_STATUS_NMI_PENDING = (1 << 3),
};

/**
 * Z80 register save struct.
 * ZOMG file: common/Z80_reg.bin
 * NOTE: Byteswapping is done in Zomg.cpp when saving/loading.
 */
#pragma pack(1)
typedef struct PACKED _Zomg_Z80RegSave_t {
	// Main register set.
	uint16_t AF;	// 16LE: AF register.
	uint16_t BC;	// 16LE: BC register.
	uint16_t DE;	// 16LE: DE register.
	uint16_t HL;	// 16LE: HL register.
	uint16_t IX;	// 16LE: IX register.
	uint16_t IY;	// 16LE: IY register.
	uint16_t PC;	// 16LE: PC register.
	uint16_t SP;	// 16LE: SP register.
	
	// Shadow register set.
	uint16_t AF2;	// 16LE: AF' register.
	uint16_t BC2;	// 16LE: BC' register.
	uint16_t DE2;	// 16LE: DE' register.
	uint16_t HL2;	// 16LE: HL' register.
	
	// Other registers.
	uint8_t IFF;	// 8-bit: IFF flipflops. [0 0 0 0 0 0 IFF2 IFF1]
	uint8_t R;	// 8-bit: R register.
	uint8_t I;	// 8-bit: I register.
	uint8_t IM;	// 8-bit: IM register. (0, 1, 2)

	// NOTE: If the following fields are not present in the savestate,
	// they should be assumed to be 0.

	// Additional internal state.
	uint16_t WZ;		// 16LE: Internal address register.
	uint8_t Status;		// 8-bit: CPU status. (bitfield)
	uint8_t IntVect;	// 8-bit: Pending interrupt vector. (IM0, IM2)
} Zomg_Z80RegSave_t;
#pragma pack()

#ifdef __cplusplus
}
#endif

#endif /* __LIBGENS_SAVE_ZOMG_ZOMG_Z80_H__ */
