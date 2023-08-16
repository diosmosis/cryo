///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( BOOST_PP_IS_ITERATING )

#if !defined( CRYO_SQL_DETAIL_SWITCH_HPP )
#define CRYO_SQL_DETAIL_SWITCH_HPP

#include <cryo/sql/detail/case_for.hpp>
#include <boost/preprocessor/iterate.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/type_traits/remove_const.hpp>

#if !defined( CRYO_SWITCH_PARAMETER_LIMIT )
#   define CRYO_SWITCH_PARAMETER_LIMIT 2
#endif

namespace cryo { namespace sql { namespace detail
{
    // NOTE: The switch value is appended as an argument to Signature, so there will always be at least one arg.
    template <typename Derived, typename Signature, typename Cases>
    struct switch_;
    
    // use preprocessor iteration to define the case methods
    #define CRYO_SWITCH_ARG_DECL(z, n, data) A##n a##n,
    #define CRYO_SWITCH_ARG(z, n, data) a##n,
    #define CRYO_n BOOST_PP_ITERATION()
    #define BOOST_PP_FILENAME_1 <cryo/sql/detail/switch.hpp>
    #define BOOST_PP_ITERATION_LIMITS (0, CRYO_SWITCH_PARAMETER_LIMIT)
    #include BOOST_PP_ITERATE()
    #undef CRYO_n
    #undef CRYO_SWITCH_ARG_DECL
    #undef CRYO_SWITCH_ARG
}}}

#endif // #if !defined( CRYO_SQL_DETAIL_SWITCH_HPP )

#else // #if !defined( BOOST_PP_IS_ITERATING )

    template <typename D, typename C, typename R BOOST_PP_ENUM_TRAILING_PARAMS(CRYO_n, typename A)>
    struct switch_<D, R(BOOST_PP_ENUM_PARAMS(CRYO_n, A)), C>
    {
        // TODO: use call_traits here.
        template <typename T>
        R operator()(BOOST_PP_REPEAT(CRYO_n, CRYO_SWITCH_ARG_DECL, _) T & x) const
        {
            typedef typename boost::remove_const<T>::type T_;
            return call_case_or_default(BOOST_PP_REPEAT(CRYO_n, CRYO_SWITCH_ARG, _) x, typename case_for<C>::apply<T_>::type());
        }

        template <typename T, int N>
        R call_case_or_default(BOOST_PP_REPEAT(CRYO_n, CRYO_SWITCH_ARG_DECL, _) T & x, boost::mpl::int_<N> n) const
        {
            return static_cast<D const*>(this)->case_(BOOST_PP_REPEAT(CRYO_n, CRYO_SWITCH_ARG, _) x, n);
        }

        template <typename T>
        R call_case_or_default(BOOST_PP_REPEAT(CRYO_n, CRYO_SWITCH_ARG_DECL, _) T & x, boost::mpl::int_<-1>) const
        {
            return static_cast<D const*>(this)->default_(BOOST_PP_REPEAT(CRYO_n, CRYO_SWITCH_ARG, _) x);
        }
    };

#endif // #if !defined( BOOST_PP_IS_ITERATING )
