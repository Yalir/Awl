
/*
 *  WorkLoop.cpp
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

#include <Awl/WorkLoop.hpp>
#include <Awl/Lock.hpp>

namespace aw {
	
	WorkLoop& WorkLoop::Default(void)
	{
		static WorkLoop shared;
		return shared;
	}
	
	bool WorkLoop::Run(void)
	{
		Lock l(m_tasksMutex);
		
		while (!m_pendingTasks.empty() && m_run)
		{
			TaskRef current = m_pendingTasks.front();
			current->Execute();
			m_pendingTasks.pop();
		}
		
		return m_run;
	}
	
	void WorkLoop::Stop(void)
	{
		m_run = false;
	}
	
	void WorkLoop::ScheduleTaskForExecution(TaskRef t)
	{
		Lock l(m_tasksMutex);
		m_pendingTasks.push(t);
	}
	
	
	WorkLoop::WorkLoop(void) :
	m_run(true)
	{
		
	}
	
	WorkLoop::~WorkLoop(void)
	{
		
	}
	
} // namespace aw
