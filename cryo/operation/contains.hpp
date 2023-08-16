///////////////////////////////////////////////////////////////////////////////
//
//! \file contains.hpp
//! Contains the \ref contains free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_OPERATION_CONTAINS_HPP )
#define CRYO_OPERATION_CONTAINS_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/primary_key_of.hpp>
#include <cryo/operation/count.hpp>

namespace cryo
{
    //! \brief Tests whether the database referenced by \conn contains an entity of type
    //!        \c Entity whose primary key is equal to \c key.
    //! \tparam Entity the type of entity to check.
    //! \param conn the backend connection.
    //! \param key a value convertible to the primary key type of \c Entity.
    //! \returns true if the database contains an entity of type \c Entity whose primary key
    //!          equals \c key, false if otherwise.
    template <typename Entity, typename Connection, typename PrimaryKey>
    inline bool contains(Connection const& conn, PrimaryKey const& key)
    {
        return count<Entity>(conn, key) != 0;
    }

    // TODO: Need a LocalEntity concept or EntityCopy concept.
    //! \brief Tests whether the database referenced by \conn contains an entity of type
    //!        \c Entity whose primary key is equal to <tt>primary_key_of(entity)</tt>.
    //! \tparam Entity the type of entity to check.
    //! \param conn the backend connection.
    //! \param entity contains the primary key value to check for.
    //! \returns true if the database contains an entity of type \c Entity whose primary key
    //!          equals <tt>primary_key_of(entity)</tt>, false if otherwise.
    template <typename Entity, typename Connection>
    inline bool contains(Connection const& conn, Entity const& entity)
    {
        return contains<Entity>(conn, primary_key_of(entity));
    }
}

#endif // #if !defined( CRYO_OPERATION_CONTAINS_HPP )
