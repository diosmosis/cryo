///////////////////////////////////////////////////////////////////////////////
//
//! \file first.hpp
//! Contains the \ref ucase \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_UCASE_HPP )
#define CRYO_SQL_DYNAMIC_UCASE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/is_function.hpp>
#include <boost/proto/traits.hpp>
#include <string>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief An \ref SqlFunctionMapping that describes the \c UCASE SQL
        //!        function.
        //!
        //! This function can only appear in cryo expressions with one argument
        //! and results in a \c std::string.
        struct ucase
        {
            template <typename Conn, typename Sig>
            struct result;

            template <typename Conn, typename A0>
            struct result<Conn, ucase(A0)>
            {
                typedef std::string type;
            };

            static void apply(std::stringstream & ss)
            {
                ss << "UCASE";
            }
        };
    }

    template <>
    struct is_function<keyword::ucase>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::ucase>::type ucase = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_UCASE_HPP )
