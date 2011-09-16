
/*
 *  Lock.hpp (imported from SFML)
 *  Awl - Asynchronous Work Library
 *
 *  Copyright (c) 2011 Lucas Soltic
 *  ceylow@gmail.com
 *
 *  SFML - Simple and Fast Multimedia Library
 *  Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef Awl_Lock_hpp
#define Awl_Lock_hpp

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <Awl/boost/noncopyable.hpp>
#include <Awl/Mutex.hpp>

namespace awl
{
	////////////////////////////////////////////////////////////
	/// \file Lock.hpp Awl/Lock.hpp
	/// \brief Automatic wrapper for locking and unlocking mutexes
	///
	////////////////////////////////////////////////////////////
	class Awl_Api Lock : boost::noncopyable
	{
	public:
		
		////////////////////////////////////////////////////////////
		/// \brief Construct the lock with a target mutex
		///
		/// The mutex passed to awl::Lock is automatically locked.
		///
		/// \param mutex Mutex to lock
		///
		////////////////////////////////////////////////////////////
		Lock(Mutex& mutex);
		
		////////////////////////////////////////////////////////////
		/// \brief Destructor
		///
		/// The destructor of awl::Lock automatically unlocks its mutex.
		///
		////////////////////////////////////////////////////////////
		~Lock();
		
	private:
		
		////////////////////////////////////////////////////////////
		// Member data
		////////////////////////////////////////////////////////////
		Mutex& myMutex; ///< Mutex to lock / unlock
	};
	
} // namespace awl


#endif // Awl_Lock_hpp


////////////////////////////////////////////////////////////
/// \class awl::Lock
/// \ingroup system
///
/// awl::Lock is a RAII wrapper for awl::Mutex. By unlocking
/// it in its destructor, it ensures that the mutex will
/// always be released when the current scope (most likely
/// a function) ends.
/// This is even more important when an exception or an early
/// return statement can interrupt the execution flow of the
/// function.
///
/// For maximum robustness, awl::Lock should always be used
/// to lock/unlock a mutex.
///
/// Usage example:
/// \code
/// awl::Mutex mutex;
/// 
/// void function()
/// {
///     awl::Lock lock(mutex); // mutex is now locked
/// 
///     functionThatMayThrowAnException(); // mutex is unlocked if this function throws
/// 
///     if (someCondition)
///         return; // mutex is unlocked
/// 
/// } // mutex is unlocked
/// \endcode
///
/// Because the mutex is not explicitely unlocked in the code,
/// it may remain locked longer than needed. If the region
/// of the code that needs to be protected by the mutex is
/// not the entire function, a good practice is to create a
/// smaller, inner scope so that the lock is limited to this
/// part of the code.
///
/// \code
/// awl::Mutex mutex;
/// 
/// void function()
/// {
///     {
///       awl::Lock lock(mutex);
///       codeThatRequiresProtection();
/// 
///     } // mutex is unlocked here
/// 
///     codeThatDoesntCareAboutTheMutex();
/// }
/// \endcode
///
/// Having a mutex locked longer than required is a bad practice
/// which can lead to bad performances. Don't forget that when
/// a mutex is locked, other threads may be waiting doing nothing
/// until it is released.
///
/// \see awl::Mutex
///
////////////////////////////////////////////////////////////
