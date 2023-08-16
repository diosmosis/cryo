///////////////////////////////////////////////////////////////////////////////
//
//! \file field_decorator.hpp
//! Contains an archetype for the \ref FieldDecorator concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_FIELD_DECORATOR_HPP )
#define CRYO_CONCEPT_FIELD_DECORATOR_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace concepts
{
    //! \brief Concept that categorizes metafunction types used to associate
    //!        \ref ColumnTags with entity fields.
    //!
    //! \c FieldDecorators are used like any other metafunction, but only when
    //! declaring entity field types:
    //! \code
    //! struct my_entity
    //! {
    //!     // autogenerate is a FieldDecorator that associates the tag::autogenerate tag
    //!     // with the &my_entity::id field.
    //!     autogenerate<int>::type id;
    //! };
    //! \endcode
    //! 
    //! \remark More than one \c FieldDecorator can be used on a field by nesting
    //!         \c FieldDecorator invocations.
    //! \remark There is no requirement regarding the number or type of arguments
    //!         the metafunction can accept.
    struct FieldDecorator
    {
        typedef int unspecified;

        //! A type that wraps the type supplied (if any) and can be used to extract
        //! the \ref ColumnTag the \ref FieldDecorator was meant to associate.
        typedef unspecified type;
    };
}}

#endif // #if !defined( CRYO_CONCEPT_FIELD_DECORATOR_HPP )
