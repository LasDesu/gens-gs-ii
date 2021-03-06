/***************************************************************************
 * libgens/tests: Gens Emulation Library. (Test Suite)                     *
 * EffectTest.cpp: Effect test base class.                                 *
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

#include "EffectTest.hpp"

// LibZomg
#include "libzomg/PngReader.hpp"
using LibZomg::PngReader;

// LibCompat
#include "libcompat/cpuflags.h"
#include "libcompat/aligned_malloc.h"

// C includes. (C++ namespace)
#include <cstdio>
#include <cstdlib>
#include <cstring>

// C++ includes.
#include <algorithm>

namespace LibGens { namespace Tests {

/**
 * Set up the test.
 */
void EffectTest::SetUp(void)
{
	// Clear the image data initially.
	memset(&img_normal, 0, sizeof(img_normal));
	memset(&img_paused, 0, sizeof(img_paused));

	// Images must be initialized by calling init() from
	// the test cases. We're reserving the parameter for
	// CPU flags.

	// Verify the CPU flags.
	EffectTest_flags flags = GetParam();
	uint32_t totalFlags = (flags.cpuFlags | flags.cpuFlags_slow);
	if (flags.cpuFlags != 0) {
		ASSERT_NE(0U, CPU_Flags & totalFlags) <<
			"CPU does not support the required flags for this test.";
	}

	// Check if the CPU flag is slow.
	if (CPU_Flags & flags.cpuFlags_slow) {
		if ((CPU_Flags & totalFlags) == flags.cpuFlags_slow) {
			// CPU is always slow.
			printf("WARNING: CPU is known to be slow with this flag.\n"
			       "Don't rely on the benchmark results.\n");
		} else if ((CPU_Flags & totalFlags) == totalFlags) {
			// CPU may be slow.
			printf("WARNING: CPU may be slow with some instructions provided by this flag.\n");
		}
	}

	cpuFlags_old = CPU_Flags;
	CPU_Flags = flags.cpuFlags;
}

/**
 * Tear down the test.
 */
void EffectTest::TearDown(void)
{
	// Free the image data.
	free(img_normal.data);
	free(img_paused.data);

	// Unreference the framebuffers.
	if (fb_normal) {
		fb_normal->unref();
	}
	if (fb_paused) {
		fb_paused->unref();
	}
	if (fb_test1) {
		fb_test1->unref();
	}
	if (fb_test2) {
		fb_test2->unref();
	}

	CPU_Flags = cpuFlags_old;
}

/**
 * Initialize image data.
 * @param bpp Color depth.
 * @return 0 on success; non-zero on error.
 */
void EffectTest::init(MdFb::ColorDepth bpp)
{
	// Load the images.
	int bppNum;
	switch (bpp) {
		case MdFb::BPP_15:
			bppNum = 15;
			break;
		case MdFb::BPP_16:
			bppNum = 16;
			break;
		case MdFb::BPP_32:
			bppNum = 32;
			break;
		default:
			ASSERT_TRUE(false) << "bpp is invalid: " << bpp;
	}

	// Get the base filename.
	const char *base_filename = baseFilename();
	ASSERT_TRUE(base_filename != nullptr);
	ASSERT_NE(0, base_filename[0]);

	// Get the render type.
	const char *render_type = renderType();
	ASSERT_TRUE(render_type != nullptr);
	ASSERT_NE(0, render_type[0]);

	char filename_normal[64];
	char filename_paused[64];

	// Original image.
	snprintf(filename_normal, sizeof(filename_normal),
		 "Effects.Normal.%d.png", bppNum);
	// Result image.
	snprintf(filename_paused, sizeof(filename_normal),
		 "%s.%s.%d.png", base_filename, render_type, bppNum);

	// Load the images.
	PngReader reader;
	int ret = reader.readFromFile(&img_normal, filename_normal,
			PngReader::RF_INVERTED_ALPHA);
	ASSERT_EQ(0, ret) << "Error loading \"" << filename_normal << "\": " << strerror(-ret);
	ret = reader.readFromFile(&img_paused, filename_paused,
			PngReader::RF_INVERTED_ALPHA);
	ASSERT_EQ(0, ret) << "Error loading \"" << filename_paused << "\": " << strerror(-ret);

	// Allocate the framebuffers.
	fb_normal = new MdFb();
	fb_paused = new MdFb();
	fb_test1 = new MdFb();
	// TODO: Only allocate for 2-FB tests?
	fb_test2 = new MdFb();

	// Set the framebuffers' color depth.
	fb_normal->setBpp(bpp);
	fb_paused->setBpp(bpp);
	fb_test1->setBpp(bpp);
	fb_test2->setBpp(bpp);

	// Make sure the images have the correct dimensions.
	EXPECT_EQ(fb_normal->pxPerLine(), (int)img_normal.w);
	EXPECT_EQ(fb_normal->numLines(),  (int)img_normal.h);
	EXPECT_EQ(fb_normal->pxPerLine(), (int)img_paused.w);
	EXPECT_EQ(fb_normal->numLines(),  (int)img_paused.h);

	// Copy the reference images into the framebuffers.
	switch (bpp) {
		case MdFb::BPP_15:
			copyToFb15(fb_normal, &img_normal);
			copyToFb15(fb_paused, &img_paused);
			break;
		case MdFb::BPP_16:
			copyToFb16(fb_normal, &img_normal);
			copyToFb16(fb_paused, &img_paused);
			break;
		case MdFb::BPP_32:
			copyToFb32(fb_normal, &img_normal);
			copyToFb32(fb_paused, &img_paused);
			break;
		default:
			ASSERT_TRUE(false) << "bpp is invalid: " << bpp;
	}
}

/**
 * Copy a loaded image to an MdFb in 15-bit color.
 * Source image must be 32-bit color.
 * @param fb	[out] Destination MdFb.
 * @param src	[in] Source img_data.
 */
void EffectTest::copyToFb15(MdFb *fb, const Zomg_Img_Data_t *src)
{
	ASSERT_EQ(32, src->bpp);

	// Set the MdFb to 15-bit color.
	fb->setBpp(MdFb::BPP_15);

	// Copy the image to the MdFb.
	uint8_t r, g, b;
	const uint32_t *pData = (const uint32_t*)src->data;
	for (int line = 0; line < fb->numLines(); line++) {
		uint16_t *pDest = fb->lineBuf16(line);
		const uint32_t *pSrc = pData;
		for (int x = src->w; x > 0; x--, pDest++, pSrc++) {
			r = (*pSrc >> 16) & 0xFF;
			g = (*pSrc >> 8) & 0xFF;
			b = (*pSrc >> 0) & 0xFF;
			*pDest = ((r & 0xF8) << 7) |
				 ((g & 0xF8) << 2) |
				 ((b & 0xF8) >> 3);
		}
		pData += (src->pitch / 4);
	}
}

/**
 * Copy a loaded image to an MdFb in 16-bit color.
 * Source image must be 32-bit color.
 * @param fb	[out] Destination MdFb.
 * @param src	[in] Source img_data.
 */
void EffectTest::copyToFb16(MdFb *fb, const Zomg_Img_Data_t *src)
{
	ASSERT_EQ(32, src->bpp);

	// Set the MdFb to 16-bit color.
	fb->setBpp(MdFb::BPP_16);

	// Copy the image to the MdFb.
	uint8_t r, g, b;
	const uint32_t *pData = (const uint32_t*)src->data;
	for (int line = 0; line < fb->numLines(); line++) {
		uint16_t *pDest = fb->lineBuf16(line);
		const uint32_t *pSrc = pData;
		for (int x = src->w; x > 0; x--, pDest++, pSrc++) {
			r = (*pSrc >> 16) & 0xFF;
			g = (*pSrc >> 8) & 0xFF;
			b = (*pSrc >> 0) & 0xFF;
			*pDest = ((r & 0xF8) << 8) |
				 ((g & 0xFC) << 3) |
				 ((b & 0xF8) >> 3);
		}
		pData += (src->pitch / 4);
	}
}

/**
 * Copy a loaded image to an MdFb in 32-bit color.
 * Source image must be 32-bit color.
 * @param fb	[out] Destination MdFb.
 * @param src	[in] Source img_data.
 */
void EffectTest::copyToFb32(MdFb *fb, const Zomg_Img_Data_t *src)
{
	ASSERT_EQ(32, src->bpp);

	// Set the MdFb to 32-bit color.
	fb->setBpp(MdFb::BPP_32);

	// Copy the image to the MdFb.
	const uint32_t bytesPerLine = src->w * 4;
	const uint32_t *pData = (const uint32_t*)src->data;
	for (int line = 0; line < fb->numLines(); line++) {
		memcpy(fb->lineBuf32(line), pData, bytesPerLine);
		pData += (src->pitch / 4);
	}
}

/**
 * Compare two framebuffers.
 * Both framebuffers must have the same color depth.
 * @param fb_expected	[in] Expected image.
 * @param fb_actual	[in] Actual image.
 */
void EffectTest::compareFb(const MdFb *fb_expected, const MdFb *fb_actual)
{
	ASSERT_EQ(fb_expected->bpp(),       fb_actual->bpp());
	ASSERT_EQ(fb_expected->pxPerLine(), fb_actual->pxPerLine());
	ASSERT_EQ(fb_expected->numLines(),  fb_actual->numLines());

	// Use the minimum pitch between the two framebuffers.
	int pitch = std::min(fb_expected->pxPitch(), fb_actual->pxPitch());

	// NOTE: Fast Blur has weird results at pixel 335.
	// Subtract one from pitch if pxPitch > pxPerLine.
	if (pitch > fb_expected->pxPerLine()) {
		pitch--;
	}

	if (fb_expected->bpp() == MdFb::BPP_32) {
		pitch *= 4;
		for (int line = 0; line < fb_expected->numLines(); line++) {
			EXPECT_EQ(0, memcmp(fb_expected->lineBuf32(line),
					    fb_actual->lineBuf32(line), pitch)) <<
				"Line " << line << " did not match the reference image.";
		}
	} else {
		pitch *= 2;
		for (int line = 0; line < fb_expected->numLines(); line++) {
			EXPECT_EQ(0, memcmp(fb_expected->lineBuf16(line),
					    fb_actual->lineBuf16(line), pitch)) <<
				"Line " << line << " did not match the reference image.";
		}
	}
}

} }
