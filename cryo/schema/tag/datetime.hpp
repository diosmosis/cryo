///////////////////////////////////////////////////////////////////////////////
//
//! \file datetime.hpp
//! Contains the \ref tag::datetime \ref ColumnTag type, the \ref is_datetime<>
//! metafunction and the \ref cryo::datetime<> \ref FieldDecorator type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_TAG_DATETIME_HPP )
#define CRYO_SCHEMA_TAG_DATETIME_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/has_tag.hpp>
#include <cryo/schema/tag/wrapper.hpp>

namespace cryo
{
    namespace tag
    {
        //! \brief A \ref ColumnTag that marks the the column it is associated with as a
        //!        \c DATETIME column.
        //!
        //! This tag only affects the way an entity's table is created. The conversion
        //! from the \c DATETIME SQL type to the specific field type is handled by the
        //! backend being used.
        struct datetime {};
    }

    //! \brief Metafunction that determines if a column has been tagged with the
    //!        \ref tag::datetime tag type.
    //!
    //! \tparam C A \ref PartialColumnDescriptor describing the table column that is to
    //!           be checked.
    template <typename C>
    struct is_datetime
        : has_tag<C, tag::datetime>
    {};

    //! \brief A \ref FieldDecorator that associates an entity field with the
    //!        \ref tag::datetime tag type.
    //!
    //! This metafunction returns a type that wraps the supplied type \c T. The
    //! wrapped type will behave the same as the supplied type.
    //!
    //! \tparam T the field type to be wrapped.
    template <typename T>
    struct datetime
        : make_wrapper<T, tag::datetime>
    {};
}

#endif // #if !defined( CRYO_SCHEMA_TAG_DATETIME_HPP )
