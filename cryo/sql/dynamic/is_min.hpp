///////////////////////////////////////////////////////////////////////////////
//
//! \file is_min.hpp
//! Contains the \ref is_min \ref SqlFunctionMapping type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_IS_MIN_HPP )
#define CRYO_SQL_DYNAMIC_IS_MIN_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/min.hpp>
#include <boost/proto/traits.hpp>
#include <boost/mpl/identity.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief The boolean aggregate version of the \ref min \ref SqlFunctionMapping
        //!        type.
        //!
        //! This function can only appear in cryo conditions and only with one argument
        //! passed to it (a \ref ColumnChain).
        struct is_min : min {};
    }

    template <>
    struct is_boolean_aggregate<keyword::is_min>
        : boost::mpl::true_
    {};

    boost::proto::terminal<keyword::is_min>::type const is_min = {keyword::is_min()};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_IS_MIN_HPP )
