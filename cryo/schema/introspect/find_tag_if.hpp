///////////////////////////////////////////////////////////////////////////////
//
//! \file find_tag_if.hpp
//! Contains the \ref find_tag_if<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_FIND_TAG_IF_HPP )
#define CRYO_SCHEMA_INTROSPECT_FIND_TAG_IF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/all_tags_of.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace cryo
{
    //! \brief Finds the first \ref ColumnTag of the column described by \c C for which
    //!        the predicate \c P returns true.
    //! \tparam C the \ref PartialColumnDescriptor describing the column whose tags are
    //!           to be searched.
    //! \tparam P a predicate metafunction.
    //! \returns the first \ref ColumnTag of \c C for which
    //!          <tt>boost::mpl::apply<P, Tag>::value == true</tt>, or \c void if none is found.
    template <typename C, typename P>
    struct find_tag_if
    {
        typedef typename all_tags_of<typename field_type_of<C>::type>::type tags;

        // look for the tag
        typedef typename boost::mpl::find_if<tags, P>::type find_i;

        // get the found tag, if any
        typedef typename boost::mpl::eval_if<
            boost::is_same<find_i, typename boost::mpl::end<tags>::type>,
            boost::mpl::identity<void>,
            boost::mpl::deref<find_i>
        >::type type;
    };
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_FIND_TAG_IF_HPP )
