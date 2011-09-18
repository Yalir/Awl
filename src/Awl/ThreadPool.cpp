
/*
 *  ThreadPool.cpp
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

#include <Awl/ThreadPool.hpp>
#include <Awl/WorkerThread.hpp>
#include <Awl/Mutex.hpp>
#include <Awl/Lock.hpp>
#include <Awl/Debug.hpp>
#include <Awl/Thread.hpp>

namespace awl {

	namespace {
		ThreadPool shared(1, 1, 1);
	}
	
#define THREAD_COUNT 10
	
	ThreadPool& ThreadPool::Default()
	{
		shared.Init();
		return shared;
	}
	
	void ThreadPool::WaitAndDie(void)
	{
		Default().DoWaitAndDie();
	}
	
	void ThreadPool::ScheduleTaskForExecution(TaskRef t)
	{
		m_hasPendingTask.Lock();
		m_pendingTasks.push(t);
		m_hasPendingTask.Unlock(1);
		//m_hasPendingTask = 1;
	}
	
	void ThreadPool::KillWorkerThread(WorkerThread *worker)
	{
		std::set<WorkerThread *>::iterator it = m_threads.find(worker);
		
		if (it != m_threads.end())
		{
			m_threads.insert(m_threads.end(), new WorkerThread());
			(*it)->Die();
		}
		else
		{
			MT_DEBUG_COUT(std::cout << "Worker thread not found in list." << std::endl);
		}
	}
	
	bool ThreadPool::WaitForTask(TaskRef& t)
	{
		bool res = m_hasPendingTask.WaitAndLock(1);
		
		if (res)
		{
			t = m_pendingTasks.front();
			m_pendingTasks.pop();
			m_hasPendingTask.Unlock(HasPendingTask_unprotected());
		}
		
		return res;
	}
	
	void ThreadPool::DoWaitAndDie()
	{
		m_hasPendingTask.WaitAndLock(0, Condition::AutoUnlock);
		m_hasPendingTask.Invalidate();
		
		// Clean threads' set
		std::set<WorkerThread *>::iterator it = m_threads.begin();
		while (it != m_threads.end())
		{
			WorkerThread *w = *it;
			delete w;
			m_threads.erase(it);
			it++;
		}
	}

	
	ThreadPool::ThreadPool(int, int, int)
	{
		Thread::RegisterMainThread();
	}
	
	void ThreadPool::Init(void)
	{
		static awl::Mutex initMutex;
		static bool initialized = false;
		
		awl::Lock l(initMutex);
		if (!initialized)
		{
			for (int i = 0; i < THREAD_COUNT;i++)
			{
				WorkerThread *w = new WorkerThread();
				m_threads.insert(m_threads.end(), w);
			}
			initialized = true;
		}
	}
	
	
	ThreadPool::~ThreadPool()
	{
		DoWaitAndDie();
	}
	
	bool ThreadPool::HasPendingTask_unprotected(void)
	{
		return (m_pendingTasks.empty() == false);
	}

} // namespace awl
