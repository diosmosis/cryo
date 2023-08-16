///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( DATABASE_SUITE_HPP )
#define DATABASE_SUITE_HPP

#include <cryo/detail/contract_test_suite.hpp>
#include <cryo/concept/relational_database_connection.hpp>
#include <cryo/operation.hpp>
#include <cryo/error.hpp>
#include <ios>
#include "test_schema.hpp"

using namespace cryo;
using namespace cryo::sql::dynamic;

// uses test_schema
CONTRACT_TEST_SUITE( concepts::RelationalDatabaseConnection )
{
    boost::optional<author> author_entity1;
    boost::optional<series> series_entity1;
    boost::optional<book> book_entity1;

    boost::optional<author> author_entity2;
    boost::optional<series> series_entity2;
    boost::optional<book> book_entity2;

    boost::optional<category> fiction;

    boost::optional<blob_container> blob_container1;

    void make_author1(int id)
    {
        author_entity1 = author();
        author_entity1->id = id;
        author_entity1->name = "Robert Jordan";
    }

    void make_series1(int id)
    {
        series_entity1 = series();
        series_entity1->id = id;
        series_entity1->title = "Wheel of Time";
        series_entity1->author = author_entity1.get();
    }

    void make_book1()
    {
        if (!fiction)
        {
            create_categories();
        }

        book_entity1 = book();
        book_entity1->title = "Eye of the World";
        book_entity1->author = author_entity1->id;
        book_entity1->series.reset(new series(series_entity1.get()));
        book_entity1->copyright_year = 1990;
        book_entity1->cost = 12;
        book_entity1->category = fiction;
    }

    void make_author2(int id)
    {
        author_entity2 = author();
        author_entity2->id = -1;
        author_entity2->name = "Piers Anthony";
    }

    void make_series2(int id)
    {
        series_entity2 = series();
        series_entity2->id = id;
        series_entity2->title = "Apprentice Adept";
        series_entity2->author = author_entity2.get();
    }

    void make_book2()
    {
        if (!fiction)
        {
            create_categories();
        }

        book_entity2 = book();
        book_entity2->title = "Blue Adept";
        book_entity2->author = author_entity2->id;
        book_entity2->series.reset(new series(series_entity2.get()));
        book_entity2->copyright_year = 1981;
        book_entity2->cost = 10;
        book_entity2->category = fiction;
    }

    void make_book(std::string const& title, author const& a, series const& s, int copyright_year, unsigned int cost)
    {
        book entity;
        entity.title = title;
        entity.author = a.id;
        entity.series.reset(new series(s));
        entity.copyright_year = copyright_year;
        entity.cost = cost;
        entity.category = fiction;
        create(**this, entity);
    }

    void create_author1()
    {
        make_author1(-1);
        create(**this, author_entity1.get());
    }

    void create_series1()
    {
        make_series1(-1);
        create(**this, series_entity1.get());
    }

    void create_book1()
    {
        make_book1();
        create(**this, book_entity1.get());
    }

    void create_author2()
    {
        make_author2(-1);
        create(**this, author_entity2.get());
    }

    void create_series2()
    {
        make_series2(-1);
        create(**this, series_entity2.get());
    }

    void create_book2()
    {
        make_book2();
        create(**this, book_entity2.get());
    }

    void create_categories()
    {
        fiction = category();
        fiction->name = "fiction";
        create(**this, fiction.get());
    }

    void create_blob_container()
    {
        blob_container1 = blob_container();
        blob_container1->blob_col.a = 1;
        blob_container1->blob_col.b = 2.0f;
        blob_container1->blob_col.c = 3.0;
        blob_container1->blob_col.d = 4;
        create(**this, blob_container1.get());
    }

    void create_extra_books()
    {
        // create robert jordan books
        make_book("The Great Hunt", author_entity1.get(), series_entity1.get(), 1990, 7);
        make_book("The Dragon Reborn", author_entity1.get(), series_entity1.get(), 1991, 6);
        make_book("The Shadow Rising", author_entity1.get(), series_entity1.get(), 1992, 6);
        make_book("The Fires of Heaven", author_entity1.get(), series_entity1.get(), 1993, 7);
        make_book("Lord of Chaos", author_entity1.get(), series_entity1.get(), 1994, 8);
        make_book("A Crown of Swords", author_entity1.get(), series_entity1.get(), 1996, 12);
        make_book("The Path of Daggers", author_entity1.get(), series_entity1.get(), 1998, 22);

        // create piers anthony books
        make_book("A Spell for Chameleon", author_entity2.get(), series_entity2.get(), 1977, 4);
        make_book("The Source of Magic", author_entity2.get(), series_entity2.get(), 1979, 5);
        make_book("Castle Roogna", author_entity2.get(), series_entity2.get(), 1979, 5);
        make_book("Centaur Aisle", author_entity2.get(), series_entity2.get(), 1982, 6);
        make_book("Ogre, Ogre", author_entity2.get(), series_entity2.get(), 1982, 7);
        make_book("Night Mare", author_entity2.get(), series_entity2.get(), 1983, 12);
    }

    // tests autogenerate
    CONTRACT_TEST_CASE( create_find_count_accuracy )
    {
        // create author
        BOOST_TEST_PASSPOINT();
        create_author1();

        // check create, find & count ops
        BOOST_TEST_PASSPOINT();
        {
            BOOST_CHECK_MESSAGE( author_entity1->id != -1, "author id was not autogenerated" );

            author_entity1 = find<author>(**this, author_entity1->id);

            BOOST_CHECK_MESSAGE( author_entity1, "author entity was not added to persistence (or find failed)" );
            BOOST_CHECK_MESSAGE( cryo::count<author>(**this) == 1, "author entity was not added to persistence (or count failed)" );

            if (author_entity1)
            {
                BOOST_CHECK( author_entity1->name == "Robert Jordan" );
            }
        }

        // create series
        BOOST_TEST_PASSPOINT();
        create_series1();

        // check create, find & count ops
        BOOST_TEST_PASSPOINT();
        {
            BOOST_CHECK_MESSAGE( series_entity1->id != -1, "series id was not autogenerated" );

            series_entity1 = find<series>(**this, series_entity1->id);
            BOOST_CHECK_MESSAGE( series_entity1, "series entity was not added to persistence (or find failed)" );
            BOOST_CHECK_MESSAGE( cryo::count<series>(**this) == 1, "series entity was not added to persistence (or count failed)" );

            if (series_entity1)
            {
                BOOST_CHECK( series_entity1->title == "Wheel of Time" );
                BOOST_CHECK( series_entity1->author.id == author_entity1->id );
                BOOST_CHECK( series_entity1->author.name == author_entity1->name );
            }
        }

        // create book
        BOOST_TEST_PASSPOINT();
        create_book1();

        // check create, find & count ops
        BOOST_TEST_PASSPOINT();
        {
            book_entity1 = find<book>(**this, book_entity1->title);
            BOOST_CHECK_MESSAGE( book_entity1, "book entity was not added to persistence (or find failed)" );
            BOOST_CHECK_MESSAGE( cryo::count<book>(**this) == 1, "book entity was not added to persistence (or count failed)" );

            if (book_entity1)
            {
                BOOST_CHECK( book_entity1->title == "Eye of the World" );
                BOOST_CHECK( book_entity1->author == author_entity1->id );

                BOOST_CHECK( book_entity1->series );
                if (book_entity1->series)
                {
                    BOOST_CHECK( book_entity1->series->id == series_entity1->id );
                    BOOST_CHECK( book_entity1->series->title == series_entity1->title );
                    BOOST_CHECK( book_entity1->series->author.id == series_entity1->author.id );
                    BOOST_CHECK( book_entity1->series->author.name == series_entity1->author.name );
                }

                BOOST_CHECK( book_entity1->category );
                if (book_entity1->category)
                {
                    BOOST_CHECK( book_entity1->category->name == fiction->name );
                }

                BOOST_CHECK( book_entity1->copyright_year == 1990 );
            }
        }
    }

    CONTRACT_TEST_CASE( update_find_accuracy )
    {
        // create author
        BOOST_TEST_PASSPOINT();
        create_author1();

        // update author
        BOOST_TEST_PASSPOINT();
        {
            author_entity1->name = "changed";
            update(**this, author_entity1.get());
        }

        // check update, find
        BOOST_TEST_PASSPOINT();
        {
            author_entity1 = find<author>(**this, author_entity1->id);
            BOOST_CHECK_MESSAGE( author_entity1, "could not find book entity" );

            if (author_entity1)
            {
                BOOST_CHECK( author_entity1->name == "changed" );
            }
        }

        // series
        BOOST_TEST_PASSPOINT();
        create_author2();
        create_series1();

        // update series
        BOOST_TEST_PASSPOINT();
        {
            series_entity1->title = "changed";
            series_entity1->author = author_entity2.get();
            update(**this, series_entity1.get());
        }

        // check update, find
        BOOST_TEST_PASSPOINT();
        {
            series_entity1 = find<series>(**this, series_entity1->id);
            BOOST_CHECK_MESSAGE( series_entity1, "could not find series entity" );

            if (series_entity1)
            {
                BOOST_CHECK( series_entity1->title == "changed" );
                BOOST_CHECK( series_entity1->author.id == author_entity2->id );
            }
        }

        // book
        BOOST_TEST_PASSPOINT();
        create_book1();
        create_series2();

        // update book
        BOOST_TEST_PASSPOINT();
        {
            book_entity1->author = author_entity2->id;
            book_entity1->series.reset(new series(series_entity2.get()));
            update(**this, book_entity1.get());
        }

        // check update, find
        BOOST_TEST_PASSPOINT();
        {
            book_entity1 = find<book>(**this, book_entity1->title);
            BOOST_CHECK_MESSAGE( book_entity1, "could not find book entity" );

            if (book_entity1)
            {
                BOOST_CHECK( book_entity1->author == author_entity2->id );
                BOOST_CHECK( book_entity1->series && book_entity1->series->id == series_entity2->id );
            }
        }
    }

    // make sure its possible to update only certain columns
    CONTRACT_TEST_CASE( update_custom_accuracy_0 )
    {
        // create the entities used
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_author2();

        // modify the title and author of series1, then update only the series
        BOOST_TEST_PASSPOINT();
        series_entity1->title = "New Title";
        series_entity1->author = author_entity2.get();
        update(**this, series_entity1.get(), &series::title);

        // find the entity & make sure the title changed, but the author didn't
        BOOST_TEST_PASSPOINT();
        {
            boost::optional<series> updated_series = find<series>(**this, series_entity1->id);

            BOOST_CHECK_MESSAGE( updated_series, "could not find series entity" );

            if (updated_series)
            {
                BOOST_CHECK( updated_series->title == "New Title" );
                BOOST_CHECK( updated_series->author.id == author_entity1->id );
            }
        }
    }

    // make sure update fails when given an invalid id
    CONTRACT_TEST_CASE( update_failure_0 )
    {
        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();

        // test author entity
        BOOST_TEST_PASSPOINT();
        {
            author invalid;
            invalid.id = -1;

            try
            {
                update(**this, invalid);
                BOOST_CHECK_MESSAGE( false, "updating author with invalid id did not throw" );
            }
            catch (cryo::error const&)
            {
                // ignore
            }
        }

        // test series entity
        BOOST_TEST_PASSPOINT();
        {
            series invalid;
            invalid.id = -1;

            try
            {
                update(**this, invalid);
                BOOST_CHECK_MESSAGE( false, "updating series with invalid id did not throw" );
            }
            catch (cryo::error const&)
            {
                // ignore
            }
        }

        // test book entity
        BOOST_TEST_PASSPOINT();
        {
            book invalid;
            invalid.title = "-----";

            try
            {
                update(**this, invalid);
                BOOST_CHECK_MESSAGE( false, "updating book with invalid id did not throw" );
            }
            catch (cryo::error const&)
            {
                // ignore
            }
        }
    }

    CONTRACT_TEST_CASE( delete_find_count_accuracy_0 )
    {
        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();

        // delete book
        BOOST_TEST_PASSPOINT();
        delete_(**this, book_entity1.get());

        // check delete, find, count
        BOOST_TEST_PASSPOINT();
        BOOST_CHECK( !find<book>(**this, book_entity1->title) );

        // delete series
        BOOST_TEST_PASSPOINT();
        delete_(**this, series_entity1.get());

        // check delete, find, count
        BOOST_TEST_PASSPOINT();
        BOOST_CHECK( !find<series>(**this, series_entity1->id) );

        // delete author
        BOOST_TEST_PASSPOINT();
        delete_(**this, author_entity1.get());

        // check delete, find, count
        BOOST_TEST_PASSPOINT();
        BOOST_CHECK( !find<author>(**this, author_entity1->id) );
    }

    CONTRACT_TEST_CASE( delete_find_count_accuracy_1 )
    {
        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();

        // delete book
        BOOST_TEST_PASSPOINT();
        delete_<book>(**this, book_entity1->title);

        // check delete, find, count
        BOOST_TEST_PASSPOINT();
        BOOST_CHECK( !find<book>(**this, book_entity1->title) );

        // delete series
        BOOST_TEST_PASSPOINT();
        delete_<series>(**this, series_entity1->id);

        // check delete, find, count
        BOOST_TEST_PASSPOINT();
        BOOST_CHECK( !find<series>(**this, series_entity1->id) );

        // delete author
        BOOST_TEST_PASSPOINT();
        delete_<author>(**this, author_entity1->id);

        // check delete, find, count
        BOOST_TEST_PASSPOINT();
        BOOST_CHECK( !find<author>(**this, author_entity1->id) );
    }

    // make sure delete fails when given an invalid id
    CONTRACT_TEST_CASE( delete_failure_0 )
    {
        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();

        // try and delete an invalid entity
        BOOST_TEST_PASSPOINT();
        {
            author invalid;
            invalid.id = -1;
            try
            {
                delete_(**this, invalid);
                BOOST_CHECK_MESSAGE( false, "deleting author with invalid id did not throw" );
            }
            catch (cryo::error const&)
            {
                // ignore
            }
        }

        // try and delete an invalid entity
        BOOST_TEST_PASSPOINT();
        {
            series invalid;
            invalid.id = -1;
            try
            {
                delete_(**this, invalid);
                BOOST_CHECK_MESSAGE( false, "deleting series with invalid id did not throw" );
            }
            catch (cryo::error const&)
            {
                // ignore
            }
        }

        // try and delete an invalid entity
        BOOST_TEST_PASSPOINT();
        {
            book invalid;
            invalid.title = "-----";
            try
            {
                delete_(**this, invalid);
                BOOST_CHECK_MESSAGE( false, "deleting book with invalid title did not throw" );
            }
            catch (cryo::error const&)
            {
                // ignore
            }
        }
    }

    // make sure delete fails when given a constraint was violated
    CONTRACT_TEST_CASE( delete_failure_1 )
    {
        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();

        // try deleting author
        BOOST_TEST_PASSPOINT();
        try
        {
            delete_(**this, author_entity1.get());
            BOOST_CHECK_MESSAGE( false, "deleting an author that was referenced by another entity did not throw" );
        }
        catch (cryo::error const&)
        {
            // ignore
        }

        // try deleting series
        BOOST_TEST_PASSPOINT();
        try
        {
            delete_(**this, series_entity1.get());
            BOOST_CHECK_MESSAGE( false, "deleting a series that was referenced by another entity did not throw" );
        }
        catch (cryo::error const&)
        {
            // ignore
        }
    }

    // test that nothing is returned in an empty db
    CONTRACT_TEST_CASE( find_key_accuracy_0 )
    {
        BOOST_CHECK( !find<author>(**this, 1) );
        BOOST_CHECK( !find<series>(**this, 1) );
        BOOST_CHECK( !find<book>(**this, "title") );
    }

    CONTRACT_TEST_CASE( find_query_accuracy_0 )
    {
        using namespace cryo;

        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();
        create_author2();
        create_series2();
        create_book2();

        // find book 1
        BOOST_TEST_PASSPOINT();
        {
            condition q = col(&book::title) == "Eye of the World"
                   && col(&book::author) == author_entity1->id
                   && col(&book::series) == series_entity1->id;

            typename result_set<book, typename T::cursor_type>::iterator f, l;
            result_set<book, typename T::cursor_type> rs = find<book>(**this, q);

            BOOST_TEST_PASSPOINT();
            f = rs.begin();
            l = rs.end();

            BOOST_CHECK_MESSAGE( f != l, "query returned no results" );

            book entity = *f;
            ++f;

            BOOST_CHECK( entity.title == book_entity1->title );

            BOOST_CHECK_MESSAGE( f == l, "query returned more than one result" );
        }

        // find both books
        BOOST_TEST_PASSPOINT();
        {
            condition q = col(&book::title) == "Eye of the World" || col(&book::title) == "Blue Adept";

            typename result_set<book, typename T::cursor_type>::iterator f, l;
            result_set<book, typename T::cursor_type> rs = find<book>(**this, q);

            BOOST_TEST_PASSPOINT();
            f = rs.begin();
            l = rs.end();

            BOOST_CHECK_MESSAGE( f != l, "query returned no results" );

            book entity = *f;
            ++f;

            BOOST_CHECK( entity.title == book_entity1->title || entity.title == book_entity2->title );

            BOOST_CHECK_MESSAGE( f != l, "query only returned one result" );

            entity = *f;
            ++f;

            BOOST_CHECK( entity.title == book_entity1->title || entity.title == book_entity2->title );

            BOOST_CHECK_MESSAGE( f == l, "query returned more than two results" );
        }
    }

    CONTRACT_TEST_CASE( find_query_accuracy_1 )
    {
        using namespace cryo;

        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();
        create_author2();
        create_series2();
        create_book2();

        BOOST_TEST_PASSPOINT();
        condition q = col(&book::title) == "Eye of the World" && col(&book::title) != "Eye of the World";

        typename result_set<book, typename T::cursor_type>::iterator f, l;
        result_set<book, typename T::cursor_type> rs = find<book>(**this, q);

        BOOST_TEST_PASSPOINT();
        f = rs.begin();
        l = rs.end();

        BOOST_CHECK_MESSAGE( f == l, "query did not return an empty result set" );
    }

    // test that count returns 0 when it should
    CONTRACT_TEST_CASE( count_accuracy )
    {
        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_author2();

        BOOST_CHECK( cryo::count<series>(**this) == 0 );
        BOOST_CHECK( cryo::count<author>(**this) == 2 );
    }

    CONTRACT_TEST_CASE( count_query_accuracy_0 )
    {
        using namespace cryo;

        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();
        create_author2();
        create_series2();
        create_book2();

        // check count
        BOOST_TEST_PASSPOINT();
        {
            condition q = col(&book::title) == "Eye of the World"
                   && col(&book::author) == author_entity1->id
                   && col(&book::series) == series_entity1->id;

            BOOST_CHECK( cryo::count<book>(**this, q) == 1 );
        }

        // check count
        BOOST_TEST_PASSPOINT();
        {
            condition q = col(&book::title) == "Eye of the World" || col(&book::title) == "Blue Adept";

            BOOST_CHECK( cryo::count<book>(**this, q) == 2 );
        }
    }

    CONTRACT_TEST_CASE( count_query_accuracy_1 )
    {
        using namespace cryo;

        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();
        create_author2();
        create_series2();
        create_book2();

        // check count
        BOOST_TEST_PASSPOINT();
        condition q = col(&book::title) == "Eye of the World" && col(&book::title) != "Eye of the World";

        BOOST_CHECK( cryo::count<book>(**this, q) == 0 );
    }

    CONTRACT_TEST_CASE( find_all_accuracy_0 )
    {
        // create entities
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_author2();

        // find all authors and check results
        typename result_set<author, typename T::cursor_type>::iterator f, l;
        result_set<author, typename T::cursor_type> rs = find_all<author>(**this);

        BOOST_TEST_PASSPOINT();
        f = rs.begin();
        l = rs.end();

        BOOST_CHECK_MESSAGE( f != l, "find_all returned no results" );

        author entity = *f;
        ++f;

        BOOST_CHECK( entity.id == author_entity1->id || entity.id == author_entity2->id );

        BOOST_CHECK_MESSAGE( f != l, "find_all only returned one result" );

        entity = *f;
        ++f;

        BOOST_CHECK( entity.id == author_entity1->id || entity.id == author_entity2->id );

        BOOST_CHECK_MESSAGE( f == l, "find_all returned more than two results" );
    }

    CONTRACT_TEST_CASE( find_all_accuracy_1 )
    {
        BOOST_TEST_PASSPOINT();

        // find all authors and check results
        typename result_set<author, typename T::cursor_type>::iterator f, l;
        result_set<author, typename T::cursor_type> rs = find_all<author>(**this);

        BOOST_TEST_PASSPOINT();
        f = rs.begin();
        l = rs.end();

        BOOST_CHECK_MESSAGE( f == l, "find_all returned results with an empty database" );
    }

    CONTRACT_TEST_CASE( contains_accuracy_0 )
    {
        BOOST_TEST_PASSPOINT();

        // fill database
        create_author1();
        create_series1();
        create_book1();

        // perform checks
        BOOST_CHECK( contains<author>(**this, author_entity1->id) );
        BOOST_CHECK( contains<series>(**this, series_entity1->id) );
        BOOST_CHECK( contains<book>(**this, book_entity1->title) );

        BOOST_CHECK( contains(**this, author_entity1.get()) );
        BOOST_CHECK( contains(**this, series_entity1.get()) );
        BOOST_CHECK( contains(**this, book_entity1.get()) );
    }

    CONTRACT_TEST_CASE( contains_accuracy_1 )
    {
        BOOST_TEST_PASSPOINT();

        // fill database
        create_author1();
        create_series1();
        create_book1();

        // find an id that does not exist in the DB
        int invalid_id = -1;
        if (invalid_id == author_entity1->id)
        {
            --invalid_id;
        }
        if (invalid_id == series_entity1->id)
        {
            --invalid_id;
        }

        // fill the other entities but don't put them in the DB
        BOOST_TEST_PASSPOINT();
        make_author2(invalid_id);
        make_series2(invalid_id);
        make_book2();

        // perform checks
        BOOST_CHECK( !contains<author>(**this, invalid_id) );
        BOOST_CHECK( !contains<series>(**this, invalid_id) );
        BOOST_CHECK( !contains<book>(**this, invalid_id) );

        BOOST_CHECK( !contains(**this, author_entity2.get()) );
        BOOST_CHECK( !contains(**this, series_entity2.get()) );
        BOOST_CHECK( !contains(**this, book_entity2.get()) );
    }

    CONTRACT_TEST_CASE( shared_ptr_null_accuracy_0 )
    {
        BOOST_TEST_PASSPOINT();

        // fill database
        create_author1();
        create_series1();
        create_book1();

        // set the series to NULL and update
        BOOST_TEST_PASSPOINT();
        book_entity1->series.reset();
        update(**this, book_entity1.get());

        // make sure the shared_ptr reference can be set to null
        BOOST_TEST_PASSPOINT();
        {
            book_entity1 = find<book>(**this, book_entity1->title);
            BOOST_CHECK_MESSAGE( book_entity1, "could not find book entity" );

            if (book_entity1)
            {
                BOOST_CHECK( !book_entity1->series );
            }
        }
    }

    CONTRACT_TEST_CASE( optional_null_accuracy_0 )
    {
        BOOST_TEST_PASSPOINT();

        // fill database
        create_author1();
        create_series1();
        create_book1();

        // set copyright year to NULL and update
        BOOST_TEST_PASSPOINT();
        book_entity1->copyright_year = boost::none;
        update(**this, book_entity1.get());

        // make sure the optional field can be set to null
        BOOST_TEST_PASSPOINT();
        {
            book_entity1 = find<book>(**this, book_entity1->title);
            BOOST_CHECK_MESSAGE( book_entity1, "could not find book entity" );

            if (book_entity1)
            {
                BOOST_CHECK( book_entity1->copyright_year == boost::none );
            }
        }
    }

    CONTRACT_TEST_CASE( find_partial_accuracy_0 )
    {
        using namespace cryo;

        // fill database
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();

        // find the author & series columns of the created book
        BOOST_TEST_PASSPOINT();
        boost::optional<
            boost::fusion::vector<int, boost::optional<series> >
        > found_book = find<book>(**this, col(&book::author), col(&book::series), book_entity1->title);

        BOOST_CHECK_MESSAGE( found_book, "could not find the created book" );

        if (found_book)
        {
            BOOST_CHECK( boost::fusion::at_c<0>(found_book.get()) == author_entity1->id );
            BOOST_CHECK( boost::fusion::at_c<1>(found_book.get()) &&
                         boost::fusion::at_c<1>(found_book.get())->id == series_entity1->id );
        }
    }

    CONTRACT_TEST_CASE( find_partial_accuracy_1 )
    {
        using namespace cryo;

        typedef boost::fusion::vector<int, boost::optional<series> > result_type;
        typedef result_set<result_type, typename T::cursor_type> result_set_type;

        // fill database
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();

        // create the query to be used
        BOOST_TEST_PASSPOINT();
        condition q = col(&book::title) == book_entity1->title;

        // find the author & series columns of the created book
        BOOST_TEST_PASSPOINT();
        result_set_type rs = find<book>(**this, col(&book::author), col(&book::series), q);

        typename result_set_type::iterator i = rs.begin();

        BOOST_REQUIRE_MESSAGE( i != rs.end(), "could not find any book entities" );

        result_type found_book = *i;

        BOOST_CHECK( boost::fusion::at_c<0>(found_book) == author_entity1->id );
        BOOST_CHECK( boost::fusion::at_c<1>(found_book) &&
                     boost::fusion::at_c<1>(found_book)->id == series_entity1->id );
    }

    CONTRACT_TEST_CASE( find_all_partial_accuracy_0 )
    {
        using namespace cryo;

        typedef result_set<boost::fusion::vector<std::string>, typename T::cursor_type> result_set_type;

        // fill database
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_author2();

        // select every author's name
        BOOST_TEST_PASSPOINT();
        result_set_type rs = find_all<author>(**this, col(&author::name));

        typename result_set_type::iterator i = rs.begin();

        // check that two authors were selected
        BOOST_REQUIRE_MESSAGE( i != rs.end(), "found no authors" );

        BOOST_TEST_PASSPOINT();
        boost::fusion::vector<std::string> item = *i;

        BOOST_CHECK( boost::fusion::at_c<0>(item) == author_entity1->name );

        // second author
        BOOST_TEST_PASSPOINT();
        ++i;

        BOOST_REQUIRE_MESSAGE( i != rs.end(), "found only one author" );

        BOOST_TEST_PASSPOINT();
        item = *i;

        BOOST_CHECK( boost::fusion::at_c<0>(item) == author_entity2->name );

        // end of the line
        BOOST_TEST_PASSPOINT();
        ++i;

        BOOST_CHECK_MESSAGE( i == rs.end(), "found more than one author" );
    }

    CONTRACT_TEST_CASE( result_set_bool_convert_accuracy_0 )
    {
        using namespace cryo;

        // create other database instance (the 'false' is to tell the fixture NOT to clear the test db)
        BOOST_TEST_PASSPOINT();
        boost::scoped_ptr<F> other(new F(false));

        // create the query
        BOOST_TEST_PASSPOINT();
        condition q = col(&book::title) == "Eye of the World" && col(&book::title) != "Eye of the World";

        // get the result set
        BOOST_TEST_PASSPOINT();
        result_set<book, typename T::cursor_type> rs = find<book>(other->concrete_type(), q);

        // check that the result_set is valid
        BOOST_CHECK(rs);

        // destroy the database instance & check that the result_set is rendered invalid
        other.reset(0);
        BOOST_CHECK(!rs);
    }

    CONTRACT_TEST_CASE( transaction_accuracy_0 )
    {
        typedef typename T::transaction_scope_type transaction_type;

        // fill database
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();

        // start transaction
        {
            BOOST_TEST_PASSPOINT();
            transaction_type txn(**this);

            // find the book whose title is 'Eye of the World'
            BOOST_TEST_PASSPOINT();
            book_entity1 = find<book>(**this, "Eye of the World");

            // change the copyright_year & update
            BOOST_TEST_PASSPOINT();
            book_entity1->copyright_year = -1;
            update(**this, book_entity1.get());

            // commit the transaction
            BOOST_TEST_PASSPOINT();
            txn.commit();
        }

        // find the book again & make sure the copyright yearbb's changed
        BOOST_TEST_PASSPOINT();
        book_entity1 = find<book>(**this, "Eye of the World");
        BOOST_CHECK(book_entity1->copyright_year && book_entity1->copyright_year.get() == -1);
    }

    CONTRACT_TEST_CASE( transaction_failure_0 )
    {
        typedef typename T::transaction_scope_type transaction_type;

        // fill database
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();

        // start transaction
        {
            BOOST_TEST_PASSPOINT();
            transaction_type txn(**this);

            // find the book whose title is 'Eye of the World'
            BOOST_TEST_PASSPOINT();
            book_entity1 = find<book>(**this, "Eye of the World");

            // change the copyright_year & update
            BOOST_TEST_PASSPOINT();
            book_entity1->copyright_year = -1;
            update(**this, book_entity1.get());

            // no commit; should implicitly rollback
            BOOST_TEST_PASSPOINT();
        }

        // find the book again & make sure the copyright year has not changed
        BOOST_TEST_PASSPOINT();
        book_entity1 = find<book>(**this, "Eye of the World");
        BOOST_CHECK(book_entity1->copyright_year && book_entity1->copyright_year.get() == 1990);
    }

    CONTRACT_TEST_CASE( transaction_failure_1 )
    {
        typedef typename T::transaction_scope_type transaction_type;

        // fill database
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();

        // start transaction
        try
        {
            BOOST_TEST_PASSPOINT();
            transaction_type txn(**this);

            // find the book whose title is 'Eye of the World'
            BOOST_TEST_PASSPOINT();
            book_entity1 = find<book>(**this, "Eye of the World");

            // change the copyright_year & update
            BOOST_TEST_PASSPOINT();
            book_entity1->copyright_year = -1;
            update(**this, book_entity1.get());

            // try to change the author to reference a non-existant entity (will throw)
            BOOST_TEST_PASSPOINT();
            book_entity1->author = 999;
            update(**this, book_entity1.get());

            // commit (this code should not be executed)
            txn.commit();

            BOOST_CHECK_MESSAGE(false, "updating w/ constraint violation failed to throw");
        }
        catch (...)
        {
            // ignore
        }

        // find the book again & make sure the copyright year has not changed
        BOOST_TEST_PASSPOINT();
        book_entity1 = find<book>(**this, "Eye of the World");
        BOOST_CHECK(book_entity1->copyright_year && book_entity1->copyright_year.get() == 1990);
    }

    template <typename ResultVector>
    void check_group_by_result(ResultVector const& rv, int n)
    {
        double const robert_jordan_avg_cost = 10.0;
        double const piers_anthony_avg_cost = 7.0;

        if (boost::fusion::at_c<1>(rv) == "Robert Jordan")
        {
            BOOST_CHECK_MESSAGE(
                boost::fusion::at_c<0>(rv) == author_entity1->id, "result" << n << " has an invalid id");

            BOOST_CHECK_MESSAGE(boost::fusion::at_c<2>(rv), "average returned null");
            if (boost::fusion::at_c<2>(rv))
            {
                BOOST_CHECK_MESSAGE(
                    boost::fusion::at_c<2>(rv).get() == robert_jordan_avg_cost,
                    "result" << n << " returned an invalid average cost");
            }
        }
        else if (boost::fusion::at_c<1>(rv) == "Piers Anthony")
        {
            BOOST_CHECK_MESSAGE(
                boost::fusion::at_c<0>(rv) == author_entity2->id, "result" << n << " has an invalid id");

            BOOST_CHECK_MESSAGE(boost::fusion::at_c<2>(rv), "average returned null");
            if (boost::fusion::at_c<2>(rv))
            {
                BOOST_CHECK_MESSAGE(
                    boost::fusion::at_c<2>(rv).get() == piers_anthony_avg_cost,
                    "result" << n << " returned an invalid average cost");
            }
        }
        else
        {
            BOOST_CHECK_MESSAGE(false, "result" << n << " has an invalid author name");
        }
    }

    CONTRACT_TEST_CASE( group_by_accuracy_0 )
    {
        // fill db
        BOOST_TEST_PASSPOINT();
        create_author1();
        create_series1();
        create_book1();
        create_author2();
        create_series2();
        create_book2();
        create_extra_books();

        typedef boost::fusion::vector<int, std::string, boost::optional<double> > result_vector;
        typedef result_set<result_vector, typename T::cursor_type> result_set_type;

        result_vector result0;
        result_vector result1;

        {
            // find the average cost of books for each author
            result_set_type rs = find_all<book>(
                **this, group_by(&book::author), group_by(&book::author)(&author::name), avg(col(&book::cost)));

            typename result_set_type::iterator
                f = rs.begin(),
                l = rs.end();

            BOOST_REQUIRE_MESSAGE(f != l, "query returned no results");

            result0 = *f;
            ++f;

            BOOST_REQUIRE_MESSAGE(f != l, "query only returned one result");

            result1 = *f;
            ++f;

            BOOST_CHECK_MESSAGE(f == l, "query returned more than two results");
        }

        // check that the results are for both authors
        BOOST_CHECK(boost::fusion::at_c<1>(result0) != boost::fusion::at_c<1>(result1));

        // check the results
        check_group_by_result(result0, 0);
        check_group_by_result(result1, 1);
    }

    CONTRACT_TEST_CASE( blob_insert_find_accuracy_0 )
    {
        // create the blobbed entity
        BOOST_TEST_PASSPOINT();
        create_blob_container();

        // find the blobbed entity
        BOOST_TEST_PASSPOINT();
        boost::optional<blob_container> updated = find<blob_container>(**this, blob_container1->id);

        // check that the blob data is correct
        BOOST_REQUIRE(updated);
        BOOST_CHECK(blob_container1->blob_col.a == 1);
        BOOST_CHECK(blob_container1->blob_col.b == 2.0f);
        BOOST_CHECK(blob_container1->blob_col.c == 3.0);
        BOOST_CHECK(blob_container1->blob_col.d == 4);
    }

    CONTRACT_TEST_CASE( blob_update_find_accuracy_0 )
    {
        // create the blobbed entity
        BOOST_TEST_PASSPOINT();
        create_blob_container();

        // modify the blobbed entity
        BOOST_TEST_PASSPOINT();
        blob_container1->blob_col.a = 5;
        blob_container1->blob_col.b = 6.0f;
        blob_container1->blob_col.c = 7.0;
        blob_container1->blob_col.d = 8;

        // update the entity
        BOOST_TEST_PASSPOINT();
        update(**this, blob_container1.get());

        // find the blobbed entity
        BOOST_TEST_PASSPOINT();
        boost::optional<blob_container> updated = find<blob_container>(**this, blob_container1->id);

        // check that the blob data is correct
        BOOST_REQUIRE(updated);
        BOOST_CHECK(updated->blob_col.a == 5);
        BOOST_CHECK(updated->blob_col.b == 6.0f);
        BOOST_CHECK(updated->blob_col.c == 7.0);
        BOOST_CHECK(updated->blob_col.d == 8);
    }

    /*CONTRACT_TEST_CASE( blob_stream_accuracy_0 )
    {
        unsigned char const data[10] = { 0, 12, 45, 123, 23, 234, 42, 78, 90, 240 };

        // create the blobbed entity
        BOOST_TEST_PASSPOINT();
        create_blob_container();

        {
            // create a blob output stream
            BOOST_TEST_PASSPOINT();
            typename T::blob_ostream stream(**this, *blob_container1, &blob_container::blob_col2);

            // write to the stream
            BOOST_TEST_PASSPOINT();
            stream.write(data, 10);
        }

        {
            // create a blob input stream
            BOOST_TEST_PASSPOINT();
            typename T::blob_istream stream(**this, *blob_container1, &blob_container::blob_col2);

            // read from the stream
            BOOST_TEST_PASSPOINT();
            unsigned char input[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            stream.read(input, 10);

            // check that the data is the same
            BOOST_CHECK(std::equal(
                static_cast<unsigned char const*>(data), data + 10, static_cast<unsigned char const*>(input)));
        }

        {
            unsigned char const new_data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

            // create a blob input/output stream
            BOOST_TEST_PASSPOINT();
            typename T::blob_iostream stream(**this, *blob_container1, &blob_container::blob_col2);

            // rewrite the data that has already been written
            BOOST_TEST_PASSPOINT();
            stream.write(data, 10);

            // seek to the beginning
            BOOST_TEST_PASSPOINT();
            stream.seekg(0, std::ios::beg);

            // read from the stream
            BOOST_TEST_PASSPOINT();
            unsigned char input[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            stream.read(input, 10);

            // check that the data is the same
            BOOST_CHECK(std::equal(
                static_cast<unsigned char const*>(new_data), new_data + 10, static_cast<unsigned char const*>(input)));
        }
    }*/
};

#endif // #if !defined( DATABASE_SUITE_HPP )
