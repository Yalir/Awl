
/*
 *  Debug.cpp
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

#include <Awl/Debug.hpp>
#include <Awl/Thread.hpp>
#include <Awl/WorkerThread.hpp>
#include <Awl/Mutex.hpp>
#include <string>
#include <iostream>

aw::Mutex __mt_cout_mutex;
	
namespace aw {
	
	namespace priv {
		
		void do_display_thread_id(const char *function, const char *file, unsigned int line)
		{
			std::string shortenedFile = file;
			
			if (shortenedFile.length() > 20)
			{
				shortenedFile = shortenedFile.substr(shortenedFile.length() - 20);
				shortenedFile = "..." + shortenedFile;
			}
			bool worker;
			Uint64 threadId = WorkerThread::LocalThreadId(Thread::GetCurrentThreadId(), worker);
			
			if (worker)
			{
				MT_COUT(std::cout << function << "() in " << shortenedFile
						<< " line " << line << " called from worker thread "
						<< threadId << std::endl);
			}
			else
			{
				MT_COUT(std::cout << function << "() in " << shortenedFile
						<< " line " << line << " called from thread "
						<< threadId << std::endl);
			}
			
		}
	} // namespace priv
} // namespace aw
