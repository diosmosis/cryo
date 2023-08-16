///////////////////////////////////////////////////////////////////////////////
//
//! \file create_db.hpp
//! Contains the \ref create_db operation.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_OPERATION_CREATE_DB_HPP )
#define CRYO_OPERATION_CREATE_DB_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/relational/execute_command.hpp>
#include <cryo/sql/create_table_sql.hpp>
#include <cryo/extension/post_create_db_hook.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/type_traits/add_pointer.hpp>

namespace cryo
{
    namespace detail
    {
        template <typename Connection>
        struct create_table
        {
            create_table(Connection const& conn_)
                : conn(conn_)
            {}

            template <typename E>
            void operator()(E *) const
            {
                execute_command(conn, sql::create_table_sql<Connection, E>());
            }

            Connection const& conn;
        };
    }

    //! \brief Sets up a database by creating tables for each entity that is supposed to exist
    //!        in the database.
    //!
    //! For every type in \c Schema, this function creates and executes a <tt>CREATE TABLE</tt>
    //! statement. The columns of each table are determined by the members of each type and the
    //! field tags used to decorate those members.
    //!
    //! \remark This operation is performed within a transaction, so if an error occurs, all changes will be
    //!         rolled back.
    //! \tparam Schema a Boost.MPL Random Access Sequence that lists every entity that should
    //!                exist in the database.
    //! \param conn the backend connection. Models \ref RelationalDatabaseConnection.
    //! \throws error if a table already exists in the db or if \c conn is inactive.
    template <typename Schema, typename Connection>
    inline void create_db(Connection const& conn)
    {
        typedef typename boost::mpl::transform<
            Schema, boost::add_pointer<boost::mpl::_1>
        >::type pointer_vector;

        typename Connection::transaction_scope_type txn(conn);

        boost::fusion::for_each(pointer_vector(), detail::create_table<Connection>(conn));

        // do post create db work
        extension::post_create_db_hook<Connection, Schema>()(conn);

        // commit the changes
        txn.commit();
    }
}

#endif // #if !defined( CRYO_OPERATION_CREATE_DB_HPP )
