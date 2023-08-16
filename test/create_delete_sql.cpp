///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#define BOOST_TEST_MODULE cryo_create_delete_sql
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

BOOST_AUTO_TEST_CASE( delete_test )
{
    BOOST_CHECK_EQUAL(
        (sql::create_delete_sql<concepts::RelationalDatabaseConnection, book>("the title")),
        "DELETE FROM book WHERE title='the title';"
        );
}
