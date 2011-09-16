
/*
 *  WorkerThread.hpp
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

#ifndef Awl_WorkerThread_hpp
#define Awl_WorkerThread_hpp

#include <Awl/Config.hpp>
#include <Awl/Thread.hpp>

namespace awl {
	
	/** @file WorkerThread.hpp Awl/WorkerThread.hpp
	 */
	
	/** @brief Defines a thread that will grab the tasks from the ThreadPool
	 * and execute it asynchronously.
	 */
	class Awl_Api WorkerThread {
		friend class ThreadPool;
	public:
		
		/** Returns the id of the WorkerThread according to a global thread id
		 * in order to easily identify the WorkerThread that executes a specific
		 * Task.
		 *
		 * @param globalThreadId The global thread id given by
		 * Thread::GetCurrentThreadId()
		 * @param isWorkerThread Boolean set to true if @a globalThreadId
		 * represents a WorkerThread false otherwise
		 *
		 * @return The WorkerThread id if @a globalThreadId is a WorkerThread,
		 * @a globalThreadId otherwise
		 */
		static Uint64 LocalThreadId(Uint64 globalThreadId, bool& isWorkerThread);
	private:
		WorkerThread();
		~WorkerThread();
		void ThreadCallback(void);
		
		Thread m_thread;
	};
	
} // namespace awl

#endif // Awl_WorkerThread_hpp
