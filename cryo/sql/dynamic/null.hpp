///////////////////////////////////////////////////////////////////////////////
//
//! \file null.hpp
//! Contains the \ref null keyword.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_NULL_HPP )
#define CRYO_SQL_DYNAMIC_NULL_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/proto/proto.hpp>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        //! \brief A keyword used to represent \c NULL in cryo query expressions.
        //!
        //! This keyword can only be used on either end of a equals or not equals
        //! binary expression. When used <tt>IS NULL</tt> or <tt>IS NOT NULL</tt>
        //! SQL will be generated.
        struct null {};
    }

    // null
    boost::proto::terminal<keyword::null>::type const null = {{}};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_NULL_HPP )
