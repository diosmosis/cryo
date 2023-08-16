///////////////////////////////////////////////////////////////////////////////
//
//! \file entity_column.hpp
//! Contains the \ref entity_column<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_ENTITY_COLUMN_HPP )
#define CRYO_SCHEMA_ENTITY_COLUMN_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>

namespace cryo
{
    //! \brief A \ref ColumnDescriptor implementation.
    //!
    //! This type is specialized appropriately when the \ref CRYO_DECLARE_ENTITY macro
    //! is used.
    //!
    //! \tparam T the entity type whose column is being described.
    //! \tparam N the index of the column being described.
    template <typename T, int N>
    struct entity_column;

    template <typename T, int N>
    struct entity_column<T const, N>
        : entity_column<T, N>
    {};

    template <typename E, int N>
    struct field_type_of<entity_column<E, N> >
    {
        typedef typename entity_column<E, N>::field_type type;
    };
}

#endif // #if !defined( CRYO_SCHEMA_ENTITY_COLUMN_HPP )
