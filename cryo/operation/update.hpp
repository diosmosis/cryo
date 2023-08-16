///////////////////////////////////////////////////////////////////////////////
//
//! \file update.hpp
//! Contains the \ref update operation.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( CRYO_OPERATION_UPDATE_HPP )
#define CRYO_OPERATION_UPDATE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/error.hpp>
#include <cryo/sql/create_update_sql.hpp>
#include <cryo/detail/updatable_columns_of.hpp>
#include <cryo/relational/execute_command.hpp>
#include <boost/fusion/include/vector_tie.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

namespace cryo
{
    namespace detail
    {
        template <typename Connection>
        inline void post_update_check(Connection const& conn)
        {
            // check that the entity was updated
            if (conn.affected_row_count() != 1)
            {
                if (conn.affected_row_count() == 0)
                {
                    throw error("the entity was not found when updating");
                }
                else
                {
                    throw error("more than one entity was updated");
                }
            }
        }
    }

    //! \brief Modifies the entity of type \c Entity in the database with the data found
    //!        in \c entity.
    //! \param conn the backend connection.
    //! \param entity contains the data used to modify the entity in the database with.
    //! \throws error if conn is inactive or if one of \c Entity's constraints is violated.
    template <typename Connection, typename Entity>
    inline void update(Connection & conn, Entity const& entity)
    {
        // update the entity
        execute_command(conn, sql::create_update_sql<Connection>(entity, updatable_columns_of<Entity>()));

        detail::post_update_check(conn);
    }

#if defined( DOXYGEN_INVOKED )
    //! \brief Modifies the entity of type \c Entity in the database with the data found
    //!        in specific columns of \c entity.
    //! \param conn the backend connection.
    //! \param entity contains the data used to modify the entity in the database with.
    //! \param cols a collection of \ref ColumnDescriptors that specify which columns of
    //!        the entity to modify in the database. The number of columns that can be
    //!        specified here is determined by the \ref CRYO_PARAMETER_LIMIT macro.
    template <typename Connection, typename Entity, typename... Columns>
    inline void update(Connection & conn, Entity const& entity, Columns... cols);
#endif

    template <typename Connection, typename Entity, typename Columns>
    inline void update_impl(Connection & conn, Entity const& entity, Columns const& columns)
    {
        // update the specified columns
        execute_command(conn, sql::create_update_sql<Connection>(entity, columns));

        detail::post_update_check(conn);
    }

    // use preprocessor iteration to define the partial update methods
    #define CRYO_n BOOST_PP_ITERATION()
    #define BOOST_PP_FILENAME_1 <cryo/operation/update.hpp>
    #define BOOST_PP_ITERATION_LIMITS (1, CRYO_PARAMETER_LIMIT)
    #include BOOST_PP_ITERATE()
    #undef CRYO_n
}

#endif // #if !defined( CRYO_OPERATION_UPDATE_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename Connection, typename Entity BOOST_PP_ENUM_TRAILING_PARAMS(CRYO_n, typename T)>
    inline void update(Connection & conn, Entity const& entity, BOOST_PP_ENUM_BINARY_PARAMS(CRYO_n, T, Entity::* col))
    {
        update_impl(conn, entity, boost::fusion::vector_tie(BOOST_PP_ENUM_PARAMS(CRYO_n, col)));
    }

#endif // #if !defined( BOOST_PP_IS_ITERATING )
