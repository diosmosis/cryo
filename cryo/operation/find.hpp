///////////////////////////////////////////////////////////////////////////////
//
//! \file find.hpp
//! Contains the \ref find operation.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( CRYO_OPERATION_FIND_HPP )
#define CRYO_OPERATION_FIND_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/condition.hpp>
#include <cryo/sql/dynamic/expr_result_of.hpp>
#include <cryo/sql/create_find_sql.hpp>
#include <cryo/relational/execute_query.hpp>
#include <boost/optional.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/fusion/include/vector_tie.hpp>
#include <string>

namespace cryo
{
    namespace detail
    {
        template <typename Result, typename Connection>
        inline boost::optional<Result> find_impl(Connection const& conn, std::string const& cmd)
        {
            typedef result_set<Result, typename Connection::cursor_type> result_set;

            // perform the query
            result_set rs = execute_query<Result>(conn, cmd);

            // get the first result, if any
            typename result_set::iterator i = rs.begin();

            return i == rs.end() ? boost::none : boost::optional<Result>(*i);
        }

        template <typename Entity, typename Connection, typename PrimaryKey, typename Result, typename Columns>
        inline void find_partial_impl(
            Connection const& conn, PrimaryKey const& key, boost::optional<Result> & result, Columns const& cols)
        {
            result = find_impl<Result>(conn, sql::create_find_sql<Connection, Entity>(key, cols));
        }
    }

    //! \brief Finds the entity of type \c Entity whose primary key is equal to \c key.
    //! 
    //! If for some reason more than one entity is found, everything but the first is ignored.
    //!
    //! \tparam Entity the entity type to search for.
    //! \param conn the backend connection.
    //! \param key the the primary key of the entity being searched for.
    //! \returns the found entity wrapped in a \c boost::optional<> instance or \c boost::none,
    //!          if nothing could be found.
    //! \throws error if \c conn is inactive.
    template <typename Entity, typename Connection, typename PrimaryKey>
    inline boost::optional<Entity> find(Connection const& conn, PrimaryKey const& key)
    {
        typedef result_set<Entity, typename Connection::cursor_type> result_set;

        return detail::find_impl<Entity>(conn, sql::create_find_sql<Connection, Entity>(key));
    }

    //! \brief Finds all entities of type \c Entity in the database for which the condition
    //!        specified by \c q is true.
    //!
    //! \tparam Entity the entity type to search for.
    //! \param conn the backend connection.
    //! \param q a cryo query condition. If \c q is a null query, this function will retrieve
    //!          all entities of type \c Entity.
    //! \returns a \ref result_set<> that spans every entity of type \c Entity for which \c q
    //!          is true.
    //! \throws error if \c conn is inactive.
    template <typename Entity, typename Connection>
    inline result_set<Entity, typename Connection::cursor_type> find(Connection const& conn, sql::dynamic::condition const& q)
    {
        typedef result_set<Entity, typename Connection::cursor_type> result_set;

        // perform the query and return the result set
        return execute_query<Entity>(conn, sql::create_find_sql<Connection, Entity>(q));
    }

#if defined( DOXYGEN_INVOKED )
    // TODO: Need to make sure people can tell why an error was thrown. need enum of known error types.
    //! \brief Finds the entity of type \c Entity whose primary key is equal to \c key, and
    //!        selects specific bits of data determined by \c cols.
    //!
    //! If for some reason more than one entity is found, everything but the first is ignored.
    //!
    //! \tparam Entity the entity type to search for.
    //! \param conn the backend connection.
    //! \param xprs a collection of cryo query expressions describing the data to select from the
    //!             found entity. The number of expressions that can be specified here is determined
    //!             by \ref CRYO_PARAMETER_LIMIT.
    //! \param key the primary key of the entity being searched for.
    //! \returns the requested data as a Boost.Fusion sequence wrapped in a \c boost::optional<>
    //!          instance, or \c boost::none if nothing was found.
    //! \throws error if \c conn is inactive or if the \c cols specifies an invalid <tt>GROUP BY</tt>.
    template <typename Entity, typename Connection, typename... SelectExprs, typename PrimaryKey>
    inline boost::optional<boost::fusion::vector<unspecified> > find(
        Connection const& conn
      , SelectExprs xprs
      , PrimaryKey const& key
    );

    //! \brief Finds all entities of type \c Entity for which the cryo query condition \c q is
    //!        true, and select specific bits of data determined by \c cols.
    //!
    //! \tparam Entity the entity type to search for.
    //! \param conn the backend connection.
    //! \param xprs a collection of cryo query expressions describing the data to select from
    //!             each found entity. The number of expressions that can be specified here is
    //!             determined by \ref CRYO_PARAMETER_LIMIT.
    //! \param q a cryo query condition. If \c is a null query, this function will retrieve all
    //!          entities of type \c Entity.
    //! \returns a \ref result_set<> that returns a Boost.Fusion sequence for every entity for which
    //!          \c q is true. Only the data requested by \xprs is selected.
    template <typename Entity, typename Connection, typename... Columns>
    inline result_set<unspecified, typename Connection::cursor_type> find(
        Connection const& conn
      , SelectExprs xprs
      , sql::dynamic::condition const& q
    );
#endif

    // use preprocessor iteration to define the partial find methods
    #define CRYO_n BOOST_PP_ITERATION()
    #define CRYO_AUX_UNWRAP(z, n, data)                                                                               \
        BOOST_PP_COMMA_IF(n) typename sql::dynamic::expr_result_of<Connection, T ## n>::type
    #define BOOST_PP_FILENAME_1 <cryo/operation/find.hpp>
    #define BOOST_PP_ITERATION_LIMITS (1, CRYO_PARAMETER_LIMIT)
    #include BOOST_PP_ITERATE()
    #undef CRYO_n
    #undef CRYO_AUX_UNWRAP
}

#endif // #if !defined( CRYO_OPERATION_FIND_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <
        typename Entity
      , typename Connection
        BOOST_PP_ENUM_TRAILING_PARAMS(CRYO_n, typename T)
      , typename PrimaryKey
    >
    inline boost::optional<boost::fusion::vector<
        BOOST_PP_REPEAT(CRYO_n, CRYO_AUX_UNWRAP, _)
    > > find(
          Connection const& conn
        , BOOST_PP_ENUM_BINARY_PARAMS(CRYO_n, T, col)
        , PrimaryKey const& key
    )
    {
        boost::optional<boost::fusion::vector<BOOST_PP_REPEAT(CRYO_n, CRYO_AUX_UNWRAP, _)> > result;

        detail::find_partial_impl<Entity>(conn, key, result, boost::fusion::vector_tie(
            BOOST_PP_ENUM_PARAMS(CRYO_n, col)
        ));

        return result;
    }

    template <typename Entity, typename Connection BOOST_PP_ENUM_TRAILING_PARAMS(CRYO_n, typename T)>
    inline result_set<
        boost::fusion::vector<BOOST_PP_REPEAT(CRYO_n, CRYO_AUX_UNWRAP, _)>
      , typename Connection::cursor_type
    > find(Connection const& conn, BOOST_PP_ENUM_BINARY_PARAMS(CRYO_n, T, col), sql::dynamic::condition const& q)
    {
        return execute_query<
            boost::fusion::vector<BOOST_PP_REPEAT(CRYO_n, CRYO_AUX_UNWRAP, _)>
        >(conn, sql::create_find_sql<Connection, Entity>(q, boost::fusion::vector_tie(
            BOOST_PP_ENUM_PARAMS(CRYO_n, col)
        )));
    }

#endif // #if !defined( BOOST_PP_IS_ITERATING )
