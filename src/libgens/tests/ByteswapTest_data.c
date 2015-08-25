/***************************************************************************
 * libgens/tests: Gens Emulation Library. (Test Suite)                     *
 * ByteswapTest_data.c: ByteswapTest test data.                            *
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

#include "ByteswapTest_data.h"

/**
 * Original test data.
 */
const uint8_t ByteswapTest_data_orig[516] = {
	0x4A, 0x6D, 0xF3, 0x4A, 0x23, 0x9D, 0x26, 0x3D, 0xDD, 0x24, 0xE4, 0x40, 0x84, 0x46, 0xA8, 0xEB,
	0xDF, 0xD7, 0x96, 0xDB, 0x93, 0x5C, 0xDD, 0xDA, 0x7F, 0xC2, 0xEF, 0xCF, 0xEE, 0xED, 0x40, 0x16,
	0x47, 0xB9, 0xEE, 0x26, 0x0F, 0x74, 0xB7, 0x4C, 0xC6, 0x8C, 0x6A, 0xD0, 0xC9, 0xBA, 0xA4, 0x47,
	0x57, 0xFD, 0xA3, 0x20, 0xC8, 0x59, 0xA5, 0xCC, 0x64, 0xB3, 0x01, 0xE5, 0x70, 0x3A, 0x19, 0x4F,
	0xD0, 0x55, 0x63, 0x34, 0xC7, 0x81, 0x42, 0x0F, 0xF8, 0xA7, 0xDE, 0x5D, 0xC0, 0x61, 0x9C, 0x4A,
	0x29, 0x0A, 0x58, 0x23, 0xB5, 0xFF, 0x10, 0xE2, 0xC6, 0x27, 0x06, 0x9A, 0x5C, 0xF7, 0xF0, 0x87,
	0xE4, 0xDA, 0xDB, 0x4C, 0xD6, 0xED, 0x39, 0x6F, 0x70, 0x2D, 0x68, 0xEC, 0xFE, 0xE5, 0x0D, 0xDA,
	0xC8, 0x2C, 0x37, 0xAC, 0xD6, 0x5B, 0x9E, 0xB3, 0x32, 0x4A, 0x3A, 0xB4, 0x62, 0x3E, 0xA4, 0xEA,
	0xD7, 0x42, 0x0B, 0x69, 0xE4, 0x56, 0xF2, 0xFD, 0x8B, 0x1C, 0x08, 0x83, 0xD8, 0x36, 0xD1, 0x5C,
	0x37, 0x5E, 0x04, 0xBB, 0xC7, 0x52, 0x0D, 0x75, 0x1B, 0xFF, 0x68, 0xA7, 0xE6, 0x00, 0x35, 0x34,
	0x68, 0xFB, 0x13, 0x50, 0x00, 0x90, 0x60, 0x67, 0xEE, 0x1D, 0x88, 0xFD, 0x4D, 0x90, 0x22, 0x1E,
	0x75, 0xC9, 0xF4, 0x43, 0xAE, 0x3D, 0xA9, 0xD1, 0x88, 0xEF, 0x07, 0x99, 0xA1, 0x51, 0x00, 0xE6,
	0x5C, 0x51, 0x45, 0x5B, 0x9C, 0x96, 0xBC, 0xDB, 0x47, 0x78, 0x89, 0x8E, 0xD9, 0x9D, 0x91, 0xED,
	0xEE, 0xA0, 0x57, 0xFD, 0xE0, 0x28, 0xB3, 0x0D, 0x30, 0x79, 0x78, 0xD2, 0x8D, 0xD6, 0x41, 0x3E,
	0x65, 0x58, 0x6D, 0x4F, 0x81, 0xB2, 0x8A, 0xBA, 0x19, 0x0B, 0xF6, 0x1F, 0x9F, 0x1B, 0xF1, 0xAA,
	0x9B, 0x8A, 0x0B, 0xD1, 0x77, 0x4F, 0x03, 0x54, 0x2E, 0xA0, 0x11, 0x52, 0xD7, 0xDA, 0x96, 0x49,
	0x72, 0x53, 0xFC, 0x62, 0xE8, 0x66, 0xA8, 0xB4, 0xC6, 0x67, 0xC6, 0x8B, 0xAF, 0x60, 0x73, 0x2D,
	0x05, 0xD6, 0xAE, 0x32, 0x8C, 0xB1, 0x35, 0xF7, 0x63, 0x43, 0xF3, 0xC8, 0x7B, 0xAE, 0x35, 0x4D,
	0x0A, 0x84, 0x45, 0xC0, 0x6F, 0x12, 0x6B, 0xFC, 0xB6, 0x4D, 0x92, 0x2F, 0x40, 0xB5, 0x82, 0xC7,
	0x2A, 0xB8, 0xF8, 0x1F, 0x0A, 0x18, 0x8E, 0x79, 0x2C, 0x63, 0xFF, 0xE5, 0x9C, 0x11, 0x93, 0xC6,
	0xA2, 0xA7, 0x7F, 0x3F, 0x55, 0x02, 0xD9, 0x0F, 0xDC, 0xAA, 0x1B, 0xEE, 0x7F, 0xDA, 0x4E, 0xE2,
	0x42, 0x43, 0xB8, 0x4A, 0xE4, 0x4D, 0xCE, 0x60, 0xB4, 0x8E, 0x6D, 0x56, 0x31, 0x89, 0xAB, 0x13,
	0x7C, 0x76, 0xFA, 0x35, 0x47, 0x3C, 0xAB, 0x1D, 0xE4, 0x15, 0x73, 0x7F, 0xCB, 0x3E, 0x2A, 0x92,
	0xB9, 0xA0, 0x28, 0x51, 0x62, 0xF5, 0xE9, 0x09, 0x72, 0xCF, 0xB2, 0x5F, 0xE2, 0x6F, 0x69, 0x30,
	0xFF, 0xF6, 0x15, 0x41, 0x6B, 0xA6, 0xC4, 0xBD, 0x93, 0x9A, 0xA5, 0x58, 0x2C, 0x2C, 0x2E, 0x98,
	0x7A, 0xFB, 0x17, 0xA0, 0xF8, 0x49, 0xB1, 0x81, 0xAE, 0xCC, 0x93, 0x0F, 0xF9, 0x87, 0xDA, 0x66,
	0x62, 0xA6, 0x8C, 0x85, 0x8F, 0xDF, 0x80, 0x2D, 0x14, 0xD9, 0xDB, 0xAC, 0x58, 0x26, 0x7C, 0x8D,
	0x62, 0x05, 0x24, 0xD2, 0x69, 0x13, 0x2E, 0xA5, 0x6E, 0x1A, 0x2A, 0x95, 0x45, 0x65, 0x9B, 0x57,
	0x44, 0xC9, 0xA3, 0x4F, 0x2C, 0xB7, 0x28, 0x8F, 0xE7, 0x18, 0xA7, 0x05, 0x9A, 0x55, 0xE1, 0x7D,
	0x55, 0xA0, 0x4B, 0xDC, 0x21, 0xEE, 0x06, 0x7D, 0xFB, 0x88, 0x0A, 0xC0, 0xDE, 0x78, 0x6C, 0xBD,
	0x90, 0xFC, 0x90, 0x86, 0x29, 0x03, 0x8D, 0xF2, 0x42, 0xB0, 0xFF, 0x9A, 0x2E, 0x63, 0x06, 0xD9,
	0xBB, 0x4D, 0x3D, 0x41, 0x3D, 0xD9, 0x97, 0x5D, 0x1E, 0x06, 0xC3, 0x50, 0x6F, 0x81, 0x28, 0xA8,
	0x4D, 0x6E, 0xFA, 0xC9
};

/**
 * 16-bit byteswapped test data.
 */
const uint8_t ByteswapTest_data_swap16[516] = {
	0x6D, 0x4A, 0x4A, 0xF3, 0x9D, 0x23, 0x3D, 0x26, 0x24, 0xDD, 0x40, 0xE4, 0x46, 0x84, 0xEB, 0xA8,
	0xD7, 0xDF, 0xDB, 0x96, 0x5C, 0x93, 0xDA, 0xDD, 0xC2, 0x7F, 0xCF, 0xEF, 0xED, 0xEE, 0x16, 0x40,
	0xB9, 0x47, 0x26, 0xEE, 0x74, 0x0F, 0x4C, 0xB7, 0x8C, 0xC6, 0xD0, 0x6A, 0xBA, 0xC9, 0x47, 0xA4,
	0xFD, 0x57, 0x20, 0xA3, 0x59, 0xC8, 0xCC, 0xA5, 0xB3, 0x64, 0xE5, 0x01, 0x3A, 0x70, 0x4F, 0x19,
	0x55, 0xD0, 0x34, 0x63, 0x81, 0xC7, 0x0F, 0x42, 0xA7, 0xF8, 0x5D, 0xDE, 0x61, 0xC0, 0x4A, 0x9C,
	0x0A, 0x29, 0x23, 0x58, 0xFF, 0xB5, 0xE2, 0x10, 0x27, 0xC6, 0x9A, 0x06, 0xF7, 0x5C, 0x87, 0xF0,
	0xDA, 0xE4, 0x4C, 0xDB, 0xED, 0xD6, 0x6F, 0x39, 0x2D, 0x70, 0xEC, 0x68, 0xE5, 0xFE, 0xDA, 0x0D,
	0x2C, 0xC8, 0xAC, 0x37, 0x5B, 0xD6, 0xB3, 0x9E, 0x4A, 0x32, 0xB4, 0x3A, 0x3E, 0x62, 0xEA, 0xA4,
	0x42, 0xD7, 0x69, 0x0B, 0x56, 0xE4, 0xFD, 0xF2, 0x1C, 0x8B, 0x83, 0x08, 0x36, 0xD8, 0x5C, 0xD1,
	0x5E, 0x37, 0xBB, 0x04, 0x52, 0xC7, 0x75, 0x0D, 0xFF, 0x1B, 0xA7, 0x68, 0x00, 0xE6, 0x34, 0x35,
	0xFB, 0x68, 0x50, 0x13, 0x90, 0x00, 0x67, 0x60, 0x1D, 0xEE, 0xFD, 0x88, 0x90, 0x4D, 0x1E, 0x22,
	0xC9, 0x75, 0x43, 0xF4, 0x3D, 0xAE, 0xD1, 0xA9, 0xEF, 0x88, 0x99, 0x07, 0x51, 0xA1, 0xE6, 0x00,
	0x51, 0x5C, 0x5B, 0x45, 0x96, 0x9C, 0xDB, 0xBC, 0x78, 0x47, 0x8E, 0x89, 0x9D, 0xD9, 0xED, 0x91,
	0xA0, 0xEE, 0xFD, 0x57, 0x28, 0xE0, 0x0D, 0xB3, 0x79, 0x30, 0xD2, 0x78, 0xD6, 0x8D, 0x3E, 0x41,
	0x58, 0x65, 0x4F, 0x6D, 0xB2, 0x81, 0xBA, 0x8A, 0x0B, 0x19, 0x1F, 0xF6, 0x1B, 0x9F, 0xAA, 0xF1,
	0x8A, 0x9B, 0xD1, 0x0B, 0x4F, 0x77, 0x54, 0x03, 0xA0, 0x2E, 0x52, 0x11, 0xDA, 0xD7, 0x49, 0x96,
	0x53, 0x72, 0x62, 0xFC, 0x66, 0xE8, 0xB4, 0xA8, 0x67, 0xC6, 0x8B, 0xC6, 0x60, 0xAF, 0x2D, 0x73,
	0xD6, 0x05, 0x32, 0xAE, 0xB1, 0x8C, 0xF7, 0x35, 0x43, 0x63, 0xC8, 0xF3, 0xAE, 0x7B, 0x4D, 0x35,
	0x84, 0x0A, 0xC0, 0x45, 0x12, 0x6F, 0xFC, 0x6B, 0x4D, 0xB6, 0x2F, 0x92, 0xB5, 0x40, 0xC7, 0x82,
	0xB8, 0x2A, 0x1F, 0xF8, 0x18, 0x0A, 0x79, 0x8E, 0x63, 0x2C, 0xE5, 0xFF, 0x11, 0x9C, 0xC6, 0x93,
	0xA7, 0xA2, 0x3F, 0x7F, 0x02, 0x55, 0x0F, 0xD9, 0xAA, 0xDC, 0xEE, 0x1B, 0xDA, 0x7F, 0xE2, 0x4E,
	0x43, 0x42, 0x4A, 0xB8, 0x4D, 0xE4, 0x60, 0xCE, 0x8E, 0xB4, 0x56, 0x6D, 0x89, 0x31, 0x13, 0xAB,
	0x76, 0x7C, 0x35, 0xFA, 0x3C, 0x47, 0x1D, 0xAB, 0x15, 0xE4, 0x7F, 0x73, 0x3E, 0xCB, 0x92, 0x2A,
	0xA0, 0xB9, 0x51, 0x28, 0xF5, 0x62, 0x09, 0xE9, 0xCF, 0x72, 0x5F, 0xB2, 0x6F, 0xE2, 0x30, 0x69,
	0xF6, 0xFF, 0x41, 0x15, 0xA6, 0x6B, 0xBD, 0xC4, 0x9A, 0x93, 0x58, 0xA5, 0x2C, 0x2C, 0x98, 0x2E,
	0xFB, 0x7A, 0xA0, 0x17, 0x49, 0xF8, 0x81, 0xB1, 0xCC, 0xAE, 0x0F, 0x93, 0x87, 0xF9, 0x66, 0xDA,
	0xA6, 0x62, 0x85, 0x8C, 0xDF, 0x8F, 0x2D, 0x80, 0xD9, 0x14, 0xAC, 0xDB, 0x26, 0x58, 0x8D, 0x7C,
	0x05, 0x62, 0xD2, 0x24, 0x13, 0x69, 0xA5, 0x2E, 0x1A, 0x6E, 0x95, 0x2A, 0x65, 0x45, 0x57, 0x9B,
	0xC9, 0x44, 0x4F, 0xA3, 0xB7, 0x2C, 0x8F, 0x28, 0x18, 0xE7, 0x05, 0xA7, 0x55, 0x9A, 0x7D, 0xE1,
	0xA0, 0x55, 0xDC, 0x4B, 0xEE, 0x21, 0x7D, 0x06, 0x88, 0xFB, 0xC0, 0x0A, 0x78, 0xDE, 0xBD, 0x6C,
	0xFC, 0x90, 0x86, 0x90, 0x03, 0x29, 0xF2, 0x8D, 0xB0, 0x42, 0x9A, 0xFF, 0x63, 0x2E, 0xD9, 0x06,
	0x4D, 0xBB, 0x41, 0x3D, 0xD9, 0x3D, 0x5D, 0x97, 0x06, 0x1E, 0x50, 0xC3, 0x81, 0x6F, 0xA8, 0x28,
	0x6E, 0x4D, 0xC9, 0xFA
};

/**
 * 32-bit byteswapped test data.
 */
const uint8_t ByteswapTest_data_swap32[516] = {
	0x4A, 0xF3, 0x6D, 0x4A, 0x3D, 0x26, 0x9D, 0x23, 0x40, 0xE4, 0x24, 0xDD, 0xEB, 0xA8, 0x46, 0x84,
	0xDB, 0x96, 0xD7, 0xDF, 0xDA, 0xDD, 0x5C, 0x93, 0xCF, 0xEF, 0xC2, 0x7F, 0x16, 0x40, 0xED, 0xEE,
	0x26, 0xEE, 0xB9, 0x47, 0x4C, 0xB7, 0x74, 0x0F, 0xD0, 0x6A, 0x8C, 0xC6, 0x47, 0xA4, 0xBA, 0xC9,
	0x20, 0xA3, 0xFD, 0x57, 0xCC, 0xA5, 0x59, 0xC8, 0xE5, 0x01, 0xB3, 0x64, 0x4F, 0x19, 0x3A, 0x70,
	0x34, 0x63, 0x55, 0xD0, 0x0F, 0x42, 0x81, 0xC7, 0x5D, 0xDE, 0xA7, 0xF8, 0x4A, 0x9C, 0x61, 0xC0,
	0x23, 0x58, 0x0A, 0x29, 0xE2, 0x10, 0xFF, 0xB5, 0x9A, 0x06, 0x27, 0xC6, 0x87, 0xF0, 0xF7, 0x5C,
	0x4C, 0xDB, 0xDA, 0xE4, 0x6F, 0x39, 0xED, 0xD6, 0xEC, 0x68, 0x2D, 0x70, 0xDA, 0x0D, 0xE5, 0xFE,
	0xAC, 0x37, 0x2C, 0xC8, 0xB3, 0x9E, 0x5B, 0xD6, 0xB4, 0x3A, 0x4A, 0x32, 0xEA, 0xA4, 0x3E, 0x62,
	0x69, 0x0B, 0x42, 0xD7, 0xFD, 0xF2, 0x56, 0xE4, 0x83, 0x08, 0x1C, 0x8B, 0x5C, 0xD1, 0x36, 0xD8,
	0xBB, 0x04, 0x5E, 0x37, 0x75, 0x0D, 0x52, 0xC7, 0xA7, 0x68, 0xFF, 0x1B, 0x34, 0x35, 0x00, 0xE6,
	0x50, 0x13, 0xFB, 0x68, 0x67, 0x60, 0x90, 0x00, 0xFD, 0x88, 0x1D, 0xEE, 0x1E, 0x22, 0x90, 0x4D,
	0x43, 0xF4, 0xC9, 0x75, 0xD1, 0xA9, 0x3D, 0xAE, 0x99, 0x07, 0xEF, 0x88, 0xE6, 0x00, 0x51, 0xA1,
	0x5B, 0x45, 0x51, 0x5C, 0xDB, 0xBC, 0x96, 0x9C, 0x8E, 0x89, 0x78, 0x47, 0xED, 0x91, 0x9D, 0xD9,
	0xFD, 0x57, 0xA0, 0xEE, 0x0D, 0xB3, 0x28, 0xE0, 0xD2, 0x78, 0x79, 0x30, 0x3E, 0x41, 0xD6, 0x8D,
	0x4F, 0x6D, 0x58, 0x65, 0xBA, 0x8A, 0xB2, 0x81, 0x1F, 0xF6, 0x0B, 0x19, 0xAA, 0xF1, 0x1B, 0x9F,
	0xD1, 0x0B, 0x8A, 0x9B, 0x54, 0x03, 0x4F, 0x77, 0x52, 0x11, 0xA0, 0x2E, 0x49, 0x96, 0xDA, 0xD7,
	0x62, 0xFC, 0x53, 0x72, 0xB4, 0xA8, 0x66, 0xE8, 0x8B, 0xC6, 0x67, 0xC6, 0x2D, 0x73, 0x60, 0xAF,
	0x32, 0xAE, 0xD6, 0x05, 0xF7, 0x35, 0xB1, 0x8C, 0xC8, 0xF3, 0x43, 0x63, 0x4D, 0x35, 0xAE, 0x7B,
	0xC0, 0x45, 0x84, 0x0A, 0xFC, 0x6B, 0x12, 0x6F, 0x2F, 0x92, 0x4D, 0xB6, 0xC7, 0x82, 0xB5, 0x40,
	0x1F, 0xF8, 0xB8, 0x2A, 0x79, 0x8E, 0x18, 0x0A, 0xE5, 0xFF, 0x63, 0x2C, 0xC6, 0x93, 0x11, 0x9C,
	0x3F, 0x7F, 0xA7, 0xA2, 0x0F, 0xD9, 0x02, 0x55, 0xEE, 0x1B, 0xAA, 0xDC, 0xE2, 0x4E, 0xDA, 0x7F,
	0x4A, 0xB8, 0x43, 0x42, 0x60, 0xCE, 0x4D, 0xE4, 0x56, 0x6D, 0x8E, 0xB4, 0x13, 0xAB, 0x89, 0x31,
	0x35, 0xFA, 0x76, 0x7C, 0x1D, 0xAB, 0x3C, 0x47, 0x7F, 0x73, 0x15, 0xE4, 0x92, 0x2A, 0x3E, 0xCB,
	0x51, 0x28, 0xA0, 0xB9, 0x09, 0xE9, 0xF5, 0x62, 0x5F, 0xB2, 0xCF, 0x72, 0x30, 0x69, 0x6F, 0xE2,
	0x41, 0x15, 0xF6, 0xFF, 0xBD, 0xC4, 0xA6, 0x6B, 0x58, 0xA5, 0x9A, 0x93, 0x98, 0x2E, 0x2C, 0x2C,
	0xA0, 0x17, 0xFB, 0x7A, 0x81, 0xB1, 0x49, 0xF8, 0x0F, 0x93, 0xCC, 0xAE, 0x66, 0xDA, 0x87, 0xF9,
	0x85, 0x8C, 0xA6, 0x62, 0x2D, 0x80, 0xDF, 0x8F, 0xAC, 0xDB, 0xD9, 0x14, 0x8D, 0x7C, 0x26, 0x58,
	0xD2, 0x24, 0x05, 0x62, 0xA5, 0x2E, 0x13, 0x69, 0x95, 0x2A, 0x1A, 0x6E, 0x57, 0x9B, 0x65, 0x45,
	0x4F, 0xA3, 0xC9, 0x44, 0x8F, 0x28, 0xB7, 0x2C, 0x05, 0xA7, 0x18, 0xE7, 0x7D, 0xE1, 0x55, 0x9A,
	0xDC, 0x4B, 0xA0, 0x55, 0x7D, 0x06, 0xEE, 0x21, 0xC0, 0x0A, 0x88, 0xFB, 0xBD, 0x6C, 0x78, 0xDE,
	0x86, 0x90, 0xFC, 0x90, 0xF2, 0x8D, 0x03, 0x29, 0x9A, 0xFF, 0xB0, 0x42, 0xD9, 0x06, 0x63, 0x2E,
	0x41, 0x3D, 0x4D, 0xBB, 0x5D, 0x97, 0xD9, 0x3D, 0x50, 0xC3, 0x06, 0x1E, 0xA8, 0x28, 0x81, 0x6F,
	0xC9, 0xFA, 0x6E, 0x4D
};
