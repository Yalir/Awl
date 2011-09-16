
/*
 *  WorkLoop.hpp
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

#ifndef Awl_WorkLoop_hpp
#define Awl_WorkLoop_hpp

#include <Awl/boost/noncopyable.hpp>
#include <Awl/Mutex.hpp>
#include <Awl/Task.hpp>
#include <queue>

namespace aw {
	
	/** @brief Defines a class for executing functions, methods or blocks on
	 * the main thread
	 */
	class Awl_Api WorkLoop : boost::noncopyable {
	public:
		/** @brief Return the default WorkLoop instance
		 * 
		 * @return The WorkLoop instance
		 */
		static WorkLoop& Default(void);
		
		/** @brief Executes the awaiting tasks
		 *
		 * @return false is the WorkLoop has been stopped, true otherwise
		 */
		bool Run(void);
		
		/** @brief Marks the WorkLoop as stopped
		 */
		void Stop(void);
		
		/** @brief Registers a Task for later execution on the main thread.
		 *
		 * @details The registered Task is executed when Run() is called.
		 * @param t The Task to execute on the main thread
		 */
		void ScheduleTaskForExecution(TaskRef t);
		
	private:
		WorkLoop(void);
		~WorkLoop(void);
		
		Mutex m_tasksMutex;
		std::queue<TaskRef> m_pendingTasks;
		bool m_run;
	};
	
} // namespace aw

#endif // Awl_WorkLoop_hpp
