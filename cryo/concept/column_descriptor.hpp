///////////////////////////////////////////////////////////////////////////////
//
//! \file column_descriptor.hpp
//! Contains an archetype for the \c ColumnDescriptor concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_COLUMN_DESCRIPTOR_HPP )
#define CRYO_CONCEPT_COLUMN_DESCRIPTOR_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace concepts
{
    //! \brief Concept that categorizes types that fully describe a member to
    //!        column mapping.
    //!
    //! Types that model this concept can be used to get a column's entity type,
    //! field_type, table index and name along with a pointer-to-member that can
    //! extract the column data from an entity instance.
    struct ColumnDescriptor
    {
        typedef int unspecified;
        static const int unspecified_value = -1;

        //! \brief The entity the column belongs to.
        typedef unspecified entity_type;

        //! \brief The column's field_type.
        typedef unspecified field_type;

        //! \brief An integer representing the index of the column in the database
        //! table.
        static const int index = unspecified_value;

        //! \brief Gets the pointer-to-member associated with the column.
        //! \returns The pointer-to-member that extracts the column data from an
        //!          entity instance.
        field_type entity_type::* member() const
        {
            return 0;
        }

        //! \brief Gets the name of the column.
        //! \returns A null-terminated string representing the name of the column
        //!          as defined by the backend database.
        char const* name() const
        {
            return 0;
        }
    };
}}

#endif // #if !defined( CRYO_CONCEPT_COLUMN_DESCRIPTOR_HPP )
