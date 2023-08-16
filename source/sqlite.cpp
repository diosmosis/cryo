///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define CRYO_SOURCE

#include <cryo/relational/backend/sqlite.hpp>
#include <boost/assert.hpp>
#include <sqlite3.h>

namespace cryo { namespace sqlite
{
    namespace detail
    {
        static void close_connection(void * conn)
        {
            if (!conn)
            {
                return;
            }

            // get the database connection pointer
            ::sqlite3 * pDb = static_cast<::sqlite3 *>(conn);

            // find all prepared statements (if any) and close them
            ::sqlite3_stmt * pStmt;
            while ((pStmt = ::sqlite3_next_stmt(pDb, 0)) != 0)
            {
                ::sqlite3_finalize(pStmt);
            }

            // try and close the database
            int error = ::sqlite3_close(pDb);

            // check for errors (shouldn't happen)
            if (error != SQLITE_OK)
            {
                throw sqlite::error(error, "failed to close database");
            }
        }
    }

    // blob_source
    std::streamsize blob_source::read(char_type * buffer, std::streamsize n)
    {
        if (location + n >= length)
        {
            n = length - location - 1;

            if (n == 0)
            {
                return -1;
            }
        }

        char_type const* start = static_cast<char_type const*>(data) + location;
        std::copy(start, start + n, buffer);

        return n;
    }

    // cursor
    cursor::cursor(void * stmt_, boost::weak_ptr<void> const& conn_)
        : conn(conn_)
        , stmt(stmt_, delete_stmt(conn_))
    {
        next();
    }

    void cursor::get_column(int_type & value) const
    {
        // get the statement pointer
        sqlite3_stmt * pStmt = static_cast<sqlite3_stmt *>(stmt.get());

        // check the column type
        BOOST_ASSERT(::sqlite3_column_type(pStmt, col) == SQLITE_INTEGER ||
                     ::sqlite3_column_type(pStmt, col) == SQLITE_NULL);

        // access the column and return the result
        value = sqlite3_column_int(pStmt, col);
    }

    void cursor::get_column(double & value) const
    {
        // get the statement pointer
        ::sqlite3_stmt * pStmt = static_cast<::sqlite3_stmt *>(stmt.get());

        // check the column type
        BOOST_ASSERT(::sqlite3_column_type(pStmt, col) == SQLITE_FLOAT ||
                     ::sqlite3_column_type(pStmt, col) == SQLITE_NULL);

        // access the column and return the result
        value = ::sqlite3_column_double(pStmt, col);
    }

    void cursor::get_column(std::string & value) const
    {
        // get the statement pointer
        ::sqlite3_stmt * pStmt = static_cast<::sqlite3_stmt *>(stmt.get());

        // check the column type
        BOOST_ASSERT(::sqlite3_column_type(pStmt, col) == SQLITE_TEXT ||
                     ::sqlite3_column_type(pStmt, col) == SQLITE_NULL);

        unsigned char const* str = ::sqlite3_column_text(pStmt, col);

        // return a copy of the string
        value = std::string(reinterpret_cast<char const*>(str));
    }

    blob_source cursor::get_column_blob() const
    {
        // get the statement pointer
        ::sqlite3_stmt * pStmt = static_cast<::sqlite3_stmt *>(stmt.get());

        // check the column type
        BOOST_ASSERT(::sqlite3_column_type(pStmt, col) == SQLITE_BLOB ||
                     ::sqlite3_column_type(pStmt, col) == SQLITE_NULL);

        // the result
        std::pair<void const*, std::streamsize> result;

        // access the column data as a blob
        result.first = ::sqlite3_column_blob(pStmt, col);

        // get the length of the blob (after accessing so sqlite will behave correctly)
        result.second = ::sqlite3_column_bytes(pStmt, col);

        // if the length of the blog is zero, make sure the data pointer returned is a null pointer
        if (result.second == 0)
        {
            result.first = 0;
        }

        return blob_source(result.first, result.second);
    }

    bool cursor::is_column_null() const
    {
        return ::sqlite3_column_type(static_cast<::sqlite3_stmt *>(stmt.get()), col) == SQLITE_NULL;
    }

    void cursor::next()
    {
        // get the statement pointer
        ::sqlite3_stmt * pStmt = static_cast<::sqlite3_stmt *>(stmt.get());

        // increment and/or execute the query
        int error = ::sqlite3_step(pStmt);

        // check for errors
        if (error != SQLITE_OK && error != SQLITE_DONE && error != SQLITE_ROW)
        {
            // throw an exception
            if (boost::shared_ptr<void> conn_ = conn.lock())
            {
                throw sqlite::error(error, ::sqlite3_errmsg(static_cast<::sqlite3 *>(conn_.get())));
            }
            else
            {
                throw sqlite::error(error, "incrementing cursor failed");
            }
        }

        // if we've gone past the last row, reset the statement
        if (error == SQLITE_DONE)
        {
            stmt.reset();
        }

        col = 0;
    }

    cursor::operator cursor::safe_bool_type() const
    {
        return conn.expired() ? 0 : &cursor::stmt;
    }

    bool cursor::at_end() const
    {
        return !stmt;
    }

    bool cursor::at_column_end() const
    {
        return col >= ::sqlite3_column_count(static_cast<::sqlite3_stmt *>(stmt.get()));
    }

    void cursor::delete_stmt::operator()(void * stmt) const
    {
        // make sure the database hasn't been closed; if it has, the statement will have been deleted already
        if (!conn.expired())
        {
            ::sqlite3_finalize(static_cast<::sqlite3_stmt *>(stmt));
        }
    }

    // connection
    connection::connection(std::string const& file)
    {
        // the pointer to the database connection
        sqlite3 * pDb = NULL;

        // try and open a connection to the database specified by 'file'
        int error = ::sqlite3_open_v2(file.c_str(), &pDb, SQLITE_OPEN_READWRITE, NULL);

        // check for errors
        if (error != SQLITE_OK)
        {
            char const* what = "failed to open database";

            // close the connection if resources were allocated
            if (pDb != NULL)
            {
                // get the error message
                what = ::sqlite3_errmsg(pDb);

                // free resources
                ::sqlite3_close(pDb);
                pDb = NULL;
            }

            // if the database isn't missing, throw
            if (error != SQLITE_CANTOPEN && error != SQLITE_ERROR)
            {
                throw sqlite::error(error, what);
            }

            // if the database does not exist (at this point we know it doesn't), create it
            error = ::sqlite3_open_v2(file.c_str(), &pDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

            // check for errors
            if (error != SQLITE_OK)
            {
                what = "failed to create database";

                // close the connection if resources were allocated
                if (pDb != NULL)
                {
                    // get the error message
                    what = ::sqlite3_errmsg(pDb);

                    // free resources
                    ::sqlite3_close(pDb);
                }

                // throw an exception
                throw sqlite::error(error, what);
            }
        }

        conn.reset(pDb, &detail::close_connection);

        execute_query("PRAGMA foreign_keys = ON;");
    }

    cursor connection::execute_query(std::string const& query) const
    {
        if (query.empty())
        {
            return cursor();
        }

        // get the database connection pointer
        sqlite3 * pDb = static_cast<sqlite3 *>(conn.get());

        // prepare the statement
        sqlite3_stmt * pStmt = NULL;
        int error = ::sqlite3_prepare_v2(pDb, query.c_str(), query.size(), &pStmt, NULL);

        // check for errors
        if (error != SQLITE_OK || pStmt == NULL)
        {
            std::string what(::sqlite3_errmsg(pDb));

            // finalize the statement if it was created
            if (pStmt != NULL)
            {
                ::sqlite3_finalize(pStmt);
            }

            // throw an exception
            throw sqlite::error(error, what);
        }

        // create a cursor w/ the statement and return it
        return cursor(pStmt, boost::weak_ptr<void>(conn));
    }

    connection::uint_type connection::affected_row_count() const
    {
        return ::sqlite3_changes(static_cast<sqlite3 *>(conn.get()));
    }

    connection::uint_type connection::last_insert_rowid() const
    {
        return ::sqlite3_last_insert_rowid(static_cast<sqlite3 *>(conn.get()));
    }

    // transaction
    transaction::transaction(connection const& conn_)
        : conn(conn_)
        , do_rollback(true)
        , committed(false)
    {
        ::sqlite3_rollback_hook(static_cast<::sqlite3 *>(conn.conn.get()), &transaction::on_implicit_rollback, this);

        conn.execute_query("BEGIN;");
    }

    transaction::~transaction()
    {
        ::sqlite3_rollback_hook(static_cast<::sqlite3 *>(conn.conn.get()), NULL, NULL);

        if (do_rollback)
        {
            conn.execute_query("ROLLBACK;");
        }
    }

    void transaction::commit()
    {
        if (!committed)
        {
            conn.execute_query("COMMIT;");
            do_rollback = false;
            committed = true;
        }
    }

    void transaction::on_implicit_rollback(void * self)
    {
        static_cast<transaction *>(self)->do_rollback = false;
    }

    // error
    sqlite_error_category sqlite_error_category::cat;
}}
