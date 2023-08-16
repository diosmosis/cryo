///////////////////////////////////////////////////////////////////////////////
//
//! \file mysql.hpp
//! Contains MySQL connection, cursor, transaction scope and error types.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_RELATIONAL_BACKEND_MYSQL_HPP )
#define CRYO_RELATIONAL_BACKEND_MYSQL_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/error.hpp>
#include <cryo/extension/output_datatype_hook.hpp>
#include <cryo/extension/output_column_modifier_hook.hpp>
#include <cryo/extension/output_exterior_column_modifier_hook.hpp>
#include <cryo/extension/select_last_insert_hook.hpp>
#include <cryo/extension/last_insert_id_hook.hpp>
#include <cryo/sql/output_column_name.hpp>
#include <cryo/sql/output_table_name.hpp>
#include <cryo/schema/introspect/table_name_of.hpp>
#include <cryo/schema/introspect/primary_key_column_of.hpp>
#include <cryo/schema/tag/varchar.hpp>
#include <cryo/schema/tag/blob.hpp>
#include <cryo/schema/tag/foreign_key.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/move/move.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <ios>

#if defined( BOOST_MSVC )
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif

namespace cryo
{
    namespace mysql
    {
        typedef boost::int64_t int_type;
        typedef boost::uint64_t uint_type;

        struct transaction;

        struct blob_source
        {
            BOOST_MOVABLE_BUT_NOT_COPYABLE(blob_source)

        public:
            typedef unsigned char char_type;
            typedef boost::iostreams::source_tag category;

            blob_source(char_type const* s, std::streamsize l)
                : data(s)
                , length(l)
                , location(0)
            {}

            blob_source(BOOST_RV_REF(blob_source) x)
                : data(x.data)
                , length(x.length)
                , location(x.location)
            {}

            CRYO_DECL std::streamsize read(char_type * buffer, std::streamsize n);

        private:
            char_type const* data;
            std::streamsize length;
            std::streamoff location;
        };

        //! \brief MySQL \ref Cursor type.
        //! Models the \ref Cursor concept.
        struct cursor
        {
            typedef int_type int_type;
            typedef blob_source blob_source_type;
            typedef boost::shared_ptr<void> cursor::* safe_bool_type;

            cursor()
                : impl()
                , conn()
                , row(0)
            {}

            CRYO_DECL cursor(void * impl_, boost::shared_ptr<void> conn);

            cursor(cursor const& x)
                : impl(x.impl)
                , conn(x.conn)
                , row(x.row)
                , col(x.col)
            {}

            template <typename Numeric>
            typename boost::enable_if<
                boost::is_arithmetic<Numeric>, void
            >::type get_column(Numeric & value) const
            {
                value = boost::lexical_cast<Numeric>(row[col]);
            }

            void get_column(std::string & value) const
            {
                value = std::string(row[col]);
            }

            CRYO_DECL blob_source_type get_column_blob() const;

            bool is_column_null() const
            {
                return row[col] ? false : true;
            }

            CRYO_DECL void next();

            void next_column()
            {
                ++col;
            }

            bool at_end() const
            {
                return row == 0;
            }

            CRYO_DECL bool at_column_end() const;

            operator safe_bool_type() const
            {
                return impl && !conn.expired() ? &cursor::impl : 0;
            }

        private:
            boost::shared_ptr<void> impl;
            boost::weak_ptr<void> conn;
            char ** row;
            unsigned int col;
        };

        //! \brief MySQL database connection type.
        //! Models the \ref RelationalDatabaseConnection concept.
        struct connection
        {
            //! \brief MySQL's signed integer type.
            typedef boost::int64_t      int_type;

            //! \brief MySQL's unsigned integer type.
            typedef boost::uint64_t     uint_type;

            //! \brief The MySQL \ref Cursor model used to read result set data.
            typedef cursor cursor_type;

            //! \brief A type that can be used to start and manage transactions in MySQL.
            typedef transaction transaction_scope_type;

            typedef boost::shared_ptr<void> connection::* safe_bool_type;

            //! \brief Constructor. Opens a connection to the database specified by the supplied
            //!        arguments.
            //! If the database referenced by \c dbname does not exist, it is created.
            //! \param host the host name of the machine the MySQL database is on.
            //! \param user the user name to login with.
            //! \param pwd the user's associated password.
            //! \param dbname the name of the database to select once logged in.
            //! \param port the port to use when connecting.
            //! \throws error if a connection cannot be made.
            CRYO_DECL connection(
                std::string const& host,
                std::string const& user,
                std::string const& pwd,
                std::string const& dbname,
                unsigned int port = 0);

            //! \brief Executes the supplied SQL and returns a \ref Cursor to read the result set
            //!        data, if any.
            //! \param query the SQL to execute.
            //! \returns a \ref Cursor that can be used to read the data returned by the SQL. If no
            //!          data is returned, the \ref Cursor's <tt>at_end()</tt> function will return
            //!          true.
            //! \throws sqlite::error if the connection is inactive, if \c query is invalid SQL or
            //!                       if \c query violates a constraint.
            CRYO_DECL cursor execute_query(std::string const& query) const;

            //! \brief Returns the affected rows of the last query, if the last query is an \c UPDATE
            //!        or \c DELETE statement.
            //! \remark The behavior of this function is undefined if the connection is inactive
            //!         or if the last SQL query executed was not an \c UPDATE or \c DELETE.
            //! \returns the affected row count of the last executed \c UPDATE or \c DELETE query.
            CRYO_DECL uint_type affected_row_count() const;

            //! \brief Returns the last generated \c AUTO_INCREMENT integer, if the last query was an
            //!        \c INSERT statement.
            //! \remark The behavior of this function is undefined if the connection is inactive, if
            //!         the last SQL query executed was not an \c INSERT or if the last inserted row
            //!         did not have an \c AUTO_INCREMENT column.
            //! \remark If the last inserted row has more than one \c AUTO_INCREMENT column, the last
            //!         generated id is returned.
            //! \returns the last generated \c AUTO_INCREMENT integer.
            CRYO_DECL uint_type last_insert_id() const;

            //! \brief Tests whether the \ref connection instance is active or not.
            //! \returns a value convertible to true if the \ref connection is active, false if
            //!          otherwise.
            operator safe_bool_type() const
            {
                return conn ? &connection::conn : 0;
            }

        private:
            boost::shared_ptr<void> conn;
        };

        //! \brief Creates and manages MySQL transactions.
        //! Models \ref TransactionScope.
        struct transaction
        {
            //! \brief Constructor. Starts a transaction.
            //! \param conn_ the MySQL connection referencing the database to start a transaction
            //!              in.
            //! \throws error if a transaction cannot be started.
            CRYO_DECL transaction(connection const& conn_);

            //! \brief Destructor. Rolls back a transaction if \c commit() has not been called.
            //! \throws Nothing. If an error occurs during rollback, it is ignored.
            CRYO_DECL ~transaction();

            //! \brief Commits the transaction.
            //! \throws error if the transaction cannot be committed.
            CRYO_DECL void commit();

        private:
            connection const& conn;
            bool committed;
        };

        //! \brief Boost.System \c error_category for MySQL errors.
        //! \remark Since a connection is required to get an error message, the message function 
        //!         of this type does nothing. Instead, the error description is obtained before
        //!         an \ref mysql::error is thrown.
        struct mysql_error_category
            : boost::system::error_category
        {
            char const* name() const
            {
                return "cryo.mysql";
            }

            std::string message(int value) const
            {
                return "";
            }

            static mysql_error_category cat;
        };

        //! \brief The exception thrown by the MySQL backend of cryo.
        struct error
            : cryo::error
        {
            //! \brief Constructor.
            //! \param ev the error code.
            //! \param mysql_err the error description provided by MySQL.
            error(int ev, char const* mysql_err)
                : cryo::error(ev, mysql_error_category::cat, make_error_str(mysql_err, 0))
            {}

            //! \brief Constructor.
            //! \param ev the error code.
            //! \param mysql_err the error description provided by MySQL.
            //! \param what_arg a description of the error context provided by cryo.
            error(int ev, char const* mysql_err, std::string const& what_arg)
                : cryo::error(ev, mysql_error_category::cat, make_error_str(mysql_err, what_arg.c_str()))
            {}

        private:
            CRYO_DECL static std::string make_error_str(char const* mysql_err, char const* what);
        };
    }

    namespace extension
    {
        // mysql's sql generation hooks
        // hooks that map c++ types to mysql's datatypes
        template <>
        struct output_datatype_hook<
            mysql::connection,
            boost::int8_t,
            tag::primitive
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "TINYINT";
            }
        };

        template <>
        struct output_datatype_hook<
            mysql::connection,
            boost::uint8_t,
            tag::primitive
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "TINYINT UNSIGNED";
            }
        };

        template <>
        struct output_datatype_hook<
            mysql::connection,
            boost::int16_t,
            tag::primitive
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "SMALLINT";
            }
        };

        template <>
        struct output_datatype_hook<
            mysql::connection,
            boost::uint16_t,
            tag::primitive
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "SMALLINT UNSIGNED";
            }
        };

        template <>
        struct output_datatype_hook<
            mysql::connection,
            boost::int32_t,
            tag::primitive
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "INT";
            }
        };

        template <>
        struct output_datatype_hook<
            mysql::connection,
            boost::uint32_t,
            tag::primitive
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "INT UNSIGNED";
            }
        };

        template <>
        struct output_datatype_hook<
            mysql::connection,
            boost::int64_t,
            tag::primitive
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "BIGINT";
            }
        };

        template <>
        struct output_datatype_hook<
            mysql::connection,
            boost::uint64_t,
            tag::primitive
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "BIGINT UNSIGNED";
            }
        };

        template <>
        struct output_datatype_hook<
            mysql::connection,
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
            mysql::connection,
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
            mysql::connection,
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
            mysql::connection,
            T,
            tag::varchar<L>
        >
        {
            static_assert(
                boost::is_same<std::string, T>::value,
                "cryo.mysql: The varchar tag can only be used with strings.");

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
            mysql::connection,
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
            mysql::connection,
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
            mysql::connection,
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
            mysql::connection,
            C,
            T,
            tag::autogenerate
        >
        {
            static_assert(
                boost::is_integral<T>::value,
                "cryo.mysql: The autogenerate tag can only be used with integer types."
            );

            void operator()(std::stringstream & ss) const
            {
                ss << "AUTO_INCREMENT";
            }
        };

        // hooks that map cryo wrapper tags to mysql 'exterior' column modifiers
        template <typename C, typename T>
        struct output_exterior_column_modifier_hook<
            mysql::connection,
            C,
            T,
            tag::primary_key
        >
        {
            static_assert(
                boost::mpl::if_<
                    typename boost::is_same<T, std::string>::type,
                    typename is_varchar<C>::type,
                    boost::mpl::true_
                >::type::value,
                "cryo.mysql: string primary keys must use the varchar tag."
            );

            void operator()(std::stringstream & ss) const
            {
                ss << "PRIMARY KEY (";
                sql::output_column_name<mysql::connection>(ss, C());
                ss << ')';
            }
        };

        template <typename C, typename T, typename ForeignEntity>
        struct output_exterior_column_modifier_hook<
            mysql::connection,
            C,
            T,
            tag::foreign_key<ForeignEntity>
        >
        {
            void operator()(std::stringstream & ss) const
            {
                typedef typename C::entity_type entity_type;

                ss << "FOREIGN KEY fk_";
                sql::output_table_name<mysql::connection>(ss, table_name_of<entity_type>());
                ss << '_';
                sql::output_column_name<mysql::connection>(ss, C());
                ss << '(';
                sql::output_column_name<mysql::connection>(ss, C());
                ss << ")REFERENCES ";
                sql::output_table_name<mysql::connection>(ss, table_name_of<ForeignEntity>());
                ss << '(';
                sql::output_column_name<mysql::connection>(ss, primary_key_column_of<ForeignEntity>());
                ss << ')';
            }
        };

        // hook that gets & returns the last inserted id
        template <typename T>
        struct select_last_insert_hook<
            mysql::connection,
            T
        >
        {
            T operator()(mysql::connection const& conn) const
            {
                return (T)(conn.last_insert_id());
            }
        };

        // hook that adds SQL that selects the last inserted id
        template <>
        struct last_insert_id_hook<
            mysql::connection
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "LAST_INSERT_ID()";
            }
        };
    }
}

#if defined( BOOST_MSVC )
#   pragma warning( pop )
#endif

#endif // #if !defined( CRYO_RELATIONAL_BACKEND_MYSQL_HPP )
