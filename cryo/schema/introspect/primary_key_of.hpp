///////////////////////////////////////////////////////////////////////////////
//
//! \file primary_key_of.hpp
//! Contains the \ref primary_key_of<> free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_PRIMARY_KEY_OF_HPP )
#define CRYO_SCHEMA_INTROSPECT_PRIMARY_KEY_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/entity_column.hpp>

namespace cryo
{
    namespace result_of
    {
        template <typename T>
        struct primary_key_of
        {
            typedef typename entity_column<T, 0>::field_type type;
        };
    }

    //! \brief Gets the primary key of the supplied entity.
    //! \param entity the entity whose primary key is being accessed.
    //! \returns the value held by the primary key field of \c entity.
    template <typename Entity>
    inline typename result_of::primary_key_of<Entity>::type & primary_key_of(Entity & entity)
    {
        return entity.*entity_column<Entity, 0>().member();
    }

    template <typename Entity>
    inline typename result_of::primary_key_of<Entity>::type const& primary_key_of(Entity const& entity)
    {
        return entity.*entity_column<Entity, 0>().member();
    }
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_PRIMARY_KEY_OF_HPP )
