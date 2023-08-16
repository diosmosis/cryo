///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE cryo_create_count_sql
#include <boost/test/included/unit_test.hpp>

#if defined(max)
#   undef max
#endif

#if defined(min)
#   undef min
#endif

#include <cryo/concept/relational_database_connection.hpp>
#include <cryo/sql/create_delete_sql.hpp>
#include "test_schema.hpp"

using namespace cryo;
using namespace cryo::sql::dynamic;

BOOST_AUTO_TEST_CASE( count_all_test )
{
    BOOST_CHECK_EQUAL(
        (sql::create_count_sql<concepts::RelationalDatabaseConnection, author>()),
        "SELECT COUNT(*) FROM author;");
}

BOOST_AUTO_TEST_CASE( count_one_test )
{
    BOOST_CHECK_EQUAL(
        (sql::create_count_sql<concepts::RelationalDatabaseConnection, author>(234)),
        "SELECT COUNT(*) FROM author WHERE id=234;");
}

BOOST_AUTO_TEST_CASE( count_query_test )
{
    condition q = col(&book::title) == "abc"
     && col(&book::series)(&series::title) == "def"
     && col(&book::series)(&series::author)(&author::name) == "ghi";

    BOOST_CHECK_EQUAL(
        (sql::create_count_sql<concepts::RelationalDatabaseConnection, book>(q)),
        "SELECT COUNT(*) FROM book t0"
        " LEFT JOIN series t1 ON t1.id=t0.series"
        " LEFT JOIN author t2 ON t2.id=t1.author"
        " WHERE (((t0.title='abc')AND(t1.title='def'))AND(t2.name='ghi'));"
        );
}
