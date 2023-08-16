///////////////////////////////////////////////////////////////////////////////
//
//! \file blob.hpp
//! Contains the \ref tag::blob<> \ref ColumnTag type, the \ref is_blob<>
//! metafunction, the \ref cryo::blob<> \ref FieldDecorator type and the
//! \ref max_serialized_blob_size<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_TAG_BLOB_HPP )
#define CRYO_SCHEMA_TAG_BLOB_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/tag/wrapper.hpp>
#include <cryo/schema/introspect/has_tag.hpp>
#include <cryo/support/raw_byte_serializer.hpp>
#include <boost/mpl/int.hpp>

namespace cryo
{
    namespace tag
    {
        //! \brief A \ref ColumnTag that marks the the column it is associated with as a
        //!        \c DATE column.
        //!
        //! This tag affects the way an entity's table is created and the way in which an
        //! entity's column is selected. Column's associated with this tag will be defined
        //! as \c BLOBs in the database. The way in which data is read from and written to
        //! the database is determined by the \c Serializer type.
        //!
        //! \tparam Serializer the type that controls how \c BLOB data is stored and read.
        //!         Models \ref BlobSerializer. If this type is \c void, the associated
        //!         column will not be selected and \c BLOB data can only be streamed,
        //!         assuming the backend supports it.
        template <typename Serializer>
        struct blob
        {
            typedef Serializer serializer_type;
        };
    }

    namespace detail
    {
        template <typename T>
        struct is_blob_tag
            : boost::mpl::false_
        {};

        template <typename S>
        struct is_blob_tag<tag::blob<S> >
            : boost::mpl::true_
        {};
    }

    //! \brief Metafunction that determines if a column has been tagged with the
    //!        \ref tag::blob<> tag type.
    //!
    //! \tparam C A \ref PartialColumnDescriptor describing the table column that is to
    //!           be checked.
    template <typename C>
    struct is_blob
        : has_tag_if<C, detail::is_blob_tag<boost::mpl::_1> >
    {};

    //! \brief A \ref FieldDecorator that associates an entity field with the \ref tag::blob<>
    //!        tag type.
    //!
    //! This metafunction returns a type that wraps the supplied type \c T. The
    //! wrapped type will behave exactly the same as the supplied type.
    //!
    //! \tparam T the field type to be wrapped.
    //! \tparam Serializer a \ref BlobSerializer that controls how data is written to and read
    //!                    from the \c BLOB column.
    template <typename T, typename Serializer = raw_byte_serializer>
    struct blob
        : make_wrapper<T, tag::blob<Serializer> >
    {};

    //! \brief A helper metafunction that used can be used to extract the maximum serialized
    //!        size (in bytes) of a C++ type with regard to a specific \ref BlobSerializer.
    //! \tparam T the C++ type that will be serialized.
    //! \tparam Tag the \ref tag::blob<> tag whose associated serializer (if any) is to be used.
    //!             If no serializer is associated (if the type supplied to \ref tag::blob is \c void),
    //!             This metafunction returns -1.
    //! \returns The maximum number of bytes an instance of \c T will occupy when serialized,
    //!          or \c -1 if the value is unknown.
    template <typename T, typename Tag>
    struct max_serialized_blob_size;

    template <typename T, typename S>
    struct max_serialized_blob_size<T, tag::blob<S> >
        : S::template max_serialized_size<T>
    {};

    template <typename T>
    struct max_serialized_blob_size<T, tag::blob<void> >
        : boost::mpl::int_<-1>
    {};
}

#endif // #if !defined( CRYO_SCHEMA_TAG_BLOB_HPP )
