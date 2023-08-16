///////////////////////////////////////////////////////////////////////////////
//
//! \file partial_column_descriptor.hpp
//! Contains an archetype for the \ref PartialColumnDescriptor concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_PARTIAL_COLUMN_DESCRIPTOR_HPP )
#define CRYO_CONCEPT_PARTIAL_COLUMN_DESCRIPTOR_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace concepts
{
    //! \brief Concept that categorizes types that partially describe a member-to-column
    //!        mapping.
    //!
    //! Types that model this concept can be used to obtain a column's entity type, a
    //! column's field type, and a pointer-to-member that can extract the column data, but
    //! nothing more.
    //!
    //! The following types model the this concept:
    //! \li all \ref ColumnDescriptor models.
    //! \li pointer-to-members (ie, <tt>T Entity::*</tt>)
    struct PartialColumnDescriptor
    {
        typedef int unspecified;

        // TODO: I use the words entity & column w/o any sort of definition. They should be concepts. Maybe not column...
        //! \brief The entity the column belongs to.
        typedef unspecified entity_type;

        //! \brief The column's field type.
        typedef unspecified field_type;

        // TODO: How do I use pointer-to-members as PartialColumnDescriptors w/o concept_maps?
        //! \brief Returns a pointer-to-member that can be used to extract column data
        //!        from an entity instance.
        field_type entity_type::* member() const
        {
            return 0;
        }
    };
}}

#endif // #if !defined( CRYO_CONCEPT_PARTIAL_COLUMN_DESCRIPTOR_HPP )
