///////////////////////////////////////////////////////////////////////////////
//
//! \file column_name_of.hpp
//! Contains the \ref column_name_of free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_COLUMN_NAME_OF_HPP )
#define CRYO_SCHEMA_INTROSPECT_COLUMN_NAME_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension/column_name_hook.hpp>
#include <cryo/schema/introspect/column_switch.hpp>
#include <cryo/schema/entity_column.hpp>

namespace cryo
{
    namespace detail
    {
        template <typename Entity>
        struct column_name_of_impl
        {
            typedef char const* result_type;

            template <typename N>
            result_type operator()(N) const
            {
                return extension::column_name_hook<entity_column<Entity, N::value> >()();
            }
        };
    }

    template <typename T, typename Entity>
    inline char const* column_name_of(T Entity::* member)
    {
        return column_switch(member, detail::column_name_of_impl<Entity>());
    }

    //! \brief Gets the name of a column as its named in the database.
    //! \tparam C a \ref ColumnDescriptor describing the column whose name is to be retrieved.
    //! \returns the name of the column described by \c C.
    template <typename C>
    inline char const* column_name_of()
    {
        return extension::column_name_hook<C>()();
    }

    //! \brief Gets the name of a column as its named in the database.
    //! \param a \ref PartialColumnDescriptor describing the column whose name is to be retrieved.
    //! \returns the name of the column described by \c C.
    template <typename C>
    inline char const* column_name_of(C)
    {
        return extension::column_name_hook<C>()();
    }
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_COLUMN_NAME_OF_HPP )
