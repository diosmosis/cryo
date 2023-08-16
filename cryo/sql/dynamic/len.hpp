///////////////////////////////////////////////////////////////////////////////
//
//! \file len.hpp
//! Contains the \ref len \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_LEN_HPP )
#define CRYO_SQL_DYNAMIC_LEN_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/is_function.hpp>
#include <boost/proto/traits.hpp>
#include <string>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief An \ref SqlFunctionMapping that describes the \c LEN SQL
        //!        function.
        //!
        //! This function can only appear in cryo expressions with one argument
        //! whose type is a string. This function results in the unsigned integer
        //! type of the specific database type.
        struct len
        {
            template <typename Conn, typename Sig>
            struct result;

            template <typename Conn, typename A0>
            struct result<Conn, len(A0)>
            {
                typedef typename Conn::uint_type type;
            };

            static void apply(std::stringstream & ss)
            {
                ss << "LEN";
            }
        };
    }

    template <>
    struct is_function<keyword::len>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::len>::type len = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_LEN_HPP )
