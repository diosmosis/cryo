///////////////////////////////////////////////////////////////////////////////
//
//! \file config.hpp
//! Defines macros used by cryo to vary library functionality, features and/or
//! implementation.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONFIG_HPP )
#define CRYO_CONFIG_HPP

#include <boost/config.hpp>

//! \brief The library's version major.
#define CRYO_VERSION_MAJOR 0

//! \brief The library's version minor.
#define CRYO_VERSION_MINOR 6

// define CRYO_DECL to the appropriate value if needed
#if defined( BOOST_HAS_DECLSPEC )
#   if defined( BOOST_ALL_DYN_LINK ) || defined( CRYO_DYN_LINK )
#       if defined( CRYO_SOURCE )
#           define CRYO_DECL __declspec(dllexport)
#       else
#           define CRYO_DECL __declspec(dllimport)
#       endif
#   endif
#endif // #if defined( BOOST_HAS_DECLSPEC )

#if !defined( CRYO_DECL )
#   define CRYO_DECL
#endif // #if !defined( CRYO_DECL )

#if !defined( CRYO_PARAMETER_LIMIT )
//! \brief The number of arguments variadic operations allow.
//!
//! This constant determines how many columns can be individually selected in a call to
//! \ref find or \ref find_all and how many columns can be individually updated in a call
//! to update.
#   define CRYO_PARAMETER_LIMIT 6
#endif // #if !defined( CRYO_PARAMETER_LIMIT )

#if !defined( CRYO_QUERY_FUNCTION_PARAM_LIMIT )
//! \brief The number of arguments allowed in an SQL function.
//!
//! If you plan on defining your own functions to use in cryo's query EDSL, make sure this
//! constant is greater than or equal to the arity of the functions you're defining.
#   define CRYO_QUERY_FUNCTION_PARAM_LIMIT 6
#endif // #if !defined( CRYO_QUERY_FUNCTION_PARAM_LIMIT )

#endif // #if !defined( CRYO_CONFIG_HPP )
