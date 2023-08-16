///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_DETAIL_INDIRECT_INVOKE_HPP )
#define CRYO_DETAIL_INDIRECT_INVOKE_HPP

#include <cryo/cryo_fwd.hpp>
#include <sstream>

namespace cryo { namespace detail
{
    struct indirect_invoke
    {
        template <typename F>
        void operator()(std::stringstream & ss, F const& f) const
        {
            f(ss);
        }
    };
}}

#endif // #if !defined( CRYO_DETAIL_INDIRECT_INVOKE_HPP )
