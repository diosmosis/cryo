///////////////////////////////////////////////////////////////////////////////
//
//! \file is_entity_reference.hpp
//! Contains the \ref is_entity_reference<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_IS_ENTITY_REFERENCE_HPP )
#define CRYO_SCHEMA_INTROSPECT_IS_ENTITY_REFERENCE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <cryo/detail/unwrap.hpp>

namespace cryo
{
    //! \brief Metafunction that tests whether an entity column references another
    //!        entity.
    //! \tparam C a \ref PartialColumnDescriptor describing the column to check.
    //! \returns \c boost::mpl::true_ if \c C's entity field references another entity,
    //!          \c boost::mpl::false_ if otherwise.
    template <typename C>
    struct is_entity_reference
        : is_entity<
            typename result_of::unwrap<
                typename field_type_of<C>::type
            >::type
        >
    {};
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_IS_ENTITY_REFERENCE_HPP )
