///////////////////////////////////////////////////////////////////////////////
//
//! \file first.hpp
//! Contains the \ref avg \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_AVG_HPP )
#define CRYO_SQL_DYNAMIC_AVG_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/grammar.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/optional.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief An \ref SqlFunctionMapping that describes the \c AVG SQL
        //!        aggregate function.
        //!
        //! This function can only appear in cryo expressions with one argument (a \ref ColumnChain)
        //! and results in a \ref double.
        struct avg
        {
            template <typename Conn, typename Sig>
            struct result;

            template <typename Conn, typename A0>
            struct result<Conn, avg(A0)>
                : boost::mpl::identity<boost::optional<double> >
            {};

            static void apply(std::stringstream & ss)
            {
                ss << "AVG";
            }
        };
    }

    template <>
    struct is_aggregate_function<keyword::avg>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::avg>::type const avg = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_AVG_HPP )
