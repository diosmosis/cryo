///////////////////////////////////////////////////////////////////////////////
//
//! \file round.hpp
//! Contains the \ref first \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_ROUND_HPP )
#define CRYO_SQL_DYNAMIC_ROUND_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/is_function.hpp>
#include <boost/proto/traits.hpp>
#include <string>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief An \ref SqlFunctionMapping that describes the \c FIRST SQL
        //!        aggregate function.
        //!
        //! This function can only appear in cryo expressions with two arguments, an expression
        //! and an integer specifying the number of decimals to return. This function results in 
        //! a \c double.
        struct round
        {
            template <typename Conn, typename Sig>
            struct result;

            template <typename Conn, typename A0, typename A1>
            struct result<Conn, round(A0, A1)>
            {
                typedef double type;
            };

            static void apply(std::stringstream & ss)
            {
                ss << "ROUND";
            }
        };
    }

    template <>
    struct is_function<keyword::round>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::round>::type round = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_ROUND_HPP )
