///////////////////////////////////////////////////////////////////////////////
//
//! \file has_tag.hpp
//! Contains the \ref has_tag<> \ref has_tag_if<> metafunctions.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_HAS_TAG_HPP )
#define CRYO_SCHEMA_INTROSPECT_HAS_TAG_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/all_tags_of.hpp>
#include <cryo/schema/introspect/find_tag_if.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

namespace cryo
{
    //! \brief A metafunction that checks whether the supplied predicate returns true for
    //!        any of \c C's column tags.
    //! \tparam C the \ref PartialColumnDescriptor to check.
    //! \tparam P a predicate metafunction to invoke on each of \c C's column tags.
    template <typename C, typename P>
    struct has_tag_if
        : boost::mpl::not_<
            typename boost::is_same<
                typename find_tag_if<C, P>::type, void
            >::type
        >
    {};

    //! \brief A metafunction that checks if the column described by \ref C has a certain tag.
    //! \tparam C the \ref PartialColumnDescriptor describing the column to check.
    //! \tparam T the \ref FieldTag to check for.
    template <typename C, typename T>
    struct has_tag
        : has_tag_if<
            C, boost::is_same<T, boost::mpl::_1>
        >
    {};
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_HAS_TAG_HPP )
