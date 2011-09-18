
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
 * @details You're given access to the parent Task object through the @a self pointer
 *
 * @param function The function or static method to call
 * with the following signature: void function(awl::Task *self)
 * @return The Task object associated to that call
 */
#define AwlAsyncCall(function) awl::AsyncCall(boost::bind(function, _1))

/** @brief Call the given @a method in an asynchronous way
 * @details You're given access to the parent Task object through the @a self pointer
 *
 * @param method The (non-static) method to call
 * with the following signature: void class::method(awl::Task *self)
 * @param object The object targetted by the @a method
 * @return The Task object associated to that call
 */
#define AwlAsyncMethod(method, object) awl::AsyncCall(boost::bind(method, object, _1))

/** @brief Start a block that is to be executed in an asynchronous way.
 *
 * @details You're given access to the parent Task object through the @a self pointer
 * @code
 * AwlAsyncBlock
 * ({
 *		while (!self->IsCancelled())
 *		{
 *			your parallelized code here
 *		}
 * })
 * @endcode
 */
#define AwlAsyncBlock(functionBlock) \
{ struct __awl_local_struct { static void __awl_async_block(awl::Task *self) { functionBlock \
} }; AwlAsyncCall(__awl_local_struct::__awl_async_block); } 

/** @brief Start a block that is to be executed in an asynchronous way, and gives
 * back a Task object to externally control the Task.
 *
 * @details You're given access to the parent Task object through the @a self pointer
 *
 * @code
 * TaskRef myTask;
 * AwlAsyncManagedBlock
 * (myTask,
 * {
 *		while (!self->IsCancelled())
 *		{
 *			your parallelized code here
 *		}
 * })
 *
 * if (shoudl_cancel)
 *     myTask->Cancel();
 * @endcode
 */
#define AwlAsyncManagedBlock(taskRef, functionBlock)\
{ struct __awl_local_struct { static void __awl_async_block(awl::Task *self) { functionBlock \
} }; taskRef = AwlAsyncCall(__awl_local_struct::__awl_async_block); }

namespace awl {
	
	/** @brief Call the given callback in an asynchronous way
	 * and get a handle on this task
	 *
	 * @param f the function or method that represents the task
	 * with the following signature: void function(awl::Task *self)
	 * @return The associated Task object
	 */
	TaskRef Awl_Api AsyncCall(Callback f);
	
} // namespace awl

#endif
