///////////////////////////////////////////////////////////////////////////////
//
//! \file table_name_of.hpp
//! Contains the \ref table_name_of free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_TABLE_NAME_OF_HPP )
#define CRYO_SCHEMA_INTROSPECT_TABLE_NAME_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension/table_name_hook.hpp>

namespace cryo
{
    //! \brief Returns the name of an entity's associated database table.
    //! \tparam Entity the entity type.
    //! \returns the name of the table associated with \c Entity.
    template <typename Entity>
    inline char const* table_name_of()
    {
        return extension::table_name_hook<Entity>()();
    }

    template <typename Entity, typename Field>
    inline char const* table_name_of(Field Entity::*)
    {
        return table_name_of<Entity>();
    }    
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_TABLE_NAME_OF_HPP )
