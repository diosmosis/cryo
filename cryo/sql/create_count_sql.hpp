///////////////////////////////////////////////////////////////////////////////
//
//! \file create_count_sql.hpp
//! Contains the \ref create_count_sql free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_CREATE_COUNT_SQL_HPP )
#define CRYO_SQL_CREATE_COUNT_SQL_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/output_column_value.hpp>
#include <cryo/sql/output_table_name.hpp>
#include <cryo/sql/output_column_name.hpp>
#include <cryo/extension/concrete_hooks.hpp>
#include <cryo/sql/dynamic/condition.hpp>
#include <cryo/schema/introspect/primary_key_column_of.hpp>
#include <cryo/schema/introspect/table_name_of.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    //! \brief Creates a \c SELECT SQL statement that counts every stored entity.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \tparam Entity the entity type to count.
    //! \returns a \c SELECT statement that counts every \c Entity entity.
    template <typename Connection, typename Entity>
    inline std::string create_count_sql()
    {
        std::stringstream ss;
        ss << "SELECT COUNT(*) FROM ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << ';';
        return ss.str();
    }

    //! \brief Creates a \c SELECT SQL statement that counts every entity for whom \c q
    //!        evaluates to \c true.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \tparam Entity the entity type to count.
    //! \param q the condition that determines which rows will be counted.
    //! \returns a \c SELECT statement that counts every \c Entity entity for whom \c q
    //!          evaluates to \c true.
    template <typename Connection, typename Entity>
    inline std::string create_count_sql(dynamic::condition const& q)
    {
        if (!q)
        {
            return create_count_sql<Connection, Entity>();
        }

        extension::concrete_hooks<Connection> hooks;

        // calculate join indices
        join_index_set indices;
        q.calc_join_indices(indices);

        std::stringstream ss;
        ss << "SELECT COUNT(*) FROM ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << " t0";

        // output left joins
        for (join_index_set::const_iterator i = indices.begin(); i != indices.end(); ++i)
        {
            i->output_join(ss, hooks);
        }

        ss << " WHERE ";
        q.output_sql(ss, hooks);

        ss << ';';
        return ss.str();
    }

    //! \brief Creates a \c SELECT SQL statement that counts every entity whose primary
    //!        key is equal to \c x.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \tparam Entity the entity type to count.
    //! \param x a value convertible to \c Entity's primary key type.
    //! \returns a \c SELECT statement that counts every \c Entity whose primary key
    //!          equals \c x.
    template <typename Connection, typename Entity, typename PrimaryKey>
    inline std::string create_count_sql(PrimaryKey const& x)
    {
        std::stringstream ss;
        ss << "SELECT COUNT(*) FROM ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << " WHERE ";
        output_column_name<Connection>(ss, primary_key_column_of<Entity>());
        ss << '=';
        output_column_value<Connection>(ss, x);
        ss << ';';
        return ss.str();
    }
}}

#endif // #if !defined( CRYO_SQLITE_CREATE_COUNT_SQL_HPP )
