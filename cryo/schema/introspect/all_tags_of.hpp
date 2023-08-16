///////////////////////////////////////////////////////////////////////////////
//
//! \file all_tags_of.hpp
//! Contains the \ref all_tags_of<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_ALL_TAGS_OF_HPP )
#define CRYO_SCHEMA_INTROSPECT_ALL_TAGS_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/is_entity.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/vector.hpp>

namespace cryo
{
    //! \brief Metafunction that gets the \ref ColumnTags associated with a column through
    //!        the column's associated field type.
    //! \tparam T the entity column's field type.
    template <typename T>
    struct all_tags_of
    {
        typedef boost::mpl::vector<
            typename boost::mpl::if_<
                is_entity<T>,
                tag::foreign_key<T>,
                tag::primitive
            >::type
        > type;
    };
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_ALL_TAGS_OF_HPP )
