
/*
 *  Condition.hpp
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

#ifndef Awl_Condition_hpp
#define Awl_Condition_hpp

#include <Awl/Config.hpp>

namespace aw {
	
	namespace priv {
		class ConditionImpl;
	}
	
	/** @brief Defines a classical Condition class
	 */
	class Condition {
	public:
		/** Constant for arg 2 of WaitAndLock()
		 */
		static const bool AutoUnlock;	// true
		
		/** Constant for arg 2 of WaitAndLock()
		 */
		static const bool ManualUnlock;	// false
		
		/** Initializes a Condition object and sets its internal value to @a value.
		 * Thus using WaitAndLock(@a value, ...) will immediately return.
		 */
		Condition(int value = 0);
		
		/** Default destructor
		 * The Condition is invalidated before destruction
		 */
		~Condition(void);
		
		/** Waits until the Condition's @a value == @a awaitedValue and protects the Condition.
		 * You're responsible for unlocking the Condition with Unlock() after
		 * WaitAndLock() returned and after you're done working on protected data,
		 * or enabling the auto unlocking mechanism.
		 *
		 * The Condition locking guarantees that the condition remains true until
		 * you unlock it and that you are the only one that acquired the Condition.
		 *
		 * @param awaitedValue the value that should unlock the Condition
		 *
		 * @param autoUnlock Condition::AutoUnlock to automatically unlock the Condition
		 * protection after it has been validated, or Condition::ManualUnlock to
		 * manually choose when the Condition should be unlocked. While a Condition
		 * is locked, both WaitAndLock() and operator=() will block
		 * until the Condition is unlocked or invalidated. When a Condition is
		 * *automatically* unlocked, its value is not updated.
		 *
		 * @return true if the @a awaitedValue has been reached, false otherwise.
		 * WaitAndLock() may return even if @a awaitedValue has not been
		 * reached if the Condition has been disabled through Invalidate(). An
		 * invalidated Condition always returns in an unlocked state.
		 */
		bool WaitAndLock(int awaitedValue, bool autoUnlock = false);
		
		/** Locks the Condition without waiting for any state
		 */
		void Lock(void);
		
		/** Unlocks a previously locked Condition with @a value as
		 * internal value. When the condition is unlocked, it is assumed
		 * to have the given value. The condition is thereafter signaled.
		 * Unlocking a non-locked Condition is undefined.
		 *
		 * @param value the value the Condition should have when it is unlocked
		 */
		void Unlock(int value);
		
		/** Performs an assignement followed by a signal() call.
		 * The internal Condition value is updated to @a value and the Condition is
		 * signaled. Note that the Condition must be unlocked in order
		 * to be updated, otherwise it'll block until the Condition
		 * is unlocked.
		 *
		 * @param value the value to be assigned to the Condition
		 *
		 * @return @a value
		 */
		int operator=(int value);
		
		/** Get the current internal Condition value.
		 * This is a non-blocking call.
		 *
		 * @return the current internal state
		 */
		int GetValue(void) const;
		
		/** Signals one awaiting Condition that the Condition state has changed
		 * and that threads waiting on this Condition should check
		 * the new internal value.
		 */
		void Signal(void);
		
		/** Signals all of the awaiting Conditions that the Condition state has changed
		 * and that threads waiting on this Condition should check
		 * the new internal value.
		 */
		void Broadcast(void);
		
		/** Signals the Condition and disables blocking calls,
		 * thus WaitAndLock() does no more wait whatever
		 * the @a awaitedValue is and waiting calls are unlocked, returning false.
		 */
		void Invalidate(void);
		
		/** Restores the blocking capabilities of the Condition,
		 * possibly previously disabled with Invalidate() 
		 */
		void Restore(void);
		
	private:
		priv::ConditionImpl *m_impl;
	};
	
} // namespace aw

#endif // Awl_Condition_hpp

