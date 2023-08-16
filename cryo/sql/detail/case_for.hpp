///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( CRYO_SQL_DETAIL_CASE_FOR_HPP )
#define CRYO_SQL_DETAIL_CASE_FOR_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repeat_from_to.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/dec.hpp>

#if !defined( CRYO_SWITCH_LIMIT )
#   define CRYO_SWITCH_LIMIT 6
#endif

namespace cryo { namespace sql { namespace detail
{
    template <typename Vector, int S = boost::mpl::size<Vector>::value>
    struct case_for;

    // use preprocessor iteration to define the case methods
    #define CRYO_CASE_FOR_APPLY(n)                                                                                \
        boost::mpl::apply<typename boost::mpl::at_c<Vector, n>::type, T>::type::value ? n : -1
    #define CRYO_CASE_FOR_CHK_BEF(n)                                                                              \
        BOOST_PP_CAT(p, BOOST_PP_DEC(n)) == -1 ? (CRYO_CASE_FOR_APPLY(n)) : BOOST_PP_CAT(p, BOOST_PP_DEC(n))
    #define CRYO_CASE_FOR_CHECK_PRED(z, n, data)                                                                  \
        p ## n = CRYO_CASE_FOR_CHK_BEF(n),
    #define CRYO_n BOOST_PP_ITERATION()
    #define BOOST_PP_FILENAME_1 <cryo/sql/detail/case_for.hpp>
    #define BOOST_PP_ITERATION_LIMITS (1, CRYO_SWITCH_LIMIT)
    #include BOOST_PP_ITERATE()
    #undef CRYO_n
    #undef CRYO_CASE_FOR_APPLY
    #undef CRYO_CASE_FOR_CHK_BEF
    #undef CRYO_CASE_FOR_CHECK_PRED
}}}

#endif // #if !defined( CRYO_SQL_DETAIL_CASE_FOR_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename Vector>
    struct case_for<Vector, CRYO_n>
    {
        template <typename T>
        struct apply
        {
            enum
            {
                p0 = CRYO_CASE_FOR_APPLY(0),
                BOOST_PP_REPEAT_FROM_TO(1, CRYO_n, CRYO_CASE_FOR_CHECK_PRED, _)

                value = BOOST_PP_CAT(p, BOOST_PP_DEC(CRYO_n))
            };

            typedef boost::mpl::int_<value> type;
        };
    };

#endif // #if !defined( BOOST_PP_IS_ITERATING )
