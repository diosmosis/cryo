///////////////////////////////////////////////////////////////////////////////
//
//! \file create_insert_sql.hpp
//! Contains the \ref create_insert_sql free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_CREATE_INSERT_SQL_HPP )
#define CRYO_SQL_CREATE_INSERT_SQL_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/output_column_value.hpp>
#include <cryo/sql/output_column_name.hpp>
#include <cryo/sql/output_table_name.hpp>
#include <cryo/sql/enumerate.hpp>
#include <cryo/schema/entity_column.hpp>
#include <cryo/schema/introspect/primary_key_column_of.hpp>
#include <cryo/schema/introspect/table_name_of.hpp>
#include <cryo/schema/introspect/columns_of.hpp>
#include <cryo/schema/blob_reference.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    namespace detail
    {
        template <typename Connection>
        struct output_column_name_impl
        {
            template <typename C>
            void operator()(std::stringstream & ss, C c) const
            {
                output_column_name<Connection>(ss, c);
            }
        };

        template <typename Connection, typename Entity>
        struct output_entity_column
        {
            output_entity_column(Entity const& entity_)
                : entity(entity_)
            {}

            template <typename C>
            void operator()(std::stringstream & ss, C c) const
            {
                output_column_value<Connection>(ss, entity.*(c.member()));
            }

            Entity const& entity;
        };
    }

    //! \brief Creates an \c INSERT SQL statement for an entity.
    //! \remark This function will ignore all autogenerate columns.
    //! \tparam Connection a \ref RelationalDatabaseConnection describing the database
    //!                    the resulting SQL must execute in.
    //! \param entity an entity holding the data that is to be used to intialize the new
    //!               table row.
    template <typename Connection, typename Entity>
    inline std::string create_insert_sql(Entity const& entity)
    {
        typedef typename boost::mpl::remove_if<
            typename result_of::columns_of<Entity>::type,
            boost::mpl::or_<
                is_autogenerate<boost::mpl::_1>,
                boost::is_same<
                    blob_reference,
                    field_type_of<boost::mpl::_1>
                >
            >
        >::type filtered_columns;

        std::stringstream ss;
        ss << "INSERT INTO ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << " (";
        enumerate(filtered_columns(), detail::output_column_name_impl<Connection>(), ss, ',');
        ss << ") VALUES (";
        enumerate(filtered_columns(), detail::output_entity_column<Connection, Entity>(entity), ss, ',');
        ss << ");";
        return ss.str();
    }
}}

#endif // #if !defined( CRYO_SQL_CREATE_INSERT_SQL_HPP )
