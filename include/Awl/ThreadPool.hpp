
/*
 *  ThreadPool.hpp
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

#ifndef Awl_ThreadPool_hpp
#define Awl_ThreadPool_hpp

#include <queue>
#include <set>
#include <Awl/Condition.hpp>
#include <Awl/Task.hpp>

namespace awl {
	
	/** @file ThreadPool.hpp Awl/ThreadPool.hpp
	 */
	
	class ThreadPoolConstructor;
	
	/** @brief Defines a manager for the different threads that will execute
	 * the asynchronous Tasks.
	 */
	class Awl_Api ThreadPool {
		friend class WorkerThread;
		friend class ThreadPoolConstructor;
	public:
		/** Returns the ThreadPool instance
		 *
		 * @return The ThreadPool instance
		 */
		static ThreadPool& Default();
		
		/** Registers a Task to be executed by one of the thread pool's threads
		 *
		 * @param t The Task to register
		 */
		void ScheduleTaskForExecution(TaskRef t);
		
		void KillWorkerThread(WorkerThread *worker);
		
		// Not to be used but public for private convenience
		ThreadPool(int, int , int);
		~ThreadPool(void);
	private:
		void Init(void);
		
		bool HasPendingTask_unprotected(void);
		bool WaitForTask(TaskRef& t);
		
		std::queue<TaskRef> m_pendingTasks;
		Condition m_hasPendingTask;
		std::set<WorkerThread *> m_threads;
	};
	
} // namespace awl

#endif
