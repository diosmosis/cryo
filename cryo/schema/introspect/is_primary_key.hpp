///////////////////////////////////////////////////////////////////////////////
//
//! \brief is_primary_key.hpp
//! Contains the \ref is_primary_key<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_IS_PRIMARY_KEY_HPP )
#define CRYO_SCHEMA_INTROSPECT_IS_PRIMARY_KEY_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/mpl/bool.hpp>

namespace cryo
{
    //! \brief Metafunction predicate that tests whether a given \ref ColumnDescriptor
    //!        describes a primary key column or not.
    //! \tparam C a \ref ColumnDescriptor describing the column to check.
    template <typename C>
    struct is_primary_key
        : boost::mpl::bool_<C::index == 0>
    {};
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_IS_PRIMARY_KEY_HPP )
