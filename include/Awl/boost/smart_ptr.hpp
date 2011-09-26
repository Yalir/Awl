#ifndef BOOST_SMART_PTR_HPP_INCLUDED
#define BOOST_SMART_PTR_HPP_INCLUDED

//
//  smart_ptr.hpp
//
//  For convenience, this header includes the rest of the smart
//  pointer library headers.
//
//  Copyright (c) 2003 Peter Dimov  Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  http://www.boost.org/libs/smart_ptr/smart_ptr.htm
//

#include <Awl/boost/config.hpp>

#include <Awl/boost/scoped_ptr.hpp>
#include <Awl/boost/scoped_array.hpp>
#include <Awl/boost/shared_ptr.hpp>
#include <Awl/boost/shared_array.hpp>

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)
# include <Awl/boost/weak_ptr.hpp>
# include <Awl/boost/intrusive_ptr.hpp>
# include <Awl/boost/enable_shared_from_this.hpp>
# include <Awl/boost/make_shared.hpp>
#endif

#endif // #ifndef BOOST_SMART_PTR_HPP_INCLUDED
