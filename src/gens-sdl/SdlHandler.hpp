/***************************************************************************
 * gens-sdl: Gens/GS II basic SDL frontend.                                *
 * SdlHandler.hpp: SDL library handler.                                    *
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

#ifndef __GENS_SDL_SDLHANDLER_HPP__
#define __GENS_SDL_SDLHANDLER_HPP__

// C includes.
#include <stdint.h>

// C includes. (C++ namespace)
#include <cstdarg>

// SDL
#include <SDL.h>

#include "libgenskeys/GensKey_t.h"
namespace LibGens {
	class MdFb;
}

// utf8_str
#include "libgens/macros/common.h"

// TODO: Minimum gcc version, other compilers?
// TODO: Move to libgens/macros/common.h?
#ifdef __GNUC__
#define ATTR_FORMAT_PRINTF(fmt, varargs) \
	__attribute__ ((format (printf, (fmt), (varargs))))
#else
#define ATTR_FORMAT_PRINTF(fmt, varargs)
#endif

namespace GensSdl {

class RingBuffer;
class VBackend;

class SdlHandler {
	public:
		SdlHandler();
		~SdlHandler();

	private:
		// Q_DISABLE_COPY() equivalent.
		// TODO: Add GensSdl-specific version of Q_DISABLE_COPY().
		SdlHandler(const SdlHandler &);
		SdlHandler &operator=(const SdlHandler &);

	public:
		/**
		 * Initialize SDL video.
		 * TODO: Parameter for GL rendering.
		 * @return 0 on success; non-zero on error.
		 */
		int init_video(void);

		/**
		 * Shut down SDL video.
		 */
		void end_video(void);

		/**
		 * Set the window title.
		 * TODO: Set based on "paused" and fps values?
		 * @param title Window title.
		 */
		void set_window_title(const char *title);

		/**
		 * Set the SDL video source to an MdFb.
		 * If nullptr, removes the SDL video source.
		 * @param fb MdFb.
		 */
		void set_video_source(LibGens::MdFb *fb);

		/**
		 * Update video.
		 */
		void update_video(void);

		/**
		 * Update video while emulation is paused.
		 * If the VBackend is dirty, video is updated;
		 * otherwise, nothing happens.
		 * NOTE: This function does NOT update the frame counter.
		 */
		void update_video_paused(void);

		/**
		 * Resize the video renderer.
		 * @param width Width.
		 * @param height Height.
		 */
		void resize_video(int width, int height);

		/**
		 * Toggle fullscreen.
		 */
		void toggle_fullscreen(void);
		
		/**
		 * Get the Video Backend.
		 * @return Video Backend.
		 */
		VBackend *vBackend(void) const;

		/**
		 * Initialize SDL audio.
		 * @return 0 on success; non-zero on error.
		 */
		int init_audio(void);

		/**
		 * Shut down SDL audio.
		 */
		void end_audio(void);

		/**
		 * Pause SDL audio.
		 * This resets the audio ringbuffer.
		 * @param paused True to pause; false to unpause.
		 */
		void pause_audio(bool pause);

		/**
		 * Update SDL audio using SoundMgr.
		 */
		void update_audio(void);

		/**
		 * Convert an SDL2 scancode to a Gens keycode.
		 * @param scancode SDL2 scancode.
		 * @return Gens keycode, or 0 if unsupported.
		 */
		static GensKey_t scancodeToGensKey(SDL_Scancode scancode);

		/** Onscreen Display functions. **/

		/**
		 * Print a message to the Onscreen Display.
		 * @param duration Duration for the message to appear, in milliseconds.
		 * @param msg Message. (printf-formatted; UTF-8)
		 * @param ap Format arguments.
		 */
		void osd_vprintf(const int duration, const utf8_str *msg, va_list ap)
			ATTR_FORMAT_PRINTF(3, 0);

		/**
		 * Print a message to the Onscreen Display.
		 * @param duration Duration for the message to appear, in milliseconds.
		 * @param msg Message. (printf-formatted; UTF-8)
		 * @params ... Format arguments.
		 */
		void osd_printf(const int duration, const utf8_str *msg, ...)
			ATTR_FORMAT_PRINTF(3, 4);

	private:
		/**
		 * SDL audio callback.
		 * @param userdata SdlHandler class pointer.
		 * @param stream SDL audio stream.
		 * @param len Number of bytes requested.
		 */
		static void sdl_audio_callback(void *userdata, uint8_t *stream, int len);

	private:
		// Video backend.
		VBackend *m_vBackend;

		// Frames rendered.
		int m_framesRendered;

		// Audio.
		SDL_AudioDeviceID m_audioDevice;
		RingBuffer *m_audioBuffer;
		int m_sampleSize;

		// Segment buffer.
		int16_t *m_segBuffer;
		// Length of m_segBuffer, in bytes.
		unsigned int m_segBufferLen;
		// Number of samples in m_segBuffer.
		unsigned int m_segBufferSamples;
};

}

#endif /* __GENS_SDL_SDLHANDLER_HPP__ */