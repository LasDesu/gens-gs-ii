/***************************************************************************
 * libgens: Gens Emulation Library.                                        *
 * VdpRend_static.hpp: VDP rendering class. (Static member init)           *
 *                                                                         *
 * Copyright (c) 1999-2002 by Stéphane Dallongeville.                      *
 * Copyright (c) 2003-2004 by Stéphane Akhoun.                             *
 * Copyright (c) 2008-2010 by David Korth.                                 *
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

#ifndef __LIBGENS_MD_VDPREND_STATIC_HPP__
#define __LIBGENS_MD_VDPREND_STATIC_HPP__

#include "Vdp.hpp"
#include "VdpPalette.hpp"

namespace LibGens
{

/** Static member initialization. **/

// Palette manager.
VdpPalette Vdp::m_palette;

// MD framebuffer.
MdFb Vdp::MD_Screen;

// Sprite structs.
Vdp::Sprite_Struct_t Vdp::Sprite_Struct[128];
unsigned int Vdp::Sprite_Visible[128];

// VDP layer control.
unsigned int Vdp::VDP_Layers = Vdp::VDP_LAYERS_DEFAULT;

// Line buffer for current line.
Vdp::LineBuf_t Vdp::LineBuf;

}

#endif /* __LIBGENS_MD_VDPREND_STATIC_HPP__ */
