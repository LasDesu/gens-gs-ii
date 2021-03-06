/***************************************************************************
 * libgens: Gens Emulation Library.                                        *
 * VdpPalette.hpp: VDP palette handler.                                    *
 *                                                                         *
 * Copyright (c) 1999-2002 by Stéphane Dallongeville.                      *
 * Copyright (c) 2003-2004 by Stéphane Akhoun.                             *
 * Copyright (c) 2008-2015 by David Korth.                                 *
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

#ifndef __LIBGENS_MD_VDPPALETTE_P_HPP__
#define __LIBGENS_MD_VDPPALETTE_P_HPP__

#include <stdint.h>

#include "VdpPalette.hpp"
#include "VdpTypes.hpp"

#include "macros/common.h"

namespace LibGens {

/**
 * VdpPalette private class.
 */
class VdpPalettePrivate
{
	public:
		VdpPalettePrivate(VdpPalette *q);

	private:
		VdpPalette *const q;

		// Q_DISABLE_COPY() equivalent.
		// TODO: Add LibGens-specific version of Q_DISABLE_COPY().
		VdpPalettePrivate(const VdpPalettePrivate &);
		VdpPalettePrivate &operator=(const VdpPalettePrivate &);

	public:
		/** Properties. **/
		VdpPalette::PalMode_t palMode;	// Palette mode.
		uint8_t bgColorIdx;		// Background color index.
		uint8_t maskedBgColorIdx;	// Masked background color index.
		void applyBgColorIdxMask(void);

		// TODO: Make these variables protected or private?

		/**
		 * M5/M4 bits.
		 * Used for Mega Drive and Master System.
		 * [ x  x  x  x  x  x M5 M4]
		 */
		uint8_t m5m4bits;

		/**
		 * Shadow/Highlight enable bit. (Mode 5 only)
		 */
		bool mdShadowHighlight;

		// Full TMS9918A/SMS palette.
		// SMS palette may be used in MD mode.
		// TMS9918A only uses 0x10 entries.
		union {
			uint16_t u16[0x40];
			uint32_t u32[0x40];
		} palFullSMS;

		// Full MD/GG palette.
		union {
			uint16_t u16[0x1000];
			uint32_t u32[0x1000];
		} palFullMD;

		// Full 32X palette.
		// TODO: Only allocate this if 32X mode is enabled?
		union {
			uint16_t u16[0x10000];
			uint32_t u32[0x10000];
		} palFull32X;

		/** Full palette recalculation functions. **/

		template<typename pixel,
			int RBits, int GBits, int BBits,
			int RMask, int GMask, int BMask>
		FORCE_INLINE void T_recalcFull_MD(pixel *palFullMD);

		template<typename pixel,
			int RBits, int GBits, int BBits,
			int RMask, int GMask, int BMask>
		FORCE_INLINE void T_recalcFull_32X(pixel *palFull32X);

		template<typename pixel,
			int RBits, int GBits, int BBits,
			int RMask, int GMask, int BMask>
		FORCE_INLINE void T_recalcFull_SMS(pixel *palFullSMS);

		template<typename pixel,
			int RBits, int GBits, int BBits,
			int RMask, int GMask, int BMask>
		FORCE_INLINE void T_recalcFull_GG(pixel *palFullGG);

		template<typename pixel,
			int RBits, int GBits, int BBits,
			int RMask, int GMask, int BMask>
		FORCE_INLINE void T_recalcFull_TMS9918A(pixel *palFullTMS);

		template<typename pixel,
			int RBits, int GBits, int BBits,
			int RMask, int GMask, int BMask>
		FORCE_INLINE void T_recalcFull_CGA(pixel *palFullCGA);

		void recalcFull(void);

	public:
		/**
		 * Is the system running an app-based OS?
		 * If true, palette will be limited to 16 colors.
		 */
		bool isAppOs;
};

}

#endif /* __LIBGENS_MD_VDPPALETTE_P_HPP__ */
