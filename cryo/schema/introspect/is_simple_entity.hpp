///////////////////////////////////////////////////////////////////////////////
//
//! \file is_simple_entity.hpp
//! Contains the \ref is_simple_entity<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_IS_SIMPLE_ENTITY_HPP )
#define CRYO_SCHEMA_INTROSPECT_IS_SIMPLE_ENTITY_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/entity_metadata.hpp>
#include <cryo/detail/select_column_count.hpp>
#include <boost/mpl/bool.hpp>

namespace cryo
{
    //TODO: this will fail for entities that have blob_references...
    //! \brief Metafunction that tests whether an entity references other entities
    //!        or not.
    //! \tparam Entity the entity type to check.
    //! \returns \c boost::mpl::true_ if \c Entity's columns do not reference other entities,
    //!          \c boost::mpl::false_ if otherwise.
    template <typename Entity>
    struct is_simple_entity
        : boost::mpl::bool_<
            entity_metadata<Entity>::column_count == select_column_count<Entity>::value
        >
    {};
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_IS_SIMPLE_ENTITY_HPP )
