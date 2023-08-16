///////////////////////////////////////////////////////////////////////////////
//
//! \file entity_metadata.hpp
//! Contains the \ref entity_metadata<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_ENTITY_METADATA_HPP )
#define CRYO_SCHEMA_ENTITY_METADATA_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo
{
    //! \brief An implementation of the \ref EntityDescriptor concept.
    //!
    //! This type is filled out by the \ref CRYO_DECLARE_ENTITY macro.
    //!
    //! \tparam T the entity type being described.
    template <typename T>
    struct entity_metadata;

    template <typename T>
    struct entity_metadata<T const>
        : entity_metadata<T>
    {};
}

#endif // #if !defined( CRYO_SCHEMA_ENTITY_METADATA_HPP )
