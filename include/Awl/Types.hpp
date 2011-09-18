
/*
 *  Types.hpp
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

#ifndef Awl_Types_hpp
#define Awl_Types_hpp

#include <Awl/boost/function.hpp>
#include <Awl/boost/noncopyable.hpp>

namespace awl {
	class Task;
	
	/** Defines a simple callback to a function defined as follow : void f(Task* self)
	 * where @a self is the parent Task
	 */
	typedef boost::function <void (Task*)> Callback;

} // namespace awl

#endif // Awl_Types_hpp
