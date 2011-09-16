
/*
 *  Condition.cpp
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

#include <Awl/Condition.hpp>

#ifdef Awl_SystemWindows
#include <Awl/Win32/ConditionImpl.hpp>
#else
#include <Awl/Unix/ConditionImpl.hpp>
#endif

namespace awl {
	const bool Condition::AutoUnlock = true;
	const bool Condition::ManualUnlock = false;
	
	
	Condition::Condition(int value) :
	m_impl(NULL)
	{
		m_impl = new priv::ConditionImpl(value);
	}
	
	Condition::~Condition(void)
	{
		delete m_impl;
	}
	
	bool Condition::WaitAndLock(int awaitedValue, bool autorelease)
	{
		bool flag = m_impl->waitAndRetain(awaitedValue);
		
		if (autorelease)
			m_impl->release(awaitedValue);
		
		return flag;
	}
	
	void Condition::Lock(void)
	{
		m_impl->lock();
	}
	
	void Condition::Unlock(int value)
	{
		m_impl->release(value);
	}
	
	int Condition::operator=(int value)
	{
		m_impl->setValue(value);
		return value;
	}
	
	int Condition::GetValue(void) const
	{
		return m_impl->value();
	}
	
	void Condition::Signal(void)
	{
		m_impl->signal();
	}
	
	void Condition::Broadcast(void)
	{
		m_impl->broadcast();
	}
	
	void Condition::Invalidate(void)
	{
		m_impl->invalidate();
	}
	
	void Condition::Restore(void)
	{
		m_impl->restore();
	}
	
} // namespace awl
