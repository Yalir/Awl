
/*
 *  Err.hpp (imported from SFML)
 *  Awl - Asynchronous Work Library
 *
 *  Copyright (c) 2011 Lucas Soltic
 *  ceylow@gmail.com
 *
 *  SFML - Simple and Fast Multimedia Library
 *  Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef Awl_Err_hpp
#define Awl_Err_hpp

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <Awl/Config.hpp>
#include <ostream>


namespace awl
{
////////////////////////////////////////////////////////////
/// \brief Standard stream used by SFML to output warnings and errors
///
////////////////////////////////////////////////////////////
Awl_Api std::ostream& Err();

} // namespace sf


#endif // Awl_Err_hpp


////////////////////////////////////////////////////////////
/// \fn awl::Err
/// \ingroup system
///
/// By default, awl::Err() outputs to the same location as std::cerr,
/// (-> the stderr descriptor) which is the console if there's
/// one available.
///
/// It is a standard std::ostream instance, so it supports all the
/// insertion operations defined by the STL
/// (operator <<, manipulators, etc.).
///
/// awl::Err() can be redirected to write to another output, independantly
/// of std::cerr, by using the rdbuf() function provided by the
/// std::ostream class.
///
/// Example:
/// \code
/// // Redirect to a file
/// std::ofstream file("sfml-log.txt");
/// std::streambuf* previous = awl::Err().rdbuf(file.rdbuf());
///
/// // Redirect to nothing
/// awl::Err().rdbuf(NULL);
///
/// // Restore the original output
/// awl::Err().rdbuf(previous);
/// \endcode
///
////////////////////////////////////////////////////////////
