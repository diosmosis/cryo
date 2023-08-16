///////////////////////////////////////////////////////////////////////////////
//
//! \file wrapped_type_of.hpp
//! Contains the \ref wrapped_type_of<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_WRAPPED_TYPE_OF_HPP )
#define CRYO_SCHEMA_INTROSPECT_WRAPPED_TYPE_OF_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo
{
    // TODO: Need a Wrapper concept?
    //! \brief Metafunction that returns the immediate wrapped type of a wrapper type.
    //! \tparam T the wrapper type whose wrapped type is being retrieved.
    template <typename T>
    struct wrapped_type_of;

    template <typename T>
    struct wrapped_type_of<T const>
    {
        typedef typename wrapped_type_of<T>::type const type;
    };
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_WRAPPED_TYPE_OF_HPP )
