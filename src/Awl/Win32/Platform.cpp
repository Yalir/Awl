
/*
 *  Win32/Platform.cpp (imported from SFML)
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

#include <Awl/Win32/Platform.hpp>

namespace awl {
	namespace priv {
		
		////////////////////////////////////////////////////////////
		Uint64 Platform::GetSystemTime()
		{
			static LARGE_INTEGER frequency;
			static BOOL          useHighPerformanceTimer = QueryPerformanceFrequency(&frequency);
			
			if (useHighPerformanceTimer)
			{
				// High performance counter available : use it
				LARGE_INTEGER currentTime;
				QueryPerformanceCounter(&currentTime);
				
				return currentTime.QuadPart * 1000 / frequency.QuadPart;
			}
			else
			{
				// High performance counter not available: use GetTickCount (less accurate)
				return GetTickCount();
			}
		}
		
		
		////////////////////////////////////////////////////////////
		void Platform::Sleep(Uint32 time)
		{
			::Sleep(time);
		}
		
	} // namespace priv
	
} // namespace awl
