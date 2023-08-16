///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE cryo_create_find_sql
#include <boost/test/included/unit_test.hpp>

#if defined(max)
#   undef max
#endif

#if defined(min)
#   undef min
#endif

#include "test_schema.hpp"
#include <cryo/concept/relational_database_connection.hpp>
#include <cryo/sql/create_find_sql.hpp>
#include <boost/fusion/include/make_vector.hpp>

using namespace cryo;
using namespace cryo::sql::dynamic;

#define CRYO_CHECK_QUERY_SQL(table, x, str) BOOST_CHECK_EQUAL(output_where_<table>(x), str)

typedef cryo::extension::concrete_hooks<concepts::RelationalDatabaseConnection> hooks_type;

template <typename Table>
inline std::string output_where_(condition const& x)
{
    return cryo::sql::create_find_sql<concepts::RelationalDatabaseConnection, Table>(x);
}

BOOST_AUTO_TEST_CASE( is_null_test )
{
    CRYO_CHECK_QUERY_SQL(author, col(&author::id) == null, "SELECT * FROM author t0 WHERE (t0.id IS NULL);");
}

BOOST_AUTO_TEST_CASE( is_not_null_test )
{
    CRYO_CHECK_QUERY_SQL(author, col(&author::id) != null, "SELECT * FROM author t0 WHERE (t0.id IS NOT NULL);");
}

BOOST_AUTO_TEST_CASE( conditional_ops_test )
{
    CRYO_CHECK_QUERY_SQL(author, col(&author::id) == 0, "SELECT * FROM author t0 WHERE (t0.id=0);");

    CRYO_CHECK_QUERY_SQL(author, col(&author::id) != 0, "SELECT * FROM author t0 WHERE (t0.id<>0);");

    CRYO_CHECK_QUERY_SQL(author, col(&author::id) < 0, "SELECT * FROM author t0 WHERE (t0.id<0);");

    CRYO_CHECK_QUERY_SQL(author, col(&author::id) > 0, "SELECT * FROM author t0 WHERE (t0.id>0);");

    CRYO_CHECK_QUERY_SQL(author, col(&author::id) <= 0, "SELECT * FROM author t0 WHERE (t0.id<=0);");

    CRYO_CHECK_QUERY_SQL(author, col(&author::id) >= 0, "SELECT * FROM author t0 WHERE (t0.id>=0);");
}

BOOST_AUTO_TEST_CASE( logical_ops_test )
{
    CRYO_CHECK_QUERY_SQL(author, col(&author::id) == 0 && col(&author::name) != "abc",
        "SELECT * FROM author t0 WHERE ((t0.id=0)AND(t0.name<>'abc'));");

    CRYO_CHECK_QUERY_SQL(
        author, col(&author::id) == 0 || col(&author::name) != "abc",
        "SELECT * FROM author t0 WHERE ((t0.id=0)OR(t0.name<>'abc'));");
}

BOOST_AUTO_TEST_CASE( arithmetic_ops_test )
{
    CRYO_CHECK_QUERY_SQL(author, (col(&author::id) + 12) == 0, "SELECT * FROM author t0 WHERE ((t0.id+12)=0);");

    CRYO_CHECK_QUERY_SQL(author, (col(&author::id) - 12) == 0, "SELECT * FROM author t0 WHERE ((t0.id-12)=0);");

    CRYO_CHECK_QUERY_SQL(author, (col(&author::id) / 12) == 0, "SELECT * FROM author t0 WHERE ((t0.id/12)=0);");

    CRYO_CHECK_QUERY_SQL(author, (col(&author::id) % 12) == 0, "SELECT * FROM author t0 WHERE ((t0.id%12)=0);");

    CRYO_CHECK_QUERY_SQL(author, (col(&author::id) | 12) == 0, "SELECT * FROM author t0 WHERE ((t0.id|12)=0);");

    CRYO_CHECK_QUERY_SQL(author, (col(&author::id) & 12) == 0, "SELECT * FROM author t0 WHERE ((t0.id&12)=0);");

    CRYO_CHECK_QUERY_SQL(author, (col(&author::id) ^ 12) == 0, "SELECT * FROM author t0 WHERE ((t0.id^12)=0);");

    CRYO_CHECK_QUERY_SQL(author, ~col(&author::id) == 0, "SELECT * FROM author t0 WHERE (~(t0.id)=0);");

    CRYO_CHECK_QUERY_SQL(author, -col(&author::id) == 0, "SELECT * FROM author t0 WHERE (-(t0.id)=0);");

    CRYO_CHECK_QUERY_SQL(author, (col(&author::id) >> 12) == 0, "SELECT * FROM author t0 WHERE ((t0.id>>12)=0);");

    CRYO_CHECK_QUERY_SQL(author, (col(&author::id) << 12) == 0, "SELECT * FROM author t0 WHERE ((t0.id<<12)=0);");
}

BOOST_AUTO_TEST_CASE( boolean_aggregate_test )
{
    CRYO_CHECK_QUERY_SQL(
        author,
        is_max(col(&author::id)) && col(&author::name) != null,
        "SELECT *, MAX(t0.id) FROM author t0 WHERE (t0.name IS NOT NULL);"
        );
}

BOOST_AUTO_TEST_CASE( joins_test )
{
    CRYO_CHECK_QUERY_SQL(
        series,
        col(&series::author)(&author::name) == "Piers Anthony",
        "SELECT t0.id,t0.title,t1.id,t1.name FROM series t0 INNER JOIN author t1 ON t1.id=t0.author WHERE (t1.name='Piers Anthony');"
        );

    CRYO_CHECK_QUERY_SQL(
        book,
        col(&book::title) == "abc" &&
        col(&book::series)(&series::title) == "def" &&
        col(&book::series)(&series::author)(&author::name) == "ghi",
        "SELECT t0.title,t0.author,t2.id,t2.title,t5.id,t5.name,t0.copyright_year,t0.cost,t3.name FROM book t0"
        " INNER JOIN author t1 ON t1.id=t0.author" // TODO: this join is unecessary, get rid of it. it is here due to using the foriefn_key decorator.
        " LEFT JOIN series t2 ON t2.id=t0.series"
        " LEFT JOIN author t4 ON t4.id=t2.author"
        " LEFT JOIN category t3 ON t3.name=t0.category"
        " LEFT JOIN author t5 ON t5.id=t2.author"
        " WHERE (((t0.title='abc')AND(t2.title='def'))AND(t4.name='ghi'));"
        );

    CRYO_CHECK_QUERY_SQL(
        book,
        col(&book::title) == col(&book::author)(&author::name),
        "SELECT t0.title,t0.author,t2.id,t2.title,t4.id,t4.name,t0.copyright_year,t0.cost,t3.name FROM book t0"
        " INNER JOIN author t1 ON t1.id=t0.author"
        " LEFT JOIN series t2 ON t2.id=t0.series"
        " LEFT JOIN category t3 ON t3.name=t0.category"
        " LEFT JOIN author t4 ON t4.id=t2.author"
        " WHERE (t0.title=t1.name);"
        );
}

BOOST_AUTO_TEST_CASE( select_primary_key_test )
{
    BOOST_CHECK_EQUAL(
        (cryo::sql::create_find_sql<concepts::RelationalDatabaseConnection, series>(1)),
        "SELECT t0.id,t0.title,t1.id,t1.name FROM series t0 INNER JOIN author t1 ON t1.id=t0.author WHERE t0.id=1;");

    BOOST_CHECK_EQUAL(
        (cryo::sql::create_find_sql<concepts::RelationalDatabaseConnection, author>(1)),
        "SELECT * FROM author WHERE id=1;");
}

BOOST_AUTO_TEST_CASE( select_foreign_entity_test )
{
    BOOST_CHECK_EQUAL(
        (cryo::sql::create_find_sql<concepts::RelationalDatabaseConnection, series>(
            1, boost::fusion::make_vector(col(&series::author)))), 
        "SELECT t1.id,t1.name FROM series t0 INNER JOIN author t1 ON t1.id=t0.author WHERE t0.id=1;");

    condition q = col(&series::author)(&author::name) == "who";
    BOOST_CHECK_EQUAL(
        (cryo::sql::create_find_sql<concepts::RelationalDatabaseConnection, author>(
            q, boost::fusion::make_vector(col(&series::author)))),
        "SELECT t1.id,t1.name FROM author t0 INNER JOIN author t1 ON t1.id=t0.author WHERE (t1.name='who');");
}
