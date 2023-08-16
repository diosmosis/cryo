///////////////////////////////////////////////////////////////////////////////
//
//! \file foreign_key.hpp
//! Contains the \ref foriegn_key<> field tag and the \ref is_foreign_key<> and
//! \ref foreign_key<> metafunctions.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_TAG_FOREIGN_KEY_HPP )
#define CRYO_SCHEMA_TAG_FOREIGN_KEY_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/tag/wrapper.hpp>
#include <cryo/schema/entity_column.hpp>
#include <cryo/schema/introspect/has_tag.hpp>
#include <cryo/schema/introspect/primary_key_of.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <cryo/schema/introspect/column_type_of.hpp>
#include <boost/mpl/bool.hpp>

namespace cryo
{
    namespace tag
    {
        //! \brief A \ref ColumnTag that marks the the column it is associated with as a
        //!        reference to another entity.
        //!
        //! This tag affects both the way a column is defined in a backend database and the
        //! way it is read. Columns with this tag are defined with foreign key constraints.
        //! When being selected, if the field type is an entity or a reference to an entity
        //! type, the columns of the foreign entity type are selected as well.
        //!
        //! \tparam ForeignEntity The entity type referenced by the tagged field.
        template <typename ForeignEntity>
        struct foreign_key
        {
            typedef ForeignEntity type;
        };
    }

    namespace detail
    {
        template <typename T>
        struct is_foreign_key_tag
            : boost::mpl::false_
        {};

        template <typename F>
        struct is_foreign_key_tag<tag::foreign_key<F> >
            : boost::mpl::true_
        {};
    }

    //! \brief Metafunction that determines if a column has been tagged with the
    //!        \ref tag::foreign_key<> tag type.
    //!
    //! \tparam C A \ref PartialColumnDescriptor describing the table column that is to
    //!           be checked.
    template <typename C>
    struct is_foreign_key
        : has_tag_if<
            C, detail::is_foreign_key_tag<boost::mpl::_1>
        >
    {};

    //! \brief A \ref FieldDecorator that associates an entity field with the
    //!        \ref tag::foreign_key<> tag type.
    //!
    //! This metafunction returns a type that wraps the primary key type of the
    //! supplied entity. The wrapped type will behave as a primitive type.
    //!
    //! \remark The use of this decorator is only necessary if you don't want to
    //! reference another entity in memory, but only in the database. In all other
    //! cases, simply encapsulating an instance of the foreign entity or a reference
    //! to the foreign entity (ie, a \c boost::shared_ptr<>) instance will suffice.
    //!
    //! \tparam T the field type to be wrapped.
    template <typename ForeignEntity>
    struct foreign_key
        : make_wrapper<
            typename column_type_of<entity_column<ForeignEntity, 0> >::type,
            tag::foreign_key<ForeignEntity>
        >
    {};
}

#endif // #if !defined( CRYO_SCHEMA_TAG_FOREIGN_KEY_HPP )
