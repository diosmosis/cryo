///////////////////////////////////////////////////////////////////////////////
//
//! \file create_delete_sql.hpp
//! Contains the \ref create_delete_sql free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_CREATE_DELETE_SQL_HPP )
#define CRYO_SQL_CREATE_DELETE_SQL_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/output_table_name.hpp>
#include <cryo/sql/output_column_value.hpp>
#include <cryo/sql/output_table_name.hpp>
#include <cryo/schema/introspect/table_name_of.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    //! \brief Creates a \c DELETE SQL statement that deletes a single table row.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \tparam Entity the entity whose mapped table is to be deleted from.
    //! \param key a value convertible to \c Entity's primary key type that determines
    //!            which table row is to be deleted.
    //! \returns a \c DELETE SQL statement that deletes a single table row.
    template <typename Connection, typename Entity, typename PrimaryKey>
    inline std::string create_delete_sql(PrimaryKey const& key)
    {
        std::stringstream ss;

        ss << "DELETE FROM ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << " WHERE ";
        output_column_name<Connection>(ss, primary_key_column_of<Entity>());
        ss << '=';
        output_column_value<Connection>(ss, key);
        ss << ';';

        return ss.str();
    }
}}

#endif // #if !defined( CRYO_SQL_CREATE_DELETE_SQL_HPP )
