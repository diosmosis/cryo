///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_DETAIL_MAKE_MEMBER_PTR_HPP )
#define CRYO_DETAIL_MAKE_MEMBER_PTR_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace detail
{
    struct dummy {};

    template <typename T>
    struct make_member_ptr
    {
        typedef T dummy::* type;
    };
}}

#endif // #if !defined( CRYO_DETAIL_MAKE_MEMBER_PTR_HPP )
