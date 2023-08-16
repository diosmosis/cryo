///////////////////////////////////////////////////////////////////////////////
//
//! \file first.hpp
//! Contains the \ref first \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_MIN_HPP )
#define CRYO_SQL_DYNAMIC_MIN_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/is_function.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief An \ref SqlFunctionMapping that describes the \c MIN SQL
        //!        aggregate function.
        //!
        //! This function can only appear in cryo expressions with one argument (a \ref ColumnChain)
        //! and results in the same type as the passed argument.
        struct min
        {
            template <typename Conn, typename Sig>
            struct result;

            template <typename Conn, typename A0>
            struct result<Conn, min(A0)>
                : boost::remove_reference<A0>
            {};

            static void apply(std::stringstream & ss)
            {
                ss << "MIN";
            }
        };
    }

    template <>
    struct is_aggregate_function<keyword::min>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::min>::type const min = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_MIN_HPP )
