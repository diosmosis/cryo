///////////////////////////////////////////////////////////////////////////////
//
//! \brief is_wrapper.hpp
//! Contains the \ref is_wrapper<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_TAG_IS_WRAPPER_HPP )
#define CRYO_SCHEMA_TAG_IS_WRAPPER_HPP

#include <cryo/config.hpp>
#include <boost/mpl/bool.hpp>

namespace cryo
{
    //! \brief A metafunction that should be specialized for types that wrap another
    //!        type.
    //! By default this type returns \c boost::mpl::false_.
    //! \tparam T the type to check. It does not matter if this type is \c const qualified
    //!         or not.
    template <typename T>
    struct is_wrapper
        : boost::mpl::false_
    {};

    template <typename T>
    struct is_wrapper<T const>
        : is_wrapper<T>
    {};
}

#endif // #if !defined( CRYO_SCHEMA_TAG_IS_WRAPPER_HPP )
