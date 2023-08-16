///////////////////////////////////////////////////////////////////////////////
//
//! \file count.hpp
//! Contains the \ref count free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_OPERATION_COUNT_HPP )
#define CRYO_OPERATION_COUNT_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/relational/execute_scalar.hpp>
#include <cryo/sql/dynamic/condition.hpp>
#include <cryo/sql/create_count_sql.hpp>

namespace cryo
{
    // TODO: Need to get rid of this. No idea how. Used by contains().
    template <typename Entity, typename Connection, typename PrimaryKey>
    inline typename Connection::uint_type count(Connection const& conn, PrimaryKey const& key)
    {
        return execute_scalar(conn, sql::create_count_sql<Connection, Entity>(key));
    }

    //! \brief Counts the number of entities of type \c Entity for which the cryo query
    //!        condition \c q is true.
    //! \tparam Entity the entity type to search for.
    //! \param conn the backend connection.
    //! \param q a cryo query condition. If \c q is a null condition, this function will count
    //!          every entity of type \c Entity.
    //! \returns the number of entities of type \c Entity for which \c q is true.
    template <typename Entity, typename Connection>
    inline typename Connection::uint_type count(Connection const& conn, sql::dynamic::condition const& q)
    {
        return execute_scalar(conn, sql::create_count_sql<Connection, Entity>(q));
    }

    //! \brief Returns the number of entities of type \c Entity currently stored in the
    //!        database \c conn references.
    //! \tparam Entity the entity type to count.
    //! \param conn the backend connection.
    //! \returns the number of entities of type \c Entity stored in the database.
    template <typename Entity, typename Connection>
    inline typename Connection::uint_type count(Connection const& conn)
    {
        return count<Entity>(conn, sql::dynamic::condition());
    }
}

#endif // #if !defined( CRYO_OPERATION_COUNT_HPP )
