///////////////////////////////////////////////////////////////////////////////
//
//! \file create_table_sql.hpp
//! Contains the \ref create_table_sql free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_CREATE_TABLE_SQL_HPP )
#define CRYO_SQL_CREATE_TABLE_SQL_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/column_name_of.hpp>
#include <cryo/sql/output_table_name.hpp>
#include <cryo/sql/enumerate.hpp>
#include <cryo/sql/for_each.hpp>
#include <cryo/sql/output_column_name.hpp>
#include <cryo/sql/output_column_datatype.hpp>
#include <cryo/sql/output_column_modifiers.hpp>
#include <cryo/sql/output_exterior_column_modifiers.hpp>
#include <cryo/schema/introspect/columns_of.hpp>
#include <cryo/schema/introspect/table_name_of.hpp>
#include <boost/mpl/bool.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    namespace detail
    {
        template <typename Connection>
        struct create_column_sql
        {
            template <typename C>
            void operator()(std::stringstream & ss, C c) const
            {
                output_column_name<Connection>(ss, c);
                ss << ' ';
                output_column_datatype<Connection>(ss, c);

                output_column_modifiers<Connection>(ss, c);
            }
        };

        template <typename Connection>
        struct output_exterior_modifiers
        {
            template <typename C>
            void operator()(std::stringstream & ss, C c) const
            {
                output_exterior_column_modifiers<Connection>(ss, c);
            }
        };
    }

    //! \brief Creates a <tt>CREATE TABLE</tt> SQL statement that will create a table
    //!        compatible with a given entity type.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \tparam Entity the entity type whose table should be specified.
    template <typename Connection, typename Entity>
    inline std::string create_table_sql()
    {
        std::stringstream ss;

        ss << "CREATE TABLE ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << '(';
        enumerate(columns_of<Entity>(), detail::create_column_sql<Connection>(), ss, ',');
        for_each(columns_of<Entity>(), detail::output_exterior_modifiers<Connection>(), ss);
        ss << ");";
 
        return ss.str();
    }
}}

#endif // #if !defined( CRYO_SQL_CREATE_TABLE_SQL_HPP )
