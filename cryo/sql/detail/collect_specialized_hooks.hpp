///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DETAIL_COLLECT_SPECIALIZED_HOOKS_HPP )
#define CRYO_SQL_DETAIL_COLLECT_SPECIALIZED_HOOKS_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension/unspecialized_hook.hpp>
#include <cryo/schema/introspect/column_type_of.hpp>
#include <cryo/schema/introspect/all_tags_of.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace cryo { namespace sql { namespace detail
{
    template <typename C, typename MakeHook>
    struct collect_specialized_hooks
    {
        typedef typename column_type_of<C>::type column_type;

        typedef typename all_tags_of<typename C::field_type>::type all_tags;

        // if the column is the primary key, add the primary key tag
        typedef typename boost::mpl::eval_if_c<
            C::index == 0,
            boost::mpl::push_front<all_tags, tag::primary_key>,
            boost::mpl::identity<all_tags>
        >::type result_tags;

        // transform tags into hooks
        typedef typename boost::mpl::transform<result_tags, MakeHook>::type all_hooks;

        // filter all hooks that aren't specialized
        typedef typename boost::mpl::remove_if<
            all_hooks, boost::is_base_of<extension::unspecialized_hook, boost::mpl::_1>
        >::type type;
    };
}}}

#endif // #if !defined( CRYO_SQL_DETAIL_COLLECT_SPECIALIZED_HOOKS_HPP )
