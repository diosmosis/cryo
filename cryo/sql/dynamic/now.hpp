///////////////////////////////////////////////////////////////////////////////
//
//! \file now.hpp
//! Contains the \ref now \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_NOW_HPP )
#define CRYO_SQL_DYNAMIC_MAX_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/is_function.hpp>
#include <boost/proto/traits.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief An \ref SqlFunctionMapping that describes the \c FIRST SQL
        //!        aggregate function.
        //!
        //! This function can only appear in cryo expressions without arguments
        //! and results in a \c std::string.
        struct now
        {
            template <typename Conn, typename Sig>
            struct result;

            template <typename Conn>
            struct result<Conn, now()>
            {
                typedef std::string type;
            };

            static void apply(std::stringstream & ss)
            {
                ss << "NOW";
            }
        };
    }

    template <>
    struct is_function<keyword::now>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::now>::type const now = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_MAX_HPP )
