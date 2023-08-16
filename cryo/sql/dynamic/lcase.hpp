///////////////////////////////////////////////////////////////////////////////
//
//! \file first.hpp
//! Contains the \ref lcase \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_LCASE_HPP )
#define CRYO_SQL_DYNAMIC_LCASE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/is_function.hpp>
#include <boost/proto/traits.hpp>
#include <string>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    // TODO: Need a like operator or ability to add one. need a match operator too for fts indices.
    namespace keyword
    {
        //! \brief An \ref SqlFunctionMapping that describes the \c LCASE SQL
        //!        function.
        //!
        //! This function can only appear in cryo expressions with one argument
        //! and results in a \c std::string.
        struct lcase
        {
            template <typename Sig>
            struct result;

            template <typename Conn, typename A0>
            struct result<lcase(Conn, A0)>
            {
                // TODO: connections should be able to specify their own string types (which must be implicitly convertible to std::string)
                typedef std::string type;
            };

            static void apply(std::stringstream & ss)
            {
                ss << "LCASE";
            }
        };
    }

    template <>
    struct is_function<keyword::lcase>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::lcase>::type lcase = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_LCASE_HPP )
