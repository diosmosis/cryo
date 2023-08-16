///////////////////////////////////////////////////////////////////////////////
//
//! \file delete.hpp
//! Contains the \ref delete_ operation.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_OPERATION_DELETE_HPP )
#define CRYO_OPERATION_DELETE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/relational/execute_command.hpp>
#include <cryo/schema/introspect/primary_key_of.hpp>
#include <cryo/sql/create_delete_sql.hpp>

namespace cryo
{
    namespace detail
    {
        template <typename Connection>
        inline void post_delete_check(Connection const& conn)
        {
            // check that the entity was updated
            if (conn.affected_row_count() != 1)
            {
                if (conn.affected_row_count() == 0)
                {
                    throw error("the entity was not found when attempting to delete it");
                }
                else
                {
                    throw error("more than one entity was deleted");
                }
            }
        }
    }

    // TODO: Need to allow queries in delete & update statements.
    //! \brief Removes the row in the \c Entity table whose primary key is equal to \c key.
    //! \tparam Entity the type of entity to delete.
    //! \param conn the backend connection. Models \c RelationalDatabaseConnection.
    //! \param key a value convertible to \c Entity's primary key type.
    //! \throws error if \c conn is inactive, no entity was found with a primary key equal to \c key
    //!               or if more than one entity was deleted.
    template <typename Entity, typename Connection, typename PrimaryKey>
    inline void delete_(Connection & conn, PrimaryKey const& key)
    {
        execute_command(conn, sql::create_delete_sql<Connection, Entity>(key));

        detail::post_delete_check(conn);
    }

    //! \brief Removes the row in the \c Entity table whose primary key is equal to
    //!        <tt>primary_key_of(entity)</tt>.
    //! \param Connection the backend connection. Models \c RelationalDatabaseConnection.
    //! \param entity the entity that should be deleted.
    template <typename Connection, typename Entity>
    inline void delete_(Connection & conn, Entity const& entity)
    {
        delete_<Entity>(conn, primary_key_of(entity));
    }
}

#endif // #if !defined( CRYO_OPERATION_DELETE_HPP )
