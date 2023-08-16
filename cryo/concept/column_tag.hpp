///////////////////////////////////////////////////////////////////////////////
//
//! \file column_tag.hpp
//! Contains an archetype for the \ref ColumnTag concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_COLUMN_TAG_HPP )
#define CRYO_CONCEPT_COLUMN_TAG_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace concepts
{
    //! \brief Concept that categorizes types used to describe the behavior and type of
    //!        entity columns in the backend database.
    //!
    //! \ref ColumnTags are associated with entity fields through the use of
    //! \ref FieldDecorators. The associated \ref ColumnTags of an entity field can be
    //! accessed through the \ref all_tags_of<> metafunction.
    //!
    //! \ref ColumnTags are used by backend implementations to vary SQL generation. cryo
    //! provides several hooks in the mechanism that generates SQL. Backends can specialize
    //! some hooks by different tags to generate SQL as the backend requires it. For example,
    //! the \ref output_datatype_hook<> type can be specialized by the \ref tag::varchar<>
    //! tag to output 'VARCHAR(N)' SQL in <tt>CREATE TABLE</tt> statements.
    //!
    //! \remark \ref ColumnTags have no real requirements. Any type can be considered a
    //!         \ref ColumnTag if desired.
    struct ColumnTag
    {
    };
}}

#endif // #if !defined( CRYO_CONCEPT_COLUMN_TAG_HPP )
