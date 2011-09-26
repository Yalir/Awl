
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
 * @details You're given access to the parent Task object through the @a self pointer
 *
 * @param function The function or static method to call
 * @return The Task object associated to that call
 */
#define AwlMainThreadCall(function) awl::MainThreadCall(boost::bind(function, _1))

/** @brief Call the given @a method on the main thread
 * @details You're given access to the parent Task object through the @a self pointer
 *
 * @param method The (non-static) method to call
 * @param object The object targetted by the @a method
 * @return The Task object associated to that call
 */
#define AwlMainThreadMethod(method, object) awl::MainThreadCall(boost::bind(method, object, _1))

/** @brief Creates a block that is to be executed asynchronously on the main thread.
 * @details After the end of this block, the inner code is not guaranted to have been
 * performed yet. You're given access to the parent Task object through the @a self pointer
 *
 * @code
 * AwlAsyncMainThreadBlock
 * ({
 *		while (!self->IsCancelled())
 *		{
 *			// your code to execute on the main thread here
 *		}
 * })
 * // previous code may still not be executed but we don't need it to go on
 * @endcode
 */
#define AwlAsyncMainThreadBlock(functionBlock) \
{ struct __awl_local_struct { static void __awl_async_block(awl::Task *self) { functionBlock \
} }; AwlMainThreadCall(__awl_local_struct::__awl_async_block); } 

/** @brief Creates a block that is to be executed asynchronously on the main thread
 * with 1 input parameter.
 *
 * @details After the end of this block, the inner code is not guaranted to have been
 * performed yet.
 * You're given access to the parent Task object through the @a self pointer.
 * The input parameter is redeclared in the asynchronous block
 * as a reference to the original variable. Note that you can only
 * give variables or pointers to variables as input parameter
 * (no raw array nor constant allowed).
 * The other AwlAsyncMainThreadBlock_* variants allow the use of up to 10 parameters.
 *
 * @code
 * Type variable;
 * AwlAsyncMainThreadBlock_1
 * (Type, variable,
 * {
 *		while (!self->IsCancelled())
 *		{
 *			// your code to execute on the main thread here
 *			// we can work on 'variable' here
 *		}
 * })
 * // previous code may still not be executed but we don't need it to go on
 * @endcode
 */
#define AwlAsyncMainThreadBlock_1(type1, var1, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Creates a block that is to be executed asynchronously on the main thread
 * with 2 input parameters.
 *
 * @see AwlAsyncMainThreadBlock_1
 */
#define AwlAsyncMainThreadBlock_2(type1, var1, type2, var2, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Creates a block that is to be executed asynchronously on the main thread
 * with 3 input parameters.
 *
 * @see AwlAsyncMainThreadBlock_1
 */
#define AwlAsyncMainThreadBlock_3(type1, var1, type2, var2, type3, var3, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Creates a block that is to be executed asynchronously on the main thread
 * with 4 input parameters.
 *
 * @see AwlAsyncMainThreadBlock_1
 */
#define AwlAsyncMainThreadBlock_4(type1, var1, type2, var2, type3, var3, type4, var4, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Creates a block that is to be executed asynchronously on the main thread
 * with 5 input parameters.
 *
 * @see AwlAsyncMainThreadBlock_1
 */
#define AwlAsyncMainThreadBlock_5(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Creates a block that is to be executed asynchronously on the main thread
 * with 6 input parameters.
 *
 * @see AwlAsyncMainThreadBlock_1
 */
#define AwlAsyncMainThreadBlock_6(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Creates a block that is to be executed asynchronously on the main thread
 * with 7 input parameters.
 *
 * @see AwlAsyncMainThreadBlock_1
 */
#define AwlAsyncMainThreadBlock_7(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Creates a block that is to be executed asynchronously on the main thread
 * with 8 input parameters.
 *
 * @see AwlAsyncMainThreadBlock_1
 */
#define AwlAsyncMainThreadBlock_8(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, type8, var8, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Creates a block that is to be executed asynchronously on the main thread
 * with 9 input parameters.
 *
 * @see AwlAsyncMainThreadBlock_1
 */
#define AwlAsyncMainThreadBlock_9(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, type8, var8, type9, var9, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Creates a block that is to be executed asynchronously on the main thread
 * with 10 input parameters.
 *
 * @see AwlAsyncMainThreadBlock_1
 */
#define AwlAsyncMainThreadBlock_10(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, type8, var8, type9, var9, type10, var10, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
}

/** @brief Creates a block that is to be executed on the main thread.
 *
 * @details After the end of this block, the inner code is guaranted to have been
 * performed. As this block is synchronized with the main thread and waits for
 * its completion, it should be the shortest and fastest possible to avoid
 * important lock time. You're given access to the parent Task object through the @a self pointer.
 *
 * @code
 * AwlMainThreadBlock
 * ({
 *		// your code to execute on the main thread here
 * })
 * @endcode
 */
#define AwlMainThreadBlock(functionBlock) \
{ struct __awl_local_struct { static void __awl_async_block(awl::Task *self) { functionBlock \
} }; awl::TaskRef __awl_task = AwlMainThreadCall(__awl_local_struct::__awl_async_block); __awl_task->Wait(); } 

/** @brief Creates a block that is to be executed on the main thread
 * with 1 input parameter.
 *
 * @details After the end of this block, the inner code is guaranted to have been
 * performed. As this block is synchronized with the main thread and waits for
 * its completion, it should be the shortest and fastest possible to avoid
 * important lock time. You're given access to the parent Task object through the @a self pointer.
 * The input parameter is redeclared in the asynchronous block
 * as a reference to the original variable. Note that you can only
 * give variables or pointers to variables as input parameter
 * (no raw array nor constant allowed).
 * The other AwlAsyncMainThreadBlock_* variants allow the use of up to 10 parameters.
 *
 * @code
 * Type variable;
 * AwlMainThreadBlock_1
 * (Type, variable,
 * {
 *		// your code to execute on the main thread here
 *		// we can work on 'variable' here
 * })
 * @endcode
 */
#define AwlMainThreadBlock_1(type1, var1, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
__awl_t->Wait();\
}

/** @brief Creates a block that is to be executed on the main thread
 * with 2 input parameters.
 *
 * @see AwlMainThreadBlock_1
 */
#define AwlMainThreadBlock_2(type1, var1, type2, var2, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
__awl_t->Wait();\
}

/** @brief Creates a block that is to be executed on the main thread
 * with 3 input parameters.
 *
 * @see AwlMainThreadBlock_1
 */
#define AwlMainThreadBlock_3(type1, var1, type2, var2, type3, var3, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
__awl_t->Wait();\
}

/** @brief Creates a block that is to be executed on the main thread
 * with 4 input parameters.
 *
 * @see AwlMainThreadBlock_1
 */
#define AwlMainThreadBlock_4(type1, var1, type2, var2, type3, var3, type4, var4, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
__awl_t->Wait();\
}

/** @brief Creates a block that is to be executed on the main thread
 * with 5 input parameters.
 *
 * @see AwlMainThreadBlock_1
 */
#define AwlMainThreadBlock_5(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
__awl_t->Wait();\
}

/** @brief Creates a block that is to be executed on the main thread
 * with 6 input parameters.
 *
 * @see AwlMainThreadBlock_1
 */
#define AwlMainThreadBlock_6(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
__awl_t->Wait();\
}

/** @brief Creates a block that is to be executed on the main thread
 * with 7 input parameters.
 *
 * @see AwlMainThreadBlock_1
 */
#define AwlMainThreadBlock_7(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
__awl_t->Wait();\
}

/** @brief Creates a block that is to be executed on the main thread
 * with 8 input parameters.
 *
 * @see AwlMainThreadBlock_1
 */
#define AwlMainThreadBlock_8(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, type8, var8, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
__awl_t->Wait();\
}

/** @brief Creates a block that is to be executed on the main thread
 * with 9 input parameters.
 *
 * @see AwlMainThreadBlock_1
 */
#define AwlMainThreadBlock_9(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, type8, var8, type9, var9, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
__awl_t->Wait();\
}

/** @brief Creates a block that is to be executed on the main thread
 * with 10 input parameters.
 *
 * @see AwlMainThreadBlock_1
 */
#define AwlMainThreadBlock_10(type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6, type7, var7, type8, var8, type9, var9, type10, var10, functionBlock) \
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
awl::WorkLoop::Default().ScheduleTaskForExecution(__awl_t); \
__awl_t->Wait();\
}

namespace awl {
	
	/** @brief Call the given callback on the main thread
	 * and get a handle on this task
	 *
	 * @param f the function or method that represents the task
	 * @return The associated Task object
	 */
	TaskRef Awl_Api MainThreadCall(Callback f);
	
} // namespace awl

#endif

