///////////////////////////////////////////////////////////////////////////////
//
//! \file varchar.hpp
//! Contains the \ref tag::varchar<> \ref ColumnTag type, the \ref is_varchar<>
//! metafunction and the \ref cryo::varchar<> \ref FieldDecorator type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_TAG_VARCHAR_HPP )
#define CRYO_SCHEMA_TAG_VARCHAR_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/tag/wrapper.hpp>
#include <cryo/schema/introspect/has_tag.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>

namespace cryo
{
    namespace tag
    {
        //! \brief A \ref ColumnTag that marks the the column it is associated with as a
        //!        \c VARCHAR column.
        //!
        //! This tag only affects the way an entity's table is created.
        //!
        //! \tparam Length the maximum length of the data held in the column. If there is no
        //!                maximum, this parameter should be set to \c -1.
        template <int Length>
        struct varchar
        {
            typedef boost::mpl::int_<Length> type;
        };
    }

    namespace detail
    {
        template <typename T>
        struct is_varchar_tag
            : boost::mpl::false_
        {};

        template <int L>
        struct is_varchar_tag<tag::varchar<L> >
            : boost::mpl::true_
        {};
    }

    //! \brief Metafunction that determines if a column has been tagged with the
    //!        \ref tag::varchar tag type.
    //!
    //! \tparam C A \ref PartialColumnDescriptor describing the table column that is to
    //!           be checked.
    template <typename C>
    struct is_varchar
        : has_tag_if<C, detail::is_varchar_tag<boost::mpl::_1> >
    {};

    //! \brief A \ref FieldDecorator that associates an entity field with the
    //!        \ref tag::varchar tag type.
    //!
    //! This metafunction returns a type that wraps the supplied type \c T. The
    //! wrapped type will behave in exactly the same way as the supplied type.
    //!
    //! \tparam T the field type to be wrapped.
    template <typename T, int Length = -1>
    struct varchar
        : make_wrapper<T, tag::varchar<Length> >
    {};
}

#endif // #if !defined( CRYO_SCHEMA_TAG_VARCHAR_HPP )
