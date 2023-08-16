///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE cryo_create_update_sql
#include <boost/test/included/unit_test.hpp>

#if defined(max)
#   undef max
#endif

#if defined(min)
#   undef min
#endif

#include <boost/fusion/include/pop_front.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <cryo/concept/relational_database_connection.hpp>
#include <cryo/sql/create_update_sql.hpp>
#include "test_schema.hpp"

using namespace cryo;
using namespace cryo::sql::dynamic;

BOOST_AUTO_TEST_CASE( update_all_test )
{
    book b0;
    b0.title = "Drive";
    b0.author = 45;
    b0.series.reset();
    b0.copyright_year = 2345;
    b0.cost = 1245;
    b0.category = boost::none;

    BOOST_CHECK_EQUAL(
        (sql::create_update_sql<concepts::RelationalDatabaseConnection>(
            b0, boost::fusion::pop_front(columns_of<book>()))),
        "UPDATE book SET author=45,series=NULL,copyright_year=2345,cost=1245,category=NULL WHERE title = 'Drive';");
}

BOOST_AUTO_TEST_CASE( update_some_test )
{
    book b0;
    b0.title = "Drive";
    b0.author = 45;
    b0.series.reset();
    b0.copyright_year = 2345;
    b0.cost = 1245;
    b0.category = boost::none;

    BOOST_CHECK_EQUAL(
        (sql::create_update_sql<concepts::RelationalDatabaseConnection>(
            b0, boost::fusion::make_vector(&book::author, &book::series, &book::cost))),
        "UPDATE book SET author=45,series=NULL,cost=1245 WHERE title = 'Drive';");
}
