
/*
 *  Unix/ConditionImpl.cpp
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

#include <Awl/Unix/ConditionImpl.hpp>
//#include "utils.h"
#include <iostream>
using namespace std;

namespace awl {
	namespace priv {
		
		ConditionImpl::ConditionImpl(int var) :
		m_isValid(true),
		m_conditionnedVar(var),
		m_cond(),
		m_mutex()
		{
			if (0 != pthread_cond_init(&m_cond, NULL))
				cerr << "pthread_cond_init() error\n";
			
			if (0 != pthread_mutex_init(&m_mutex, NULL))
				cerr << "pthread_mutex_init() error\n";
		}
		
		
		ConditionImpl::~ConditionImpl(void)
		{
			if (0 != pthread_mutex_destroy(&m_mutex))
				cerr << "pthread_cond_destroy() error\n";
			
			if (0 != pthread_cond_destroy(&m_cond))
				cerr << "pthread_cond_destroy() error\n";
		}
		
		bool ConditionImpl::waitAndRetain(int value)
		{
			pthread_mutex_lock(&m_mutex);
			
			/*if (m_conditionnedVar == value && m_isValid)
			 {
			 MT_COUT(std::cout << "cond " << (unsigned)this%1000 << " DIRECTLY validated\n");
			 }*/
			
			while (m_conditionnedVar != value && m_isValid)
			{
				//MT_COUT(std::cout << "will wait cond " << (unsigned)this%1000 << " (expected " << value << ")\n");
				pthread_cond_wait(&m_cond, &m_mutex);
			}
			
			if (m_isValid)
			{
				//MT_COUT(std::cout << "cond " << (unsigned)this%1000 << " validated\n");
				return true;
			}
			else
			{
				//MT_COUT(std::cout << "invalid condition\n");
				pthread_mutex_unlock(&m_mutex);
				return false;
			}
		}
		
		void ConditionImpl::release(int value)
		{
			m_conditionnedVar = value;
			pthread_mutex_unlock(&m_mutex);
			
			signal();
		}
		
		void ConditionImpl::lock(void)
		{
			pthread_mutex_lock(&m_mutex);
		}
		
		void ConditionImpl::setValue(int value)
		{
			// Make sure the Condition's value is not modified while retained
			pthread_mutex_lock(&m_mutex);
			//MT_COUT(std::cout << "cond " << (unsigned)this%1000 << " set to " << value << std::endl);
			m_conditionnedVar = value;
			pthread_mutex_unlock(&m_mutex);
			
			signal();
		}
		
		int ConditionImpl::value(void) const
		{
			return m_conditionnedVar;
		}
		
		void ConditionImpl::signal(void)
		{
			pthread_cond_signal(&m_cond);
		}
		
		void ConditionImpl::broadcast(void)
		{
			pthread_cond_broadcast(&m_cond);
		}
		
		void ConditionImpl::invalidate(void)
		{
			if (m_isValid)
			{
				m_isValid = false;
				broadcast();
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

