///////////////////////////////////////////////////////////////////////////////
//
//! \file last_insert_id_hook.hpp
//! Contains the \c last_insert_id_hook<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_LAST_INSERT_ID_HOOK_HPP )
#define CRYO_EXTENSION_LAST_INSERT_ID_HOOK_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace extension
{
    //! \brief Allows backends to specify the backend-specific SQL function used to obtain
    //!        the last autogenerated integer id.
    //!
    //! Specializations must define the following operator:
    //! <tt>void operator()(std::stringstream & ss) const</tt>
    //! This operator outputs the appropriate SQL function call.
    //!
    //! \tparam Connection the backend connection type. Models \ref RelationalDatabaseConcept.
    template <typename Connection>
    struct last_insert_id_hook;
}}

#endif // #if !defined( CRYO_EXTENSION_LAST_INSERT_ID_HOOK_HPP )
