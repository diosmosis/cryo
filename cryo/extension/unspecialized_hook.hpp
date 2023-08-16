///////////////////////////////////////////////////////////////////////////////
//
//! \file unspecialized_hook.hpp
//! Contains the \ref unspecialized_hook type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_UNSPECIALIZED_HOOK_HPP )
#define CRYO_EXTENSION_UNSPECIALIZED_HOOK_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace extension
{
    //! \brief An internal type used to mark hook specializations that have not
    //!        been specialized by a backend.
    struct unspecialized_hook
    {
    private:
        unspecialized_hook() {}
    };
}}

#endif // #if !defined( CRYO_EXTENSION_UNSPECIALIZED_HOOK_HPP )
