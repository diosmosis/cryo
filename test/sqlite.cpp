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

#include <cryo/relational/backend/sqlite.hpp>
#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>
#include "database_suite.hpp"

struct db_fixture
{
    db_fixture(bool clear = true)
    {
        bool exists = false;

        // try and remove the database if it already exists
        if (clear)
        {
            try
            {
                boost::filesystem::remove("testdb.db");
            }
            catch (std::exception const&)
            {
                // note that the database file could not be removed
                exists = true;
            }
        }

        // connect to the DB
        conn.reset(new sqlite::connection("testdb.db"));

        // if the database does exist, assume the schema has been created and the reason we
        // couldn't connect was due to an active connection
        if (!exists && clear)
        {
            create_db<test_schema>(*conn);
        }
    }

    ~db_fixture()
    {
        conn.reset();

        try
        {
            boost::filesystem::remove("testdb.db");
        }
        catch (std::exception const& x)
        {
            BOOST_TEST_MESSAGE("couldn't delete 'testdb.db': " << x.what());
        }
    }

    sqlite::connection & concrete_type() { return *conn; }

    boost::scoped_ptr<sqlite::connection> conn;
};

CONCRETE_TEST_SUITE_FIXTURE( sqlite::connection, db_fixture, concepts::RelationalDatabaseConnection )
{
    CONCRETE_TEST_CASE( constructor_multiple_dbs_accuracy )
    {
        BOOST_TEST_PASSPOINT();

        sqlite::connection db_("testdb.db");
    }

    CONCRETE_TEST_CASE( constructor_failure )
    {
        try
        {
            sqlite::connection db_("..");
            BOOST_CHECK_MESSAGE( false, "did not throw when creating a db using a directory" );
        }
        catch (cryo::error const&)
        {
            // ignore
        }
    }
};

USE_CONCRETE_TEST_SUITE( sqlite::connection );
