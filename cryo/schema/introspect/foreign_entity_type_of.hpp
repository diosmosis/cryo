///////////////////////////////////////////////////////////////////////////////
//
//! \brief foreign_entity_type_of.hpp
//! Contains the \ref foreign_entity_type_of<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_FOREIGN_ENTITY_TYPE_OF_HPP )
#define CRYO_SCHEMA_INTROSPECT_FOREIGN_ENTITY_TYPE_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/find_tag_if.hpp>
#include <cryo/schema/tag/foreign_key.hpp>

namespace cryo
{
    //! \brief Gets the foreign entity type of a column with the \ref tag::foreign_key<>
    //!        column tag.
    //! \tparam C the \ref PartialColumnDescriptor to check. The field type of this column
    //!           must have the \ref tag::foreign_key<> \ref ColumnTag.
    template <typename C>
    struct foreign_entity_type_of
    {
        // make sure C is an entity reference
        static_assert(is_foreign_key<C>::value, "The column C does not reference another entity.");

        // get the foreign key tag
        typedef typename find_tag_if<
            C, detail::is_foreign_key_tag<boost::mpl::_1>
        >::type tag_type;

        // get the foreign entity type
        typedef typename tag_type::type type;
    };
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_FOREIGN_ENTITY_TYPE_OF_HPP )
