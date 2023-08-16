///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE sqlite3_database
#include <boost/test/included/unit_test.hpp>

#if defined(max)
#   undef max
#endif

#if defined(min)
#   undef min
#endif

#include <cryo/relational/backend/mysql.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/scoped_ptr.hpp>
#include "database_suite.hpp"

struct db_fixture
{
    db_fixture(bool clear = true)
    {
        try
        {
            conn.reset(new mysql::connection(
                "localhost", BOOST_PP_STRINGIZE(CRYO_MYSQL_USER), BOOST_PP_STRINGIZE(CRYO_MYSQL_PWD), "testdb"));

            if (clear)
            {
                conn->execute_query("DROP DATABASE IF EXISTS testdb;");
                conn->execute_query("CREATE DATABASE testdb;");
                conn->execute_query("USE testdb;");

                // create the db
                create_db<test_schema>(*conn);
            }
        }
        catch (std::exception const& e)
        {
            BOOST_TEST_MESSAGE("failed to create db_fixture: " << e.what());
            throw;
        }
    }

    ~db_fixture()
    {
        try
        {
            conn->execute_query("DROP DATABASE IF EXISTS testdb;");

            conn.reset();
        }
        catch (std::exception const& e)
        {
            BOOST_TEST_MESSAGE("failed to destroy db_fixture: " << e.what());
            throw;
        }
    }

    mysql::connection & concrete_type() { return *conn; }

    boost::scoped_ptr<mysql::connection> conn;
};

CONCRETE_TEST_SUITE_FIXTURE( mysql::connection, db_fixture, concepts::RelationalDatabaseConnection )
{
    CONCRETE_TEST_CASE( constructor_connect_failure )
    {
        BOOST_TEST_PASSPOINT();

        try
        {
            mysql::connection db_("incorrecthost", "user", "password", "testdb");
            BOOST_CHECK_MESSAGE(false, "opening mysql connection w/ incorrect user data did not throw");
        }
        catch (cryo::error const&)
        {
            // ignore
        }
    }
};

USE_CONCRETE_TEST_SUITE( mysql::connection );
