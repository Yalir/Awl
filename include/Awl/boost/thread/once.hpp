#ifndef BOOST_THREAD_ONCE_HPP
#define BOOST_THREAD_ONCE_HPP

//  once.hpp
//
//  (C) Copyright 2006-7 Anthony Williams 
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <Awl/boost/thread/detail/platform.hpp>
#if defined(BOOST_THREAD_PLATFORM_WIN32)
#include <Awl/boost/thread/win32/once.hpp>
#elif defined(BOOST_THREAD_PLATFORM_PTHREAD)
#include <Awl/boost/thread/pthread/once.hpp>
#else
#error "Boost threads unavailable on this platform"
#endif

#include <Awl/boost/config/abi_prefix.hpp>

namespace boost
{
    inline void call_once(void (*func)(),once_flag& flag)
    {
        call_once(flag,func);
    }
}

#include <Awl/boost/config/abi_suffix.hpp>

#endif
