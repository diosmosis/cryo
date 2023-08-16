///////////////////////////////////////////////////////////////////////////////
//
//! \brief nullable.hpp
//! Contains the \ref tag::nullable \ref ColumnTag type and the \ref is_nullable<>
//! metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_TAG_NULLABLE_HPP )
#define CRYO_SCHEMA_TAG_NULLABLE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/has_tag.hpp>

namespace cryo
{
    namespace tag
    {
        //! \brief A \ref ColumnTag that marks the the column it is associated with as a
        //!        column that can be set to null.
        //!
        //! There is no \ref FieldDecorator for this tag. Instead all implementations of
        //! the \ref Nullable concept must implicitly associate this tag with a field's
        //! column.
        struct nullable {};
    }

    //! \brief Metafunction that determines if a column has been tagged with the
    //!        \ref tag::nullable tag type.
    //!
    //! \tparam C A \ref PartialColumnDescriptor describing the table column that is to
    //!           be checked.
    template <typename C>
    struct is_nullable
        : has_tag<C, tag::nullable>
    {};
}

#endif // #if !defined( CRYO_SCHEMA_TAG_NULLABLE_HPP )
