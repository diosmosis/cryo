///////////////////////////////////////////////////////////////////////////////
//
//! \file is_column_descriptor.hpp
//! Contains the \ref is_column_descriptor<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_IS_COLUMN_DESCRIPTOR_HPP )
#define CRYO_SCHEMA_INTROSPECT_IS_COLUMN_DESCRIPTOR_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/entity_column.hpp>

namespace cryo
{
    // TODO: keyword::column & group_by should be made to model the column_descriptor conept
    //! \brief Metafunction that tells if a type models the \ref ColumnDescriptor concept.
    //!
    //! This type must be specialized explicitly.
    //!
    //! \tparam T the type to check.
    //! \returns \c boost::mpl::true_ if \c T is a \ref ColumnDescriptor, \false if otherwise.
    template <typename T>
    struct is_column_descriptor
        : boost::mpl::false_
    {};

    template <typename E, int N>
    struct is_column_descriptor<entity_column<E, N> >
        : boost::mpl::true_
    {};
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_IS_COLUMN_DESCRIPTOR_HPP )
