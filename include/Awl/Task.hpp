
/*
 *  Task.hpp
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

#ifndef Awl_Task_hpp
#define Awl_Task_hpp

#include <Awl/Config.hpp>
#include <Awl/Types.hpp>
#include <Awl/Condition.hpp>
#include <Awl/boost/shared_ptr.hpp>
#include <Awl/boost/noncopyable.hpp>

namespace awl {
	
	/** @file Task.hpp Awl/Task.hpp
	 *
	 * @brief Defines the Task control class that allows
	 * manual handling such as launching, cancelling or aborting.
	 *
	 * @details A Task is mainly defined by its callback function.
	 * Tasks can be scheduled for asynchronous execution through
	 * the ThreadPool singleton, or for main-threaded execution through
	 * the WorkLoop singleton.
	 */
	
	class WorkerThread;
	/** @brief Task is mainly defined by a callback function and allows
	 * asynchronous or synchronous execution, cancellation and abort.
	 */
	class Awl_Api Task : boost::noncopyable {
		friend class WorkerThread;
		friend class WorkLoop;
	public:
		/** @brief Empty constructor to allow temporary (but unusable) Task objects
		 */
		Task(void);
		
		/** @brief Default constructor bound to the given @a f callback
		 * @details This constructor initializes a new Task object given to
		 * the given callback. The constructed task is neither executed
		 * nor scheduled for execution until registered in the ThreadPool
		 * or WorkLoop singletons.
		 *
		 * @param f The function that represents the task. It must have the
		 * following signature: void function(void)
		 */
		Task(Callback f);
		
		//Task(const Task& other);
		//Task& operator=(const Task& other);
		
		/** @brief Default destructor
		 */
		~Task(void);
		
		/** @brief Cancel the Task
		 *
		 * @details If the Task is already being executed, it's marked as
		 * cancelled and the callback is itself responsible for checking
		 * that flag through IsCancelled() and stopping its work as quickly
		 * as possible.
		 * If the Task hasn't been started yet, it's also marked as cancelled
		 * but the bound callback will not be called.
		 */
		void Cancel(void);
		
		/** @brief Abort the Task
		 *
		 * @details The Task is marked as cancelled. If the Task hasn't begun yet,
		 * it won't be executed.
		 * If it's already started from a WorkerThread, the corresponding thread
		 * is aborted and does not let the callback function clean its work.
		 * If it's already started from a WorkLoop, it only remains in a cancelled
		 * state and has no effect if the callback function doesn't check
		 * the cancellation flag.
		 */
		void Abort(void);
		
		/** @brief Returns whether a Task has been cancelled
		 *
		 * @details A Task can be cancelled through Cancel() and Abort().
		 * You're responsible for checking the cancelled state to stop
		 * your work as quickly as possible.
		 * If a Task is cancelled before its start, it's not executed.
		 *
		 * @return A boolean value indicating whether the Task is cancelled
		 */
		bool IsCancelled(void) const;
		
		/** @brief Returns whether a Task has been completed.
		 *
		 * @details A Task is marked as completed as soon as it exits
		 * its bound block/function/method, even if the Task has been canceled
		 * during execution.
		 *
		 * @return A boolean value indicating whether the Task has finished running
		 */
		bool IsOver(void) const;
		
		/** @brief Wait until the task is over
		 *
		 * @details If the Task is to be executed on another thread, this
		 * will block the current thread and its execution flow until
		 * the Task has been completed.
		 * If the Task is to be executed on the same thread, this call has
		 * no synchronization effect and will return false.
		 *
		 * @return true is the Task has been performed on another thread, false
		 * otherwise (also meaning the Task may not have been performed)
		 */
		bool Wait(void);
		
	private:
		void Execute(WorkerThread& owner);
		void Execute(void); // from work loop
		
		Callback m_callback;
		bool m_isCancelled;
		bool m_isOver;
		WorkerThread *m_owner;
		Uint64 m_threadId;
		Condition m_taskDone;
	};
	
	/** Defines an automatically released and shared
	 * Task object.
	 */
	typedef boost::shared_ptr<Task> TaskRef;
	
} // namespace awl

#endif
