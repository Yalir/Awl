
/*
 *  WorkerThread.cpp
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

#include <Awl/WorkerThread.hpp>
#include <Awl/ThreadPool.hpp>
#include <Awl/Task.hpp>
#include <Awl/Mutex.hpp>
#include <Awl/Lock.hpp>
#include <Awl/Sleep.hpp>
#include <Awl/Debug.hpp>
#include <map>

namespace awl {
	
	static std::map<Uint64, Uint64> g_thread_table;
	static unsigned int g_thread_counter = 0;
	static awl::Mutex g_thread_table_mutex;
	
	WorkerThread::WorkerThread() :
	m_thread(&WorkerThread::ThreadCallback, this)
	{
		m_thread.Launch();
	}
	
	WorkerThread::~WorkerThread()
	{
		m_thread.Wait();
	}
	
	void WorkerThread::ThreadCallback(void)
	{
		{
			Lock l(g_thread_table_mutex);
			g_thread_table[Thread::GetCurrentThreadId()] = g_thread_counter++;
		}
		
		TaskRef t;
		while (ThreadPool::Default().WaitForTask(t))
		{
			t->Execute(*this);
			
			// Ease thread switching for some OS
			Sleep(0);
		}
	}
	
	void WorkerThread::Die(void)
	{
		m_thread.Terminate();
	}
	
	
	Uint64 WorkerThread::LocalThreadId(Uint64 globalThreadId, bool& isWorkerThread)
	{
		Uint64 res = 0;
		
		Lock l(g_thread_table_mutex);
		std::map<Uint64, Uint64>::iterator it;
		
		it = g_thread_table.find(globalThreadId);
		if (it != g_thread_table.end())
			res = it->second, isWorkerThread = true;
		else
			res = globalThreadId, isWorkerThread = false;
		
		return res;
	}
	
} // namespace awl

