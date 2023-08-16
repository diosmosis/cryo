///////////////////////////////////////////////////////////////////////////////
//
//! \file primary_key_column_of.hpp
//! Contains the \ref primary_key_column_of free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_PRIMARY_KEY_COLUMN_OF_HPP )
#define CRYO_SCHEMA_INTROSPECT_PRIMARY_KEY_COLUMN_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/entity_column.hpp>

namespace cryo
{
    namespace result_of
    {
        //! \brief Metafunction that returns a \ref ColumnDescriptor describing the
        //!        primary key column of an entity.
        //! \tparam Entity the entity whose primary key column is being accessed.
        template <typename Entity>
        struct primary_key_column_of
        {
            typedef typename entity_column<Entity, 0> type;
        };
    }

    //! \brief Returns a \ref ColumnDescriptor describing the primary key column of an
    //!        entity.
    //! \tparam Entity the entity whose primary key column is bein accessed.
    //! \returns a \ref ColumnDescriptor instance describing the primary key column of
    //!          \c Entity.
    template <typename Entity>
    inline typename result_of::primary_key_column_of<Entity>::type primary_key_column_of()
    {
        return typename result_of::primary_key_column_of<Entity>::type();
    }
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_PRIMARY_KEY_COLUMN_OF_HPP )
