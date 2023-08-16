///////////////////////////////////////////////////////////////////////////////
//
//! \file column_type_of.hpp
//! Contains the \ref column_type_of<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_COLUMN_TYPE_OF_HPP )
#define CRYO_SCHEMA_INTROSPECT_COLUMN_TYPE_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/entity_column.hpp>
#include <cryo/schema/tag/is_wrapper.hpp>
#include <cryo/schema/introspect/wrapped_type_of.hpp>
#include <cryo/schema/introspect/primary_key_of.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <boost/utility/enable_if.hpp>

namespace cryo
{
    namespace detail
    {
        template <typename T, bool IsEntity = is_entity<T>::value, bool IsWrapper = is_wrapper<T>::value>
        struct extract_column_type;

        template <typename T>
        struct extract_column_type<T, false, false>
        {
            typedef T type;
        };

        template <typename T>
        struct extract_column_type<T, false, true>
            : extract_column_type<typename wrapped_type_of<T>::type>
        {};

        template <typename T>
        struct extract_column_type<T, true, false>
            : extract_column_type<typename entity_column<T, 0>::field_type>
        {};
    }

    //! \brief Returns a C++ primitive representing how a column will exist in a database,
    //!        unless no C++ analog exists (as is the case for BLOB columns).
    //!
    //! This metafunction will strip all meta information from a column's field type,
    //! including tags and wrapper types. For foreign entity references, this metafunction
    //! will return the column type of the foreign entity's primary key column.
    //!
    //! \tparam C a \ref PartialColumnDescriptor describing the column whose stripped field
    //!         type is to be computed.
    template <typename C>
    struct column_type_of
        : detail::extract_column_type<typename field_type_of<C>::type>
    {};
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_COLUMN_TYPE_OF_HPP )
