///////////////////////////////////////////////////////////////////////////////
//
//! \file find_all.hpp
//! Contains the \ref find_all operation.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( CRYO_OPERATION_FIND_ALL_HPP )
#define CRYO_OPERATION_FIND_ALL_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/relational/result_set.hpp>
#include <cryo/operation/find.hpp>
#include <cryo/sql/dynamic/expr_result_of.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/fusion/include/vector.hpp>

namespace cryo
{
    //! \brief Retrieves every entity of type \c Entity from the backend database.
    //! \tparam Entity the type of entity to retrieve.
    //! \param conn the backend connection.
    //! \returns a \ref result_set<> spanning every entity of type \c Entity in the database.
    template <typename Entity, typename Connection>
    inline result_set<Entity, typename Connection::cursor_type> find_all(Connection const& conn)
    {
        // perform the query
        return find<Entity>(conn, condition());
    }

#if defined(DOXYGEN_INVOKED)
    //! \brief Retrieves specific bits of data from every entity of type \c Entity currently
    //!        stored in the database referenced by \c conn.
    //! \tparam Entity the type of entity to retrieve.
    //! \param conn the backend connection.
    //! \param xprs a collection of cryo query expressions describing the data to select from
    //!             each found entity. The number of expressions that can be specified here is
    //!             determined by \ref CRYO_PARAMETER_LIMIT.
    //! \returns a \ref result_set<> that returns a Boost.Fusion sequence for every entity of
    //!          type \c Entity in the database. The fusion sequence's contents are determined
    //!          by the query expressions in \c cols.
    template <typename Entity, typename Connection, typename... Columns>
    inline result_set<unspecified, typename Connection::cursor_type> find_all(Connection const& conn, SelectExprs xprs);
#endif

    // use preprocessor iteration to define the partial find_all methods
    #define CRYO_n BOOST_PP_ITERATION()
    #define CRYO_AUX_UNWRAP(z, n, data)                                                                               \
        BOOST_PP_COMMA_IF(n) typename sql::dynamic::expr_result_of<Connection, T ## n>::type
    #define BOOST_PP_FILENAME_1 <cryo/operation/find_all.hpp>
    #define BOOST_PP_ITERATION_LIMITS (1, CRYO_PARAMETER_LIMIT)
    #include BOOST_PP_ITERATE()
    #undef CRYO_n
    #undef CRYO_AUX_UNWRAP
}

#endif // #if !defined( CRYO_OPERATION_FIND_ALL_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename Entity, typename Connection BOOST_PP_ENUM_TRAILING_PARAMS(CRYO_n, typename T)>
    inline result_set<
        boost::fusion::vector<BOOST_PP_REPEAT(CRYO_n, CRYO_AUX_UNWRAP, _)>
      , typename Connection::cursor_type
    > find_all(Connection const& conn, BOOST_PP_ENUM_BINARY_PARAMS(CRYO_n, T, col))
    {
        return find<Entity>(conn, BOOST_PP_ENUM_PARAMS(CRYO_n, col), condition());
    }

#endif // #if !defined( BOOST_PP_IS_ITERATING )
