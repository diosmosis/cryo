///////////////////////////////////////////////////////////////////////////////
//
//! \file create.hpp
//! Contains the \ref create operation.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_OPERATION_CREATE_HPP )
#define CRYO_OPERATION_CREATE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/relational/execute_query.hpp>
#include <cryo/relational/execute_command.hpp>
#include <cryo/schema/introspect/primary_key_of.hpp>
#include <cryo/schema/columns_view.hpp>
#include <cryo/schema/tag/autogenerate.hpp>
#include <cryo/sql/create_insert_sql.hpp>
#include <cryo/sql/create_find_sql.hpp>
#include <cryo/detail/fusion_copy.hpp>
#include <cryo/detail/make_member_ptr.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/filter_view.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/utility/enable_if.hpp>

// TODO: Need a way to convert result_set's w/ one type of entity to another. ie, two
// equivalent vectors.
namespace cryo
{
    namespace detail
    {
        namespace result_of
        {
            template <typename Entity>
            struct autogenerate_columns_of
            {
                typedef boost::fusion::filter_view<
                    typename boost::mpl::if_<
                        boost::is_const<Entity>,
                        columns_view<Entity> const,
                        columns_view<Entity>
                    >::type,
                    is_autogenerate<detail::make_member_ptr<boost::mpl::_1> >
                > type;
            };
        }

        // TODO: as done here, need tighter integration w/ fusion
        template <typename Entity>
        inline typename result_of::autogenerate_columns_of<Entity>::type autogenerate_columns_of(Entity & entity)
        {
            typedef typename result_of::autogenerate_columns_of<Entity>::type result_type;
            return result_type(columns_view<Entity>(entity));
        }

        template <typename Entity>
        struct has_multiple_auto_columns
            : boost::mpl::greater<
                boost::mpl::size<
                    typename result_of::autogenerate_columns_of<Entity>::type
                >,
                boost::mpl::int_<1>
            >
        {};

        template <typename T>
        struct has_auto_primary_key
            : is_autogenerate<entity_column<T, 0> >
        {};
    }

#if defined( DOXYGEN_INVOKED )
    //! \brief Creates a new entity of type \c Entity in the database using the data
    //!        held by \c entity.
    //! \remark This version of \c create will ignore autogenerate columns that are not also
    //!         the primary key. They will be generated in the database, but \c entity
    //!         will not be updated with the generated values.
    //! \param conn the backend connection.
    //! \param entity contains the data used to initialize the new entity created in the
    //!               database.
    //! \returns if \c Entity has an autogenerate primary key, the generated primary key is
    //!          returned. Otherwise, the return type of this function is \c void.
    //! \throws error if conn is invalid or if one of \c Entity's constraints are violated.
    template <typename Connection, typename Entity>
    inline unspecified create(Connection & conn, Entity const& entity);

    //! \brief Creates a new entity of type \c Entity in the database using the data held
    //!        by \c entity.
    //! \remark This version of \c create will update \c entity if any \c Entity has any
    //!         autogenerate columns. This means an extra query is executed.
    //! \param conn the backend connection.
    //! \param entity contains the data used to initialize the new entity created in the
    //!               database.
    //! \throws error if conn is invalid or if one of \c Entity's constraints are violated.
    template <typename Connection, typename Entity>
    inline void create(Connection & conn, Entity & entity);
#endif

    // ignores non-primary key autogenerate columns
    template <typename Connection, typename Entity>
    inline typename boost::disable_if<
        detail::has_auto_primary_key<Entity>,
        void
    >::type create(Connection & conn, Entity const& entity)
    {
        // insert the entity
        execute_command(conn, sql::create_insert_sql<Connection>(entity));
    }

    // ignores non-primary key autogenerate columns
    template <typename Connection, typename Entity>
    inline typename boost::enable_if<
        detail::has_auto_primary_key<Entity>,
        typename result_of::primary_key_of<Entity>::type
    >::type create(Connection & conn, Entity const& entity)
    {
        // the primary column metadata of the entity
        typedef typename result_of::primary_key_of<Entity>::type primary_key_type;

        // the result set returned when querying for the generated primary key
        typedef result_set<primary_key_type, typename Connection::cursor_type> result_set;

        // insert the entity
        execute_command(conn, sql::create_insert_sql<Connection>(entity));

        // query & return for the primary key
        return extension::select_last_insert_hook<Connection, primary_key_type>()(conn);
    }

    template <typename Connection, typename Entity>
    inline typename boost::enable_if_c<
        detail::has_auto_primary_key<Entity>::value && !detail::has_multiple_auto_columns<Entity>::value,
        void
    >::type create(Connection & conn, Entity & entity)
    {
        primary_key_of(entity) = create(conn, static_cast<Entity const&>(entity));
    }

    template <typename Connection, typename Entity>
    inline typename boost::enable_if<
        detail::has_multiple_auto_columns<Entity>,
        void
    >::type create(Connection & conn, Entity & entity)
    {
        // the autogenerate column types as a fusion vector
        typedef typename boost::fusion::result_of::as_vector<
            typename detail::result_of::autogenerate_columns_of<Entity>::type
        >::type auto_columns_type;

        // insert the entity
        execute_command(conn, sql::create_insert_sql<Connection>(entity));

        // select & set all autogenerate columns
        result_set<auto_columns_type, typename Connection::cursor_type> rs =
            execute_query<auto_columns_type>(conn, sql::create_find_autogenerate_sql<Connection>(entity));

        detail::fusion_copy(*rs.begin(), detail::autogenerate_columns_of(entity));
    }
}

#endif // #if !defined( CRYO_OPERATION_CREATE_HPP )
