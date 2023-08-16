///////////////////////////////////////////////////////////////////////////////
//
//! \file mid.hpp
//! Contains the \ref mid \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_MID_HPP )
#define CRYO_SQL_DYNAMIC_MID_HPP

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
        //! This function can appear in cryo expressions with either two or three arguments.
        //! The first argument can be an expression that results in a string, the second
        //! an expression that results in an integer, and the third, if present, an expression
        //! that results in an integer. This function results in a \c std::string.
        struct mid
        {
            template <typename Sig>
            struct result;

            template <typename Conn, typename A0, typename A1>
            struct result<mid(Conn, A0, A1)>
            {
                typedef std::string type;
            };

            template <typename Conn, typename A0, typename A1, typename A2>
            struct result<mid(Conn, A0, A1, A2)>
            {
                typedef std::string type;
            };

            static void apply(std::stringstream & ss)
            {
                ss << "MID";
            }
        };
    }

    template <>
    struct is_function<keyword::mid>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::mid>::type mid = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_MID_HPP )
