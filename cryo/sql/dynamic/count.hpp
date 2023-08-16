///////////////////////////////////////////////////////////////////////////////
//
//! \file count.hpp
//! Contains the \ref count \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_COUNT_HPP )
#define CRYO_SQL_DYNAMIC_COUNT_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/grammar.hpp>
#include <boost/mpl/identity.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief An \ref SqlFunctionMapping that describes the \c COUNT SQL
        //!        aggregate function.
        //!
        //! This function can only appear in cryo expressions with one argument (a \ref ColumnChain)
        //! and results in the unsigned int type of a specific \ref RelationalDatabaseConnection.
        struct count
        {
            template <typename Conn, typename Sig>
            struct result;

            template <typename Conn, typename A0>
            struct result<Conn, count(A0)>
                : boost::mpl::identity<typename Conn::uint_type>
            {};

            static void apply(std::stringstream & ss)
            {
                ss << "COUNT";
            }
        };
    }

    template <>
    struct is_aggregate_function<keyword::count>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::count>::type const count = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_COUNT_HPP )
