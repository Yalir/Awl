
/*
 *  Task.cpp
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

#include <Awl/Task.hpp>
#include <Awl/Debug.hpp>
#include <Awl/Thread.hpp>
#include <Awl/ThreadPool.hpp>

namespace awl {
	
	Task::Task(void) :
	m_callback(),
	m_isCancelled(false),
	m_owner(NULL),
	m_threadId(-1),
	m_taskDone()
	{
		
	}
	
	Task::Task(Callback f) :
	m_callback(f),
	m_isCancelled(false),
	m_owner(NULL),
	m_threadId(-1),
	m_taskDone()
	{
		
	}
		
	Task::~Task(void)
	{
		
	}
	
	void Task::Cancel(void)
	{
		m_isCancelled = true;
	}
	
	void Task::Abort(void)
	{
		Cancel();
		
		if (m_owner)
		{
			ThreadPool::Default().KillWorkerThread(m_owner);
		}
	}
	
	bool Task::IsCancelled(void) const
	{
		return m_isCancelled;
	}
	
	bool Task::Wait(void)
	{
		if (m_threadId == Thread::GetCurrentThreadId())
		{
			MT_DEBUG_COUT(std::cout << "trying to wait on same thread" << std::endl);
			return false;
		}
		else
		{
			m_taskDone.WaitAndLock(1, Condition::AutoUnlock);
			return true;
		}
	}
	
	void Task::Execute(WorkerThread& owner)
	{
		// Save the worker thread from which we're executing the task
		m_owner = &owner;
		Execute();
	}
	
	void Task::Execute(void)
	{
		m_threadId = Thread::GetCurrentThreadId();
		
		if (!m_isCancelled)
			m_callback();
		
		m_taskDone = 1;
	}
	
} // namespace awl
