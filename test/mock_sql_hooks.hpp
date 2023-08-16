///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( MOCK_SQL_HOOKS_HPP )
#define MOCK_SQL_HOOKS_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension.hpp>
#include <cryo/sql/output_column_name.hpp>
#include <cryo/sql/output_table_name.hpp>
#include <cryo/schema/introspect/table_name_of.hpp>
#include <cryo/schema/introspect/primary_key_column_of.hpp>
#include <cryo/schema/tag/varchar.hpp>
#include <cryo/schema/tag/blob.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <ios>

namespace cryo { namespace extension
{
    // hooks that map c++ types to mysql's datatypes
    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        boost::int8_t,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "INT8";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        boost::uint8_t,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "UINT8";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        boost::int16_t,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "INT16";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        boost::uint16_t,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "UINT16";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        boost::int32_t,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "INT32";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        boost::uint32_t,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "UINT32";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        boost::int64_t,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "INT64";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        boost::uint64_t,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "UINT64";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        float,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "FLOAT";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        double,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "DOUBLE";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        std::string,
        tag::primitive
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "TEXT";
        }
    };

    template <typename T, int L>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        T,
        tag::varchar<L>
    >
    {
        void operator()(std::stringstream & ss) const
        {
            if (L == -1)
            {
                ss << "TEXT";
            }
            else
            {
                ss << "VARCHAR(" << L << ')';
            }
        }
    };

    template <typename T, typename S>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        T,
        tag::blob<S>
    >
    {
        typedef typename max_serialized_blob_size<T, tag::blob<S> >::type max_size;

        void operator()(std::stringstream & ss) const
        {
            ss << "BLOB";

            if (max_size::value != -1)
            {
                ss << '(' << max_size::value << ')';
            }
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        std::string,
        tag::date
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "DATE";
        }
    };

    template <>
    struct output_datatype_hook<
        concepts::RelationalDatabaseConnection,
        std::string,
        tag::datetime
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "DATETIME";
        }
    };

    // hooks that map cryo wrapper tags to mysql column modifiers
    template <typename C, typename T>
    struct output_column_modifier_hook<
        concepts::RelationalDatabaseConnection,
        C,
        T,
        tag::autogenerate
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "AUTO_INCREMENT";
        }
    };

    // hooks that map cryo wrapper tags to mysql 'exterior' column modifiers
    template <typename C, typename T>
    struct output_exterior_column_modifier_hook<
        concepts::RelationalDatabaseConnection,
        C,
        T,
        tag::primary_key
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "PRIMARY KEY (";
            sql::output_column_name<concepts::RelationalDatabaseConnection>(ss, C());
            ss << ')';
        }
    };

    template <typename C, typename T, typename ForeignEntity>
    struct output_exterior_column_modifier_hook<
        concepts::RelationalDatabaseConnection,
        C,
        T,
        tag::foreign_key<ForeignEntity>
    >
    {
        void operator()(std::stringstream & ss) const
        {
            typedef typename C::entity_type entity_type;

            ss << "FOREIGN KEY fk_";
            sql::output_table_name<concepts::RelationalDatabaseConnection>(ss, table_name_of<entity_type>());
            ss << '_';
            sql::output_column_name<concepts::RelationalDatabaseConnection>(ss, C());
            ss << '(';
            sql::output_column_name<concepts::RelationalDatabaseConnection>(ss, C());
            ss << ") REFERENCES ";
            sql::output_table_name<concepts::RelationalDatabaseConnection>(ss, table_name_of<ForeignEntity>());
            ss << '(';
            sql::output_column_name<concepts::RelationalDatabaseConnection>(ss, primary_key_column_of<ForeignEntity>());
            ss << ')';
        }
    };

    // hook that gets & returns the last inserted id
    template <typename T>
    struct select_last_insert_hook<
        concepts::RelationalDatabaseConnection,
        T
    >
    {
        T operator()(concepts::RelationalDatabaseConnection const& conn) const
        {
            return T();
        }
    };

    // hook that adds SQL that selects the last inserted id
    template <>
    struct last_insert_id_hook<
        concepts::RelationalDatabaseConnection
    >
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "LAST_INSERT_ID()";
        }
    };
}}

#endif // #if !defined( MOCK_SQL_HOOKS_HPP )
