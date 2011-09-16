
/*
 *  Async.hpp
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

#ifndef Awl_Async_hpp
#define Awl_Async_hpp

#include <Awl/boost/bind.hpp>
#include <Awl/boost/function.hpp>
#include <Awl/Task.hpp>
#include <Awl/ThreadPool.hpp>

/** @file Async.hpp Awl/Async.hpp
 * @brief Contains the definitions to call functions, methods or blocks
 * in an asynchronous way, thus allowing parallel computing.
 */

/** @brief Call the given @a function in an asynchronous way
 *
 * @param function The function or static method to call
 * @return The Task object associated to that call
 */
#define AwlAsyncCall(function) awl::AsyncCall(boost::bind(function))

/** @brief Call the given @a method in an asynchronous way
 *
 * @param method The (non-static) method to call
 * @param object The object targetted by the @a method
 * @return The Task object associated to that call
 */
#define AwlAsyncMethod(method, object) awl::AsyncCall(boost::bind(method, object))

/** @brief Start a block that is to be executed in an asynchronous way.
 * You have to close the block with AwlCloseAsyncBlock
 *
 * @code
 * AwlAsyncBlock
 * {
 *    your parallelized code here
 * }
 * AwlCloseAsyncBlock;
 * @endcode
 */
#define AwlAsyncBlock { struct __awl_local_struct { static void __awl_async_block(void)

/** @brief See AwlAsyncBlock
 * @see AwlAsyncBlock
 */
#define AwlCloseAsyncBlock }; awl::AsyncCall(boost::bind(__awl_local_struct::__awl_async_block)); }

#define AwlCloseAsyncControlledBlock(taskRef) }; taskRef = awl::AsyncCall(boost::bind(__awl_local_struct::__awl_async_block)); }

namespace awl {
	
	/** @brief Call the given callback in an asynchronous way
	 * and get a handle on this task
	 *
	 * @param f the function or method that represents the task
	 * @return The associated Task object
	 */
	TaskRef Awl_Api AsyncCall(Callback f);
	
} // namespace awl

#endif
