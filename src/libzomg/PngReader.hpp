/***************************************************************************
 * libzomg: Zipped Original Memory from Genesis.                           *
 * PngReader.hpp: PNG image reader.                                        *
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

#ifndef __LIBZOMG_PngReader_HPP__
#define __LIBZOMG_PngReader_HPP__

#include "minizip/zip.h"

// Image data struct.
extern "C" struct _Zomg_Img_Data_t;

namespace LibZomg {

class PngReaderPrivate;
class PngReader
{
	public:
		PngReader();
		~PngReader();

	protected:
		friend class PngReaderPrivate;
		PngReaderPrivate *const d;
	private:
		// Q_DISABLE_COPY() equivalent.
		// TODO: Add LibZomg-specific version of Q_DISABLE_COPY().
		PngReader(const PngReader &);
		PngReader &operator=(const PngReader &);

	public:
		// TODO: Better memory allocation semantics?
		/**
		 * Read an image from a PNG file in memory.
		 * Image is always loaded as 32-bit xBGR.
		 * @param img_data Image data. (Caller must free img_data->data.)
		 * @param png_file PNG file data.
		 * @param png_size Size of PNG file data.
		 * @return 0 on success; negative errno on error.
		 */
		int readFromMem(_Zomg_Img_Data_t *img_data, const void *png_file, size_t png_size);
};

}

#endif /* __LIBZOMG_PngReader_HPP__ */
