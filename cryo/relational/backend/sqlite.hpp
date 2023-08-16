///////////////////////////////////////////////////////////////////////////////
//
//! \file sqlite.hpp
//! Contains SQLite v3 connection, cursor, transaction scope and error types.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_RELATIONAL_BACKEND_SQLITE_HPP )
#define CRYO_RELATIONAL_BACKEND_SQLITE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/error.hpp>
#include <cryo/schema/introspect/primary_key_column_of.hpp>
#include <cryo/schema/introspect/table_name_of.hpp>
#include <cryo/schema/tag/foreign_key.hpp>
#include <cryo/schema/tag/blob.hpp>
#include <cryo/extension/output_datatype_hook.hpp>
#include <cryo/extension/output_column_modifier_hook.hpp>
#include <cryo/extension/output_exterior_column_modifier_hook.hpp>
#include <cryo/extension/select_last_insert_hook.hpp>
#include <cryo/extension/last_insert_id_hook.hpp>
#include <cryo/sql/output_column_name.hpp>
#include <cryo/sql/output_table_name.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/move/move.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include <boost/assert.hpp>
#include <string>
#include <sstream>
#include <ios>
#include <limits>

#if defined( BOOST_MSVC )
#   pragma warning( push )
#   pragma warning( disable : 4244 )
#endif

namespace cryo
{
    namespace sqlite
    {
        template <typename T, typename Int>
        inline bool within_range(Int v)
        {
            return v >= T((std::numeric_limits<T>::min)()) && v < T((std::numeric_limits<T>::max)());
        }

        typedef boost::int64_t int_type;
        typedef boost::uint64_t uint_type;

        struct transaction;

        struct blob_source
        {
            BOOST_MOVABLE_BUT_NOT_COPYABLE(blob_source)

        public:
            typedef unsigned char char_type;
            typedef boost::iostreams::source_tag category;

            blob_source(void const* data_, std::streamsize length_)
                : data(data_)
                , length(length_)
                , location(0)
            {}

            blob_source(BOOST_RV_REF(blob_source) x)
                : data(x.data)
                , length(x.length)
                , location(x.location)
            {}

            CRYO_DECL std::streamsize read(char_type * buffer, std::streamsize n);

            void const* data;
            std::streamsize length;
            std::streamoff location;
        };

        //! \brief SQLite3 \ref Cursor type.
        //! Models the \ref Cursor concept.
        struct cursor
        {
            typedef int_type int_type;

            typedef blob_source blob_source_type;
            typedef boost::shared_ptr<void> cursor::* safe_bool_type;

            struct delete_stmt
            {
                delete_stmt(boost::weak_ptr<void> const& conn_)
                    : conn(conn_)
                {}

                CRYO_DECL void operator()(void * stmt) const;

                boost::weak_ptr<void> conn;
            };

            cursor()
                : stmt()
                , conn()
            {}

            cursor(cursor const& x)
                : conn(x.conn)
                , stmt(x.stmt)
                , col(x.col)
            {}

            CRYO_DECL explicit cursor(void * stmt_, boost::weak_ptr<void> const& conn_);

            CRYO_DECL void get_column(int_type & value) const;

            template <typename T>
            typename boost::enable_if<
                boost::is_integral<T>, void
            >::type get_column(T & value) const
            {
                int_type cv;
                get_column(cv);

                BOOST_ASSERT(within_range<T>(cv));

                value = static_cast<T>(cv);
            }

            void get_column(float & value) const
            {
                double cv;
                get_column(cv);

                value = cv;
            }

            CRYO_DECL void get_column(double & value) const;

            CRYO_DECL void get_column(std::string & value) const;

            CRYO_DECL blob_source_type get_column_blob() const;

            CRYO_DECL bool is_column_null() const;

            CRYO_DECL void next();

            void next_column()
            {
                ++col;
            }

            CRYO_DECL bool at_end() const;

            CRYO_DECL bool at_column_end() const;

            CRYO_DECL operator safe_bool_type() const;

        private:
            boost::weak_ptr<void> conn;
            boost::shared_ptr<void> stmt;
            unsigned int col;
        };

        //! \brief SQLite3 database connection type.
        //! Models the \ref RelationalDatabaseConnection concept.
        struct connection
        {
            //! \brief SQLite3's signed integer type.
            typedef boost::int64_t      int_type;

            //! \brief SQLite3's unsigned integer type.
            typedef boost::uint64_t     uint_type;

            //! \brief The SQLite3 \ref Cursor model used to read result set data.
            typedef cursor              cursor_type;

            //! \brief A type that can be used to start and manage transactions under SQLite3.
            typedef transaction         transaction_scope_type;

            typedef boost::shared_ptr<void> connection::* safe_bool_type;

            //! \brief Constructor. Opens a connection to the SQLite3 database referenced
            //!        by \c file.
            //! If the file referenced by \c file does not exist, this function creates it.
            //! \param file a path to an SQLite3 database file.
            //! \throws error if \c file is an invalid path or a connection to \c cannot be
            //!               opened.
            CRYO_DECL connection(std::string const& file);

            //! \brief Copy constructor.
            //! \ref connection is a reference type, so a copy of a connection instance will
            //! reference the same database as the original, as well as the same underlying data.
            //! If an entirely new connection to the same database is required, a conncection
            //! should be created with a path to the database file.
            //! \param x the \ref connection instance to copy from.
            connection(connection const& x)
                : conn(x.conn)
            {}

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

            //! \brief Returns the value of the last inserted row's \c ROWID column.
            //! \remark The behavior of this function is undefined if the connection is inactive or
            //!         the last SQL query executed was not an \c INSERT query.
            //! \returns the last inserted \c ROWID.
            CRYO_DECL uint_type last_insert_rowid() const;

            //! \brief Tests whether the \ref connection instance is active or not.
            //! \returns a value convertible to true if the \ref connection is active, false if
            //!          otherwise.
            operator safe_bool_type() const
            {
                return conn ? &connection::conn : 0;
            }

        private:
            friend struct transaction;

            boost::shared_ptr<void> conn;
        };
        // TODO: Need to note if cryo supports nested transactions or not.
        //! \brief Creates and manages SQLite3 transactions.
        //! Models the \ref TransactionScope concept.
        struct transaction
            : boost::noncopyable
        {
            //! \brief Constructor. Starts a transaction.
            //! \param conn_ the SQLite3 connection referencing the database to start a transaction
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
            CRYO_DECL static void on_implicit_rollback(void * self);

            connection conn;
            bool do_rollback;
            bool committed;
        };

        //! \brief Boost.System \c error_category for SQLite3 errors.
        //! \remark Since a connection is required to get an error message, the message function 
        //!         of this type does nothing. Instead, the error description is obtained before
        //!         an \ref sqlite::error is thrown.
        struct sqlite_error_category
            : boost::system::error_category
        {
            char const* name() const
            {
                return "cryo.sqlite";
            }

            std::string message(int value) const
            {
                return "";
            }

            static sqlite_error_category cat;
        };

        //! \brief The exception thrown by the SQLite3 backend of cryo.
        struct error
            : cryo::error
        {
            //! \brief Constructor.
            //! \param ev the error code.
            error(int ev)
                : cryo::error(ev, sqlite_error_category::cat)
            {}

            //! \brief Constructor.
            //! \param ev the error code.
            //! \param what_arg the error message.
            error(int ev, std::string const& what_arg)
                : cryo::error(ev, sqlite_error_category::cat, what_arg)
            {}
        };

        template <typename T, typename Enable = void>
        struct output_datatype_impl;

        template <typename T>
        struct output_datatype_impl<T, typename boost::enable_if<boost::is_integral<T> >::type>
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "INTEGER";
            }
        };

        template <typename T>
        struct output_datatype_impl<T, typename boost::enable_if<boost::is_floating_point<T> >::type>
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "REAL";
            }
        };
    }

    namespace extension
    {
        // sqlite's sql generation hooks
        // hooks that map c++ types to sqlite's datatypes
        template <typename T>
        struct output_datatype_hook<
            sqlite::connection,
            T,
            tag::primitive
        > : sqlite::output_datatype_impl<T>
        {};

        template <>
        struct output_datatype_hook<
            sqlite::connection,
            std::string,
            tag::primitive
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "TEXT";
            }
        };

        template <typename T, typename S>
        struct output_datatype_hook<
            sqlite::connection,
            T,
            tag::blob<S>
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "BLOB";
            }
        };

        // hooks that map cryo wrapper tags to sqlite column modifiers
        template <typename C, typename T>
        struct output_column_modifier_hook<
            sqlite::connection,
            C,
            T,
            tag::autogenerate
        >
        {
            static_assert(
                boost::is_integral<T>::value,
                "cryo.sqlite: The autogenerate tag can only be used with integer types."
            );

            void operator()(std::stringstream & ss) const
            {
                ss << "AUTOINCREMENT";
            }
        };

        template <typename C, typename T>
        struct output_column_modifier_hook<
            sqlite::connection,
            C,
            T,
            tag::primary_key
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "PRIMARY KEY";
            }
        };

        // hook to add foreign key constraints
        template <typename C, typename T, typename ForeignEntity>
        struct output_exterior_column_modifier_hook<
            sqlite::connection,
            C,
            T,
            tag::foreign_key<ForeignEntity>
        >
        {
            void operator()(std::stringstream & ss) const
            {
                typedef typename C::entity_type entity_type;

                ss << "FOREIGN KEY(";
                sql::output_column_name<sqlite::connection>(ss, C());
                ss << ")REFERENCES ";
                sql::output_table_name<sqlite::connection>(ss, table_name_of<ForeignEntity>());
                ss << '(';
                sql::output_column_name<sqlite::connection>(ss, primary_key_column_of<ForeignEntity>());
                ss << ')';
            }
        };

        // hook that gets & returns the last inserted id
        template <typename T>
        struct select_last_insert_hook<
            sqlite::connection,
            T
        >
        {
            T operator()(sqlite::connection const& conn) const
            {
                return (T)(conn.last_insert_rowid());
            }
        };

        // hook that adds SQL that selects the last inserted id
        template <>
        struct last_insert_id_hook<
            sqlite::connection
        >
        {
            void operator()(std::stringstream & ss) const
            {
                ss << "last_insert_rowid()";
            }
        };
    }
}

#if defined( BOOST_MSVC )
#   pragma warning( pop )
#endif

#endif // #if !defined( CRYO_RELATIONAL_BACKEND_SQLITE_HPP )
