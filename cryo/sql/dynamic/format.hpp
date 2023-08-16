///////////////////////////////////////////////////////////////////////////////
//
//! \file format.hpp
//! Contains the \ref format \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_FORMAT_HPP )
#define CRYO_SQL_DYNAMIC_FORMAT_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/is_function.hpp>
#include <boost/proto/traits.hpp>
#include <string>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief An \ref SqlFunctionMapping that describes the \c FORMAT SQL
        //!        function.
        //!
        //! This function can only appear in cryo expressions with two arguments passed
        //! and results in a \c std::string.
        struct format
        {
            template <typename Conn, typename Sig>
            struct result;

            template <typename Conn, typename A0, typename A1>
            struct result<Conn, format(A0, A1)>
            {
                typedef std::string type;
            };

            static void apply(std::stringstream & ss)
            {
                ss << "FORMAT";
            }
        };
    }

    template <>
    struct is_function<keyword::format>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::format>::type const format = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_FORMAT_HPP )
