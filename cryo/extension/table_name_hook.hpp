///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_TABLE_NAME_HOOK_HPP )
#define CRYO_EXTENSION_TABLE_NAME_HOOK_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/entity_metadata.hpp>

//! \brief Convenience macro. Specifies a custom table name (denoted by \c name)
//!        for \c entity.
//! \param entity the entity type. A fully qualified type.
//! \param name a string constant denoting the desired table name of \c entity.
#define CRYO_MAP_ENTITY_TABLE(entity, name)                                                                           \
    namespace cryo { namespace extension                                                                              \
    {                                                                                                                 \
        template <>                                                                                                   \
        struct table_name_hook<entity>                                                                                \
        {                                                                                                             \
            char const* operator()() const                                                                            \
            {                                                                                                         \
                return name;                                                                                          \
            }                                                                                                         \
        };                                                                                                            \
    }}

namespace cryo { namespace extension
{
    //! \brief A hook that can be used to specify custom table names in case the
    //!        default table name is not desired.
    //!
    //! Specializations of this type must provide the following operator:
    //! <tt>char const* operator()() const</tt>
    //! This operator returns the desired name of the table as a null terminated string.
    //!
    //! \tparam Entity the entity type whose associated table name is being specified.
    template <typename Entity>
    struct table_name_hook
    {
        char const* operator()() const
        {
            return entity_metadata<Entity>::table_name();
        }
    };
}}

#endif // #if !defined( CRYO_EXTENSION_TABLE_NAME_HOOK_HPP )
