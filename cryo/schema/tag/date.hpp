///////////////////////////////////////////////////////////////////////////////
//
//! \file date.hpp
//! Contains the \ref tag::date \ref ColumnTag type, the \ref is_date<>
//! metafunction and the \ref cryo::date<> \ref FieldDecorator type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_TAG_DATE_HPP )
#define CRYO_SCHEMA_TAG_DATE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/has_tag.hpp>
#include <cryo/schema/tag/wrapper.hpp>

namespace cryo
{
    namespace tag
    {
        //! \brief A \ref ColumnTag that marks the the column it is associated with as a
        //!        \c DATE column.
        //!
        //! This tag only affects the way an entity's table is created. The conversion
        //! from the \c DATE SQL type to the specific field type is handled by the
        //! backend being used.
        struct date {};
    }

    //! \brief Metafunction that determines if a column has been tagged with the
    //!        \ref tag::date tag type.
    //!
    //! \tparam C A \ref PartialColumnDescriptor describing the table column that is to
    //!           be checked.
    template <typename C>
    struct is_date
        : has_tag<C, tag::date>
    {};

    //! \brief A \ref FieldDecorator that associates an entity field with the \ref tag::date
    //!        tag type.
    //!
    //! This metafunction returns a type that wraps the supplied type \c T. The
    //! wrapped type will behave in exactly the same way as the supplied type.
    //!
    //! \tparam T the field type to be wrapped.
    template <typename T>
    struct date
        : make_wrapper<T, tag::date>
    {};
}

#endif // #if !defined( CRYO_SCHEMA_TAG_DATE_HPP )
