
/*
 *  Win32/ConditionImpl.cpp
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

#include <Awl/Win32/ConditionImpl.hpp>

namespace awl {
	namespace priv {
		
		ConditionImpl::ConditionImpl(int var) :
		m_isValid(true),
		m_conditionnedVar(var),
		m_mutex()
		{
			m_cond = CreateEvent(NULL, FALSE, FALSE, NULL);
			
			if (m_cond == NULL)
				std::cerr << "ConditionImpl() - CreateEvent() error\n";
		}
		
		ConditionImpl::~ConditionImpl(void)
		{
			CloseHandle(m_cond);
		}
		
		bool ConditionImpl::waitAndRetain(int value)
		{
			m_mutex.Lock();
			
			while (m_conditionnedVar != value && m_isValid)
			{
				m_mutex.Unlock();
				WaitForSingleObject(m_cond, INFINITE);
				m_mutex.Lock();
			}
			
			if (m_isValid)
				return true;
			else
			{
				m_mutex.Unlock();
				return false;
			}
		}

		void ConditionImpl::lock(void)
		{
			m_mutex.Lock();
		}
		
		void ConditionImpl::release(int value)
		{
			m_conditionnedVar = value;
			m_mutex.Unlock();
			
			signal();
		}
		
		void ConditionImpl::setValue(int value)
		{
			// Make sure the Condition's value is not modified while retained
			m_mutex.Lock();
			m_conditionnedVar = value;
			m_mutex.Unlock();
			
			signal();
		}
		
		int ConditionImpl::value(void) const
		{
			return m_conditionnedVar;
		}
		
		void ConditionImpl::signal(void)
		{
			SetEvent(m_cond);
		}
		
		void ConditionImpl::broadcast(void)
		{
			
		}
		
		void ConditionImpl::invalidate(void)
		{
			if (m_isValid)
			{
				m_isValid = false;
				signal();
			}
		}
		void ConditionImpl::restore(void)
		{
			if (!m_isValid)
			{
				m_isValid = true;
			}
		}
		
	} // namespace priv
} // namespace awl

