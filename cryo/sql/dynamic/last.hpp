///////////////////////////////////////////////////////////////////////////////
//
//! \file last.hpp
//! Contains the \ref last \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_LAST_HPP )
#define CRYO_SQL_DYNAMIC_LAST_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/is_function.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief An \ref SqlFunctionMapping that describes the \c LAST SQL
        //!        aggregate function.
        //!
        //! This function can only appear in cryo expressions with one argument (a \ref ColumnChain)
        //! and results in the same type as the passed argument.
        struct last
        {
            template <typename Sig>
            struct result;

            template <typename Conn, typename A0>
            struct result<last(Conn, A0)>
                : boost::remove_reference<A0>
            {};

            static void apply(std::stringstream & ss)
            {
                ss << "LAST";
            }
        };
    }

    template <>
    struct is_aggregate_function<keyword::last>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::last>::type const last = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_LAST_HPP )
