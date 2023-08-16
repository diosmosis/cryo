///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define CRYO_SOURCE

#include <cryo/relational/backend/mysql.hpp>
#include <sstream>

// mysql's C API does not include winsock2 when it should, so we have to
#if defined(BOOST_WINDOWS)
#   include <winsock2.h> 
#endif

#include <mysql.h>
#include <mysqld_error.h>

namespace cryo { namespace mysql
{
    namespace detail
    {
        static void close_mysql_connection(void * conn)
        {
            MYSQL * mysql_pointer = static_cast<MYSQL *>(conn);

            ::mysql_close(mysql_pointer);

            delete mysql_pointer;
        }

        static void free_result(void * res)
        {
            ::mysql_free_result(static_cast<MYSQL_RES *>(res));
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

        char_type const* start = data + location;
        std::copy(start, start + n, buffer);

        return n;
    }

    // cursor
    cursor::cursor(void * impl_, boost::shared_ptr<void> conn_)
    {
        impl.reset(impl_, &detail::free_result);
        conn = conn_;

        if (impl_)
        {
            next();
        }
        else
        {
            row = 0;
        }
    }

    blob_source cursor::get_column_blob() const
    {
        unsigned long * lengths = ::mysql_fetch_lengths(static_cast<MYSQL_RES *>(impl.get()));
        return blob_source_type(reinterpret_cast<unsigned char *>(row[col]), lengths[col]);
    }

    void cursor::next()
    {
        boost::shared_ptr<void> real_conn = conn.lock();

        if (!real_conn)
        {
            throw error(0, 0, "attempted to use cursor created by a (now) closed connection");
        }

        row = ::mysql_fetch_row(static_cast<MYSQL_RES *>(impl.get()));

        if (row == NULL)
        {
            MYSQL * mysql_ptr = static_cast<MYSQL *>(real_conn.get());

            unsigned int eno = ::mysql_errno(mysql_ptr);

            if (eno != 0)
            {
                throw error(eno, ::mysql_error(mysql_ptr), "failed to increment cursor");
            }
        }

        col = 0;
    }

    bool cursor::at_column_end() const
    {
        return col >= ::mysql_num_fields(static_cast<MYSQL_RES *>(impl.get()));
    }

    // connection
    connection::connection(
        std::string const& host
      , std::string const& user
      , std::string const& pwd
      , std::string const& dbname
      , unsigned int port)
    {
        // create and initialize
        MYSQL * mysql_ptr = new MYSQL();

        ::mysql_init(mysql_ptr);

        conn.reset(mysql_ptr, &detail::close_mysql_connection);

        // connect
        if (::mysql_real_connect(mysql_ptr, host.c_str(), user.c_str(),
                                 pwd.c_str(), dbname.c_str(), port, NULL, 0) == NULL)
        {
            unsigned int eno = ::mysql_errno(mysql_ptr);

            // if the database does not exist, create it & then reconnect
            if (eno == ER_BAD_DB_ERROR)
            {
                mysql_ptr = new MYSQL();

                ::mysql_init(mysql_ptr);

                conn.reset(mysql_ptr, &detail::close_mysql_connection);

                // connect w/ no database name
                if (::mysql_real_connect(mysql_ptr, host.c_str(), user.c_str(), pwd.c_str(), NULL, port, NULL, 0) == NULL)
                {
                    throw error(::mysql_errno(mysql_ptr), ::mysql_error(mysql_ptr),
                                "unable to connect (without specified database)");
                }

                // create the database
                std::stringstream ss;
                ss << "CREATE DATABASE " << dbname << ";";
                execute_query(ss.str());

                // change the default database
                if (::mysql_select_db(mysql_ptr, dbname.c_str()) != 0)
                {
                    throw error(::mysql_errno(mysql_ptr), ::mysql_error(mysql_ptr),
                                "unable to change database to '" + dbname + "'");
                }
            }
            else
            {
                throw error(eno, ::mysql_error(mysql_ptr), "unable to connect");
            }
        }
    }

    cursor connection::execute_query(std::string const& query) const
    {
        MYSQL * mysql_ptr = static_cast<MYSQL *>(conn.get());

        int err = ::mysql_real_query(mysql_ptr, query.c_str(), query.size());

        if (err != 0)
        {
            throw error(::mysql_errno(mysql_ptr), ::mysql_error(mysql_ptr), "invalid query");
        }

        void * result = ::mysql_use_result(static_cast<MYSQL *>(conn.get()));

        if (result)
        {
            return cursor(result, conn);
        }
        else
        {
            return cursor();
        }
    }

    connection::uint_type connection::affected_row_count() const
    {
        return ::mysql_affected_rows(static_cast<MYSQL *>(conn.get()));
    }

    connection::uint_type connection::last_insert_id() const
    {
        return ::mysql_insert_id(static_cast<MYSQL *>(conn.get()));
    }

    // transaction
    transaction::transaction(connection const& conn_)
        : conn(conn_)
        , committed(false)
    {
        conn.execute_query("START TRANSACTION;");
    }

    transaction::~transaction()
    {
        try
        {
            conn.execute_query("ROLLBACK;");
        }
        catch (...)
        {
            // ignore
        }
    }

    void transaction::commit()
    {
        conn.execute_query("COMMIT;");
        committed = true;
    }

    // mysql_error_category
    mysql_error_category mysql_error_category::cat;

    // error
    std::string error::make_error_str(char const* mysql_err, char const* what)
    {
        std::stringstream ss;

        if (what)
        {
            ss << what;
        }

        if (mysql_err)
        {
            ss << ": " << mysql_err;
        }

        return ss.str();
    }
}}
