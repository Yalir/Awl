
/*
 *  Awl.hpp
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

#ifndef Awl_Awl_hpp
#define Awl_Awl_hpp

/** @namespace awl
 * @brief Contains all of the classes, functions, types and definitions of the Awl
 * library
 */

// General usecase files
#include <Awl/Config.hpp>
#include <Awl/Types.hpp>
#include <Awl/Debug.hpp>

// Thread-related classes
#include <Awl/Lock.hpp>
#include <Awl/Mutex.hpp>
#include <Awl/Condition.hpp>
#include <Awl/Thread.hpp>

// Real Awl interesting stuff
#include <Awl/Async.hpp>
#include <Awl/MainThread.hpp>
#include <Awl/Task.hpp>
#include <Awl/WorkLoop.hpp>

#endif
