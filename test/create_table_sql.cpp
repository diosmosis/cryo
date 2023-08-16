///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE cryo_create_table_sql
#include <boost/test/included/unit_test.hpp>

#if defined(max)
#   undef max
#endif

#if defined(min)
#   undef min
#endif

#include <cryo/concept/relational_database_connection.hpp>
#include <cryo/sql/create_table_sql.hpp>
#include "test_schema.hpp"
#include "mock_sql_hooks.hpp"

using namespace cryo;
using namespace cryo::sql::dynamic;

BOOST_AUTO_TEST_CASE( create_table_test )
{
    BOOST_CHECK_EQUAL(
        (sql::create_table_sql<concepts::RelationalDatabaseConnection, author>()),
        "CREATE TABLE author(id INT32 AUTO_INCREMENT NOT NULL,name TEXT NOT NULL,PRIMARY KEY (id));"
        );

    BOOST_CHECK_EQUAL(
        (sql::create_table_sql<concepts::RelationalDatabaseConnection, series>()),
        "CREATE TABLE series(id INT32 AUTO_INCREMENT NOT NULL,title TEXT NOT NULL,author INT32 NOT NULL,"
        "PRIMARY KEY (id),FOREIGN KEY fk_series_author(author) REFERENCES author(id));"
        );

    BOOST_CHECK_EQUAL(
        (sql::create_table_sql<concepts::RelationalDatabaseConnection, book>()),
        "CREATE TABLE book(title VARCHAR(128) NOT NULL,author INT32 NOT NULL,series INT32,copyright_year INT32,"
        "cost UINT32 NOT NULL,category VARCHAR(32),PRIMARY KEY (title),FOREIGN KEY fk_book_author(author) "
        "REFERENCES author(id),FOREIGN KEY fk_book_series(series) REFERENCES series(id),FOREIGN KEY "
        "fk_book_category(category) REFERENCES category(name));"
        );
}
