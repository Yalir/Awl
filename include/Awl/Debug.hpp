
/*
 *  Debug.hpp
 *  Awl - Asynchronous Work Library
 *
 *  Copyright (c) 2011 Lucas Soltic
 *  ceylow@gmail.com
 *
 *  This software is provided 'as-is', without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from
 *  the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it freely,
 *  subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *     you must not claim that you wrote the original software.
 *     If you use this software in a product, an acknowledgment
 *     in the product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *     and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 *
 */

#ifndef Awl_Debug_hpp
#define Awl_Debug_hpp

#include <Awl/Mutex.hpp>
#include <Awl/Lock.hpp>

#define DISPLAY_THREAD_ID aw::priv::do_display_thread_id(__func__, __FILE__, __LINE__)

extern aw::Mutex __mt_cout_mutex;

/** Thread-safe cout output
 *
 * @code
 * MT_COUT(std::cout << "message" << std::endl);
 * @endcode
 */
#define MT_COUT(sequence) \
{ aw::Lock l(__mt_cout_mutex); sequence; }

#ifndef NDEBUG
#define MT_DEBUG_COUT(sequence) MT_COUT(sequence)
#else
#define MT_DEBUG_COUT(sequence)
#endif



namespace aw {
	namespace priv {
		
		void do_display_thread_id(const char *function, const char *file, unsigned int line);
		
	} // namespace priv
} // namespace aw

#endif
