///////////////////////////////////////////////////////////////////////////////
//
//! \file field_type_of.hpp
//! Contains the \ref field_type_of<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_FIELD_TYPE_OF_HPP )
#define CRYO_SCHEMA_INTROSPECT_FIELD_TYPE_OF_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo
{
    //! \brief Extracts the type of the entity field mapped by a given
    //!        \ref PartialColumnDescriptor.
    //! \remark Any type that models the \ref PartialColumnDescriptor concept must
    //!         specialize this type.
    //! \tparam C the \ref PartialColumnDescriptor to extract the field type of.
    //! \returns the field type of \c C's associated entity field.
    template <typename C>
    struct field_type_of
    {
        // HACK! Needed since many metafunctions used in cryo::detail::switch_ depend on this metafunction.
        // So if field_type_of<C> ends up being undefined, cryo will break.
        typedef void type;
    };

    template <typename T, typename E>
    struct field_type_of<T E::*>
    {
        typedef T type;
    };
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_FIELD_TYPE_OF_HPP )
