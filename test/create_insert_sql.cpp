///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE cryo_create_insert_sql
#include <boost/test/included/unit_test.hpp>

#if defined(max)
#   undef max
#endif

#if defined(min)
#   undef min
#endif

#include <cryo/concept/relational_database_connection.hpp>
#include <cryo/sql/create_insert_sql.hpp>
#include "test_schema.hpp"

struct no_autogen_primary
{
    int prim;
    int col0;
    int col1;
};

CRYO_DECLARE_ENTITY(no_autogen_primary, prim, (col0)(col1));

using namespace cryo;
using namespace cryo::sql::dynamic;

BOOST_AUTO_TEST_CASE( no_autogen_insert_test )
{
    no_autogen_primary nap = {24, 34, 56};

    BOOST_CHECK_EQUAL(
        cryo::sql::create_insert_sql<concepts::RelationalDatabaseConnection>(nap),
        "INSERT INTO no_autogen_primary (prim,col0,col1) VALUES (24,34,56);");
}

BOOST_AUTO_TEST_CASE( simple_insert_test )
{
    author a0;
    a0.name = "Salman Rushdie";

    BOOST_CHECK_EQUAL(
        cryo::sql::create_insert_sql<concepts::RelationalDatabaseConnection>(a0),
        "INSERT INTO author (name) VALUES ('Salman Rushdie');");
}

BOOST_AUTO_TEST_CASE( foreign_entity_insert_test )
{
    author a0;
    a0.id = 56;
    a0.name = "Robert Jordan";

    series s0;
    s0.title = "Wheel of Time";
    s0.author = a0;

    BOOST_CHECK_EQUAL(
        cryo::sql::create_insert_sql<concepts::RelationalDatabaseConnection>(s0),
        "INSERT INTO series (title,author) VALUES ('Wheel of Time',56);");
}

BOOST_AUTO_TEST_CASE( insert_null_test )
{
    book b0;
    b0.title = "Drive";
    b0.author = 45;
    b0.series.reset();
    b0.copyright_year = 2345;
    b0.cost = 1245;
    b0.category = boost::none;

    BOOST_CHECK_EQUAL(
        cryo::sql::create_insert_sql<concepts::RelationalDatabaseConnection>(b0),
        "INSERT INTO book (title,author,series,copyright_year,cost,category) VALUES ('Drive',45,NULL,2345,1245,NULL);");
}
