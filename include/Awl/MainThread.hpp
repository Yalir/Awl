
/*
 *  MainThread.hpp
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

#ifndef Awl_MainThread_hpp
#define Awl_MainThread_hpp

#include <Awl/boost/bind.hpp>
#include <Awl/boost/function.hpp>
#include <Awl/Task.hpp>

/** @file MainThread.hpp
 * @brief Contains the definitions to call functions, methods or blocks
 * on the main thread, thus avoiding concurrency issues.
 */

/** @brief Call the given @a function on the main thread
 *
 * @param function The function or static method to call
 * @return The Task object associated to that call
 */
#define AwlMainThreadCall(function) aw::MainThreadCall(boost::bind(function))

/** @brief Call the given @a method on the main thread
 *
 * @param method The (non-static) method to call
 * @param object The object targetted by the @a method
 * @return The Task object associated to that call
 */
#define AwlMainThreadMethod(method, object) aw::MainThreadCall(boost::bind(method, object))

/** @brief Start a block that is to be executed on the main thread.
 * You have to close the block with AwlCloseMainThreadBlock
 *
 * @code
 * AwlMainThreadBlock
 * {
 *    your code to execute on the main thread here
 * }
 * AwlCloseMainThreadBlock;
 * @endcode
 */
#define AwlMainThreadBlock { struct __awl_local_struct { static void __awl_main_thread_block(void)

/** @brief See AwlMainThreadBlock
 * @see AwlMainThreadBlock
 */
#define AwlCloseMainThreadBlock }; aw::MainThreadCall(boost::bind(__awl_local_struct::__awl_main_thread_block)); }

namespace aw {
	
	/** @brief Call the given callback on the main thread
	 * and get a handle on this task
	 *
	 * @param f the function or method that represents the task
	 * @return The associated Task object
	 */
	TaskRef MainThreadCall(Callback f);
	
} // namespace aw

#endif

