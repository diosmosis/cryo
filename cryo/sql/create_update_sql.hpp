///////////////////////////////////////////////////////////////////////////////
//
//! \file create_update_sql.hpp
//! Contains the \ref create_update_sql free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_CREATE_UPDATE_SQL_HPP )
#define CRYO_SQL_CREATE_UPDATE_SQL_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/output_column_name.hpp>
#include <cryo/sql/output_table_name.hpp>
#include <cryo/sql/output_column_value.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <cryo/schema/introspect/primary_key_column_of.hpp>
#include <cryo/schema/introspect/primary_key_of.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    namespace detail
    {
        template <typename Connection, typename Entity>
        struct output_set_statement
        {
            output_set_statement(Entity const& e)
                : entity(e)
            {}

            template <typename T, typename Entity>
            void operator()(std::stringstream & ss, T Entity::* member) const
            {
                output_column_name<Connection>(ss, member);
                ss << '=';
                output_column_value<Connection>(ss, entity.*member);
            }

            template <typename C>
            void operator()(std::stringstream & ss, C c) const
            {
                output_column_name<Connection>(ss, c);
                ss << '=';
                output_column_value<Connection>(ss, entity.*c.member());
            }

            Entity const& entity;
        };
    }

    //! \brief Creates an \c UPDATE SQL statement that modifies a row with the values held
    //!        in \c entity for the columns specified by \c columns.
    //! \tparam Connection a \ref RelationalDatabaseConnection type specifying which database
    //!                    the resulting SQL should work with.
    //! \param entity an entity that references the table row that will be updated and holds
    //!               the data used to do the update.
    //! \param columns a Boost.Fusion random access sequence of \ref PartialColumnDescriptors.
    //! \returns an \c UPDATE statement that will update the columns described by \c columns of
    //!          the row referenced by \c entity.
    template <typename Connection, typename Entity, typename Columns>
    inline std::string create_update_sql(Entity const& entity, Columns const& columns)
    {
        std::stringstream ss;
        ss << "UPDATE ";
        output_table_name<Connection>(ss, table_name_of<Entity>());
        ss << " SET ";
        enumerate(columns, detail::output_set_statement<Connection, Entity>(entity), ss, ',');
        ss << " WHERE ";
        output_column_name<Connection>(ss, primary_key_column_of<Entity>());
        ss << " = ";
        output_column_value<Connection>(ss, primary_key_of(entity));
        ss << ';';
        return ss.str();
    }
}}

#endif // #if !defined( CRYO_SQL_CREATE_UPDATE_SQL_HPP )
