///////////////////////////////////////////////////////////////////////////////
//
//! \brief function_result_of.hpp
//! Contains the \ref function_result_of<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( CRYO_SQL_DYNAMIC_FUNCTION_RESULT_OF_HPP )
#define CRYO_SQL_DYNAMIC_FUNCTION_RESULT_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/proto/proto.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/dec.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/cat.hpp>

namespace cryo { namespace sql { namespace dynamic
{
    template <typename Connection, typename E, int Arity = E::proto_arity_c - 1>
    struct function_result_of_impl;

    #define CRYO_n BOOST_PP_ITERATION()
    #define CRYO_DETAIL_EXTRACT_ARG(z, n, text)                                                         \
        typedef typename boost::proto::result_of::value<                                                \
            typename boost::proto::result_of::child_c<E, n + 1>::type >::type BOOST_PP_CAT(A, n);
    #define BOOST_PP_FILENAME_1 <cryo/sql/dynamic/function_result_of.hpp>
    #define BOOST_PP_ITERATION_LIMITS (1, CRYO_PARAMETER_LIMIT)
    #include BOOST_PP_ITERATE()
    #undef CRYO_n
    #undef CRYO_DETAIL_EXTRACT_ARG

    //! \brief Metafunction that determines the result of a SQL function call that
    //!        appears in a cryo query expression.
    //!
    //! \tparam Connection a \ref RelationalDatabaseConnection specifying the database
    //!         in which the function in question will be called.
    //! \tparam E A Boost.Proto expression that matches \ref any_function_call,
    //!           but not \ref boolean_aggregate_call. The first terminal of this
    //!           expression must model the \ref SqlFunctionMapping concept.
    //! \returns the result type of the SQL function call mapped to a C++ type.
    template <typename Connection, typename E>
    struct function_result_of
        : function_result_of_impl<Connection, E>
    {};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_FUNCTION_RESULT_OF_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename Connection, typename E>
    struct function_result_of_impl<Connection, E, CRYO_n>
    {
        typedef typename boost::proto::result_of::value<
            typename boost::proto::result_of::child_c<E, 0>::type
        >::type F;
        BOOST_PP_REPEAT(CRYO_n, CRYO_DETAIL_EXTRACT_ARG, _)
        typedef typename F::template result<Connection, F(BOOST_PP_ENUM_PARAMS(CRYO_n, A))>::type type;
    };

#endif // #if !defined( BOOST_PP_IS_ITERATING )
