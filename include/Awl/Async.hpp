
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
 *			// your parallelized code here
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
 *			// your parallelized code here
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

/** @brief Start a block that is to be executed in an asynchronous way
 * with 1 input parameter.
 *
 * @details You're given access to the parent Task object through the @a self pointer.
 * The input parameter is redeclared in the asynchronous block
 * as a reference to the original variable. Note that you can only
 * give variables or pointers to variables as input parameter
 * (no raw array nor constant allowed).
 * The other AwlAsyncBlock_* variants allow the use of up to 10 parameters.
 *
 * @code
 * Type variable;
 * AwlAsyncBlock_1
 * (Type, variable,
 * {
 *		while (!self->IsCancelled())
 *		{
 *			// your parallelized code here
 *			// we can work on 'variable' here
 *		}
 * })
 * @endcode
 */
#define AwlAsyncBlock_1(type1, var1, functionBlock) \
{ \
struct __awl_local_struct \
{ \
static void __awl_async_block(awl::Task *self) \
{ \
type1 var1 = *(type1 *)self->input[#var1];\
functionBlock \
} \
}; \
awl::Callback __awl_f(boost::bind(__awl_local_struct::__awl_async_block, _1)); \
awl::TaskRef __awl_t(new awl::Task(__awl_f)); \
__awl_t->input[#var1] = (void *)&var1;\
awl::ThreadPool::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Start a block that is to be executed in an asynchronous way
 * with 2 input parameters.
 *
 * @see AwlAsyncBlock_1
 */
#define AwlAsyncBlock_2(type1, var1, type2, var2, functionBlock) \
{ \
struct __awl_local_struct \
{ \
static void __awl_async_block(awl::Task *self) \
{ \
type1 & var1 = *(type1 *)self->input[#var1];\
type2 & var2 = *(type2 *)self->input[#var2];\
functionBlock \
} \
}; \
awl::Callback __awl_f(boost::bind(__awl_local_struct::__awl_async_block, _1)); \
awl::TaskRef __awl_t(new awl::Task(__awl_f)); \
__awl_t->input[#var1] = (void *)&var1;\
__awl_t->input[#var2] = (void *)&var2;\
awl::ThreadPool::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Start a block that is to be executed in an asynchronous way
 * with 3 input parameters.
 *
 * @see AwlAsyncBlock_1
 */
#define AwlAsyncBlock_3(type1, var1, type2, var2, type3, var3, functionBlock) \
{ \
struct __awl_local_struct \
{ \
static void __awl_async_block(awl::Task *self) \
{ \
type1 & var1 = *(type1 *)self->input[#var1];\
type2 & var2 = *(type2 *)self->input[#var2];\
type3 & var3 = *(type3 *)self->input[#var3];\
functionBlock \
} \
}; \
awl::Callback __awl_f(boost::bind(__awl_local_struct::__awl_async_block, _1)); \
awl::TaskRef __awl_t(new awl::Task(__awl_f)); \
__awl_t->input[#var1] = (void *)&var1;\
__awl_t->input[#var2] = (void *)&var2;\
__awl_t->input[#var3] = (void *)&var3;\
awl::ThreadPool::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Start a block that is to be executed in an asynchronous way
 * with 4 input parameters.
 *
 * @see AwlAsyncBlock_1
 */
#define AwlAsyncBlock_4(type1, var1, type2, var2, type3, var3, type4, var4, functionBlock) \
{ \
struct __awl_local_struct \
{ \
static void __awl_async_block(awl::Task *self) \
{ \
type1 & var1 = *(type1 *)self->input[#var1];\
type2 & var2 = *(type2 *)self->input[#var2];\
type3 & var3 = *(type3 *)self->input[#var3];\
type4 & var4 = *(type4 *)self->input[#var4];\
functionBlock \
} \
}; \
awl::Callback __awl_f(boost::bind(__awl_local_struct::__awl_async_block, _1)); \
awl::TaskRef __awl_t(new awl::Task(__awl_f)); \
__awl_t->input[#var1] = (void *)&var1;\
__awl_t->input[#var2] = (void *)&var2;\
__awl_t->input[#var3] = (void *)&var3;\
__awl_t->input[#var4] = (void *)&var4;\
awl::ThreadPool::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Start a block that is to be executed in an asynchronous way
 * with 5 input parameters.
 *
 * @see AwlAsyncBlock_1
 */
#define AwlAsyncBlock_5(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, functionBlock) \
{ \
struct __awl_local_struct \
{ \
static void __awl_async_block(awl::Task *self) \
{ \
type1 & var1 = *(type1 *)self->input[#var1];\
type2 & var2 = *(type2 *)self->input[#var2];\
type3 & var3 = *(type3 *)self->input[#var3];\
type4 & var4 = *(type4 *)self->input[#var4];\
type5 & var5 = *(type5 *)self->input[#var5];\
functionBlock \
} \
}; \
awl::Callback __awl_f(boost::bind(__awl_local_struct::__awl_async_block, _1)); \
awl::TaskRef __awl_t(new awl::Task(__awl_f)); \
__awl_t->input[#var1] = (void *)&var1;\
__awl_t->input[#var2] = (void *)&var2;\
__awl_t->input[#var3] = (void *)&var3;\
__awl_t->input[#var4] = (void *)&var4;\
__awl_t->input[#var5] = (void *)&var5;\
awl::ThreadPool::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Start a block that is to be executed in an asynchronous way
 * with 6 input parameters.
 *
 * @see AwlAsyncBlock_1
 */
#define AwlAsyncBlock_6(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, functionBlock) \
{ \
struct __awl_local_struct \
{ \
static void __awl_async_block(awl::Task *self) \
{ \
type1 & var1 = *(type1 *)self->input[#var1];\
type2 & var2 = *(type2 *)self->input[#var2];\
type3 & var3 = *(type3 *)self->input[#var3];\
type4 & var4 = *(type4 *)self->input[#var4];\
type5 & var5 = *(type5 *)self->input[#var5];\
type6 & var6 = *(type6 *)self->input[#var6];\
functionBlock \
} \
}; \
awl::Callback __awl_f(boost::bind(__awl_local_struct::__awl_async_block, _1)); \
awl::TaskRef __awl_t(new awl::Task(__awl_f)); \
__awl_t->input[#var1] = (void *)&var1;\
__awl_t->input[#var2] = (void *)&var2;\
__awl_t->input[#var3] = (void *)&var3;\
__awl_t->input[#var4] = (void *)&var4;\
__awl_t->input[#var5] = (void *)&var5;\
__awl_t->input[#var6] = (void *)&var6;\
awl::ThreadPool::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Start a block that is to be executed in an asynchronous way
 * with 7 input parameters.
 *
 * @see AwlAsyncBlock_1
 */
#define AwlAsyncBlock_7(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, functionBlock) \
{ \
struct __awl_local_struct \
{ \
static void __awl_async_block(awl::Task *self) \
{ \
type1 & var1 = *(type1 *)self->input[#var1];\
type2 & var2 = *(type2 *)self->input[#var2];\
type3 & var3 = *(type3 *)self->input[#var3];\
type4 & var4 = *(type4 *)self->input[#var4];\
type5 & var5 = *(type5 *)self->input[#var5];\
type6 & var6 = *(type6 *)self->input[#var6];\
type7 & var7 = *(type7 *)self->input[#var7];\
functionBlock \
} \
}; \
awl::Callback __awl_f(boost::bind(__awl_local_struct::__awl_async_block, _1)); \
awl::TaskRef __awl_t(new awl::Task(__awl_f)); \
__awl_t->input[#var1] = (void *)&var1;\
__awl_t->input[#var2] = (void *)&var2;\
__awl_t->input[#var3] = (void *)&var3;\
__awl_t->input[#var4] = (void *)&var4;\
__awl_t->input[#var5] = (void *)&var5;\
__awl_t->input[#var6] = (void *)&var6;\
__awl_t->input[#var7] = (void *)&var7;\
awl::ThreadPool::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Start a block that is to be executed in an asynchronous way
 * with 8 input parameters.
 *
 * @see AwlAsyncBlock_1
 */
#define AwlAsyncBlock_8(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, type8, var8, functionBlock) \
{ \
struct __awl_local_struct \
{ \
static void __awl_async_block(awl::Task *self) \
{ \
type1 & var1 = *(type1 *)self->input[#var1];\
type2 & var2 = *(type2 *)self->input[#var2];\
type3 & var3 = *(type3 *)self->input[#var3];\
type4 & var4 = *(type4 *)self->input[#var4];\
type5 & var5 = *(type5 *)self->input[#var5];\
type6 & var6 = *(type6 *)self->input[#var6];\
type7 & var7 = *(type7 *)self->input[#var7];\
type8 & var8 = *(type8 *)self->input[#var8];\
functionBlock \
} \
}; \
awl::Callback __awl_f(boost::bind(__awl_local_struct::__awl_async_block, _1)); \
awl::TaskRef __awl_t(new awl::Task(__awl_f)); \
__awl_t->input[#var1] = (void *)&var1;\
__awl_t->input[#var2] = (void *)&var2;\
__awl_t->input[#var3] = (void *)&var3;\
__awl_t->input[#var4] = (void *)&var4;\
__awl_t->input[#var5] = (void *)&var5;\
__awl_t->input[#var6] = (void *)&var6;\
__awl_t->input[#var7] = (void *)&var7;\
__awl_t->input[#var8] = (void *)&var8;\
awl::ThreadPool::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Start a block that is to be executed in an asynchronous way
 * with 9 input parameters.
 *
 * @see AwlAsyncBlock_1
 */
#define AwlAsyncBlock_9(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, type8, var8, type9, var9, functionBlock) \
{ \
struct __awl_local_struct \
{ \
static void __awl_async_block(awl::Task *self) \
{ \
type1 & var1 = *(type1 *)self->input[#var1];\
type2 & var2 = *(type2 *)self->input[#var2];\
type3 & var3 = *(type3 *)self->input[#var3];\
type4 & var4 = *(type4 *)self->input[#var4];\
type5 & var5 = *(type5 *)self->input[#var5];\
type6 & var6 = *(type6 *)self->input[#var6];\
type7 & var7 = *(type7 *)self->input[#var7];\
type8 & var8 = *(type8 *)self->input[#var8];\
type9 & var9 = *(type9 *)self->input[#var9];\
functionBlock \
} \
}; \
awl::Callback __awl_f(boost::bind(__awl_local_struct::__awl_async_block, _1)); \
awl::TaskRef __awl_t(new awl::Task(__awl_f)); \
__awl_t->input[#var1] = (void *)&var1;\
__awl_t->input[#var2] = (void *)&var2;\
__awl_t->input[#var3] = (void *)&var3;\
__awl_t->input[#var4] = (void *)&var4;\
__awl_t->input[#var5] = (void *)&var5;\
__awl_t->input[#var6] = (void *)&var6;\
__awl_t->input[#var7] = (void *)&var7;\
__awl_t->input[#var8] = (void *)&var8;\
__awl_t->input[#var9] = (void *)&var9;\
awl::ThreadPool::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Start a block that is to be executed in an asynchronous way
 * with 10 input parameters.
 *
 * @see AwlAsyncBlock_1
 */
#define AwlAsyncBlock_10(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, type8, var8, type9, var9, type10, var10, functionBlock) \
{ \
struct __awl_local_struct \
{ \
static void __awl_async_block(awl::Task *self) \
{ \
type1 & var1 = *(type1 *)self->input[#var1];\
type2 & var2 = *(type2 *)self->input[#var2];\
type3 & var3 = *(type3 *)self->input[#var3];\
type4 & var4 = *(type4 *)self->input[#var4];\
type5 & var5 = *(type5 *)self->input[#var5];\
type6 & var6 = *(type6 *)self->input[#var6];\
type7 & var7 = *(type7 *)self->input[#var7];\
type8 & var8 = *(type8 *)self->input[#var8];\
type9 & var9 = *(type9 *)self->input[#var9];\
type10 & var10 = *(type10 *)self->input[#var10];\
functionBlock \
} \
}; \
awl::Callback __awl_f(boost::bind(__awl_local_struct::__awl_async_block, _1)); \
awl::TaskRef __awl_t(new awl::Task(__awl_f)); \
__awl_t->input[#var1] = (void *)&var1;\
__awl_t->input[#var2] = (void *)&var2;\
__awl_t->input[#var3] = (void *)&var3;\
__awl_t->input[#var4] = (void *)&var4;\
__awl_t->input[#var5] = (void *)&var5;\
__awl_t->input[#var6] = (void *)&var6;\
__awl_t->input[#var7] = (void *)&var7;\
__awl_t->input[#var8] = (void *)&var8;\
__awl_t->input[#var9] = (void *)&var9;\
__awl_t->input[#var10] = (void *)&var10;\
awl::ThreadPool::Default().ScheduleTaskForExecution(__awl_t); \
}


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
