///////////////////////////////////////////////////////////////////////////////
//
//! \file post_create_db_hook.hpp
//! Contains the \c post_create_db_hook<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_POST_CREATE_DB_HOOK_HPP )
#define CRYO_EXTENSION_POST_CREATE_DB_HOOK_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace extension
{
    //! \brief A hook that is executed at the end of the \ref create_db operation.
    //!
    //! The default implementation of this type is a no-op.
    //!
    //! \tparam Connection the backend connection type.
    //! \tparam Schema a list of entities that should have corresponding tables in the database.
    //!                Models Boost.MPL's \c RandomAccessSequence concept.
    template <typename Connection, typename Schema>
    struct post_create_db_hook
    {
        void operator()(Connection const& conn) const
        {
            // empty
        }
    };
}}

#endif // #if !defined( CRYO_EXTENSION_POST_CREATE_DB_HOOK_HPP )
