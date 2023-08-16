///////////////////////////////////////////////////////////////////////////////
//
//! \file entity_descriptor.hpp
//! Contains an archetype for the \ref EntityDescriptor concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if defined( CRYO_CONCEPT_ENTITY_DESCRIPTOR_HPP )
#define CRYO_CONCEPT_ENTITY_DESCRIPTOR_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace concepts
{
    //! \brief Concept categorizing types that describe an entity to table mapping.
    //!
    //! Types that model this concept can be used to access an entity's column count
    //! and the entity's associated table name.
    struct EntityDescriptor
    {
        enum { unspecified = 0 };

        //! \brief The number of columns that the entity maps.
        static const int column_count = unspecified;

        //! \brief Gets the name of the database table the entity is associated with.
        //! \returns The name of an entity's associated table. Must always be a non-null,
        //!          null terminated string.
        static char const* table_name()
        {
            return "";
        }
    };
}}

#endif // #if defined( CRYO_CONCEPT_ENTITY_DESCRIPTOR_HPP )
