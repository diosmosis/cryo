///////////////////////////////////////////////////////////////////////////////
//
//! \file column_name_hook.hpp
//! Contains the \ref column_name_hook<> type and the \ref CRYO_MAP_ENTITY_COLUMN
//! macro.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_COLUMN_NAME_HOOK_HPP )
#define CRYO_EXTENSION_COLUMN_NAME_HOOK_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/entity_column.hpp>

//! \brief Convenience macro. Specifies a custom column name (denoted by \c name)
//!        for the <tt>n</tt>th column of \c entity.
//! \param entity the entity type whose column is being mapped. Must be fully qualified.
//! \param n the column index.
//! \param name a string constant denoting the name of the column in the backend DBMS.
#define CRYO_MAP_ENTITY_COLUMN(entity, n, name)                                                                       \
    namespace cryo { namespace extension                                                                              \
    {                                                                                                                 \
        template <>                                                                                                   \
        struct column_name_hook<entity_column<entity, n> >                                                            \
        {                                                                                                             \
            static char const* operator()() const                                                                     \
            {                                                                                                         \
                return name;                                                                                          \
            }                                                                                                         \
        };                                                                                                            \
    }}

namespace cryo { namespace extension
{
    //! \brief A hook that allows users of cryo to specify a custom column names in case
    //!        the default name is not desired.
    //!
    //! Specializations of this type must provide the following operator:
    //! <tt>char const* operator()() const</tt>
    //! This operator returns the desired name of the column as a null terminated string.
    //!
    //! \tparam C a specialization of \c entity_column<>.
    template <typename C>
    struct column_name_hook
    {
        char const* operator()() const
        {
            return C().name();
        }
    };
}}

#endif // #if !defined( CRYO_EXTENSION_COLUMN_NAME_HOOK_HPP )
