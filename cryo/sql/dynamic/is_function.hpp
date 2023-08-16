///////////////////////////////////////////////////////////////////////////////
//
//! \file is_function.hpp
//! Contains the \ref is_function<>, \ref is_aggregate_function<>,
//! \ref is_boolean_aggregate<> and \ref is_any_function<> metafunctions.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_IS_FUNCTION_HPP )
#define CRYO_SQL_DYNAMIC_IS_FUNCTION_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/or.hpp>

namespace cryo { namespace sql { namespace dynamic
{
    //! \brief Metafunction predicate that tells whether a \ref SqlFunctionMapping
    //!        type describes a non-aggregate SQL function.
    //! \tparam T the \ref SqlFunctionMapping to check.
    template <typename T>
    struct is_function
        : boost::mpl::false_
    {};

    //! \brief Metafunction predicate that tells whether a \ref SqlFunctionMapping
    //!        type describes an aggregate SQL function.
    //! \tparam T the \ref SqlFunctionMapping to check.
    template <typename T>
    struct is_aggregate_function
        : boost::mpl::false_
    {};

    //! \brief Metafunction predicate that tells whether a \ref SqlFunctionMapping
    //!        type describes a boolean aggregate function.
    //! \tparam T the \ref SqlFunctionMapping to check.
    template <typename T>
    struct is_boolean_aggregate
        : boost::mpl::false_
    {};

    //! \brief Metafunction predicate that tells whether a \ref SqlFunctionMapping
    //!        type describes some sort SQL function.
    //! \tparam T the \ref SqlFunctionMapping to check.
    template <typename T>
    struct is_any_function
        : boost::mpl::or_<
            is_function<T>,
            is_aggregate_function<T>,
            is_boolean_aggregate<T>
        >
    {};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_IS_FUNCTION_HPP )
