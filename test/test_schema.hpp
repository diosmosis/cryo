///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( TEST_SCHEMA_HPP )
#define TEST_SCHEMA_HPP

#include <cryo/cryo.hpp>
#include <boost/mpl/vector.hpp>
#include <string>

using namespace cryo;

// NOTE: The 'entities' namespace is used so column names can be the same as entity types on GCC. This will work
// w/o the namespace on MSVC.

// author entity
namespace entities
{
    struct author_type
    {
        autogenerate<int>::type id;
        std::string name;
    };
}

typedef entities::author_type author;

CRYO_DECLARE_ENTITY(author, id, (name));

// series entity
namespace entities
{
    struct series_type
    {
        autogenerate<int>::type id;
        std::string title;
        author_type author;
    };
}

typedef entities::series_type series;

CRYO_DECLARE_ENTITY(series, id, (title)(author));

// category entity
namespace entities
{
    struct category_type
    {
        varchar<std::string, 32>::type name;
    };
}

typedef entities::category_type category;

CRYO_DECLARE_ENTITY_SINGLE(category, name);

// book entity
namespace entities
{
    struct book_type
    {
        varchar<std::string, 128>::type title;
        foreign_key<author_type>::type author;
        boost::shared_ptr<series_type> series;

        boost::optional<int> copyright_year;

        unsigned int cost;

        boost::optional<category_type> category;
    };
}

typedef entities::book_type book;

CRYO_DECLARE_ENTITY(book, title, (author)(series)(copyright_year)(cost)(category));

// blob container
struct extra_data
{
    int a;
    float b;
    double c;
    int d;
};

struct blob_container
{
    autogenerate<int>::type id;
    blob<extra_data>::type blob_col;
    blob_reference blob_col2;
};

CRYO_DECLARE_ENTITY(blob_container, id, (blob_col)(blob_col2));

// date container (quick test for date/datetime wrappers)
struct date_container
{
    autogenerate<int>::type id;

    datetime<boost::uint64_t>::type dt_int;
    date<boost::uint32_t>::type d_int;

    datetime<std::string>::type dt_str;
    date<std::string>::type d_str;
};

CRYO_DECLARE_ENTITY(date_container, id, (dt_int)(d_int)(dt_str)(d_str));

// the test schema
typedef boost::mpl::vector<category, author, series, book, blob_container, date_container> test_schema;

#endif // #if !defined( TEST_SCHEMA_HPP )
