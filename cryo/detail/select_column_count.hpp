///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_DETAIL_SELECT_COLUMN_COUNT_HPP )
#define CRYO_DETAIL_SELECT_COLUMN_COUNT_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/columns_of.hpp>
#include <cryo/detail/unwrap.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <cryo/schema/blob_reference.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/int.hpp>

namespace cryo
{
    template <typename T, typename Enable = void>
    struct select_column_count;

    template <typename T>
    struct select_column_count<T, typename boost::enable_if<is_entity<T> >::type>
        : boost::mpl::accumulate<
            typename result_of::columns_of<T>::type,
            boost::mpl::int_<0>,
            boost::mpl::if_<
                is_entity<
                    result_of::unwrap<field_type_of<boost::mpl::_2> >
                >,
                boost::mpl::plus<
                    boost::mpl::_1,
                    select_column_count<
                        result_of::unwrap<field_type_of<boost::mpl::_2> >
                    >
                >,
                boost::mpl::if_<
                    boost::is_same<blob_reference, field_type_of<boost::mpl::_2> >,
                    boost::mpl::_1,
                    boost::mpl::next<boost::mpl::_1>
                >
            >
        >::type
    {};

    template <typename T>
    struct select_column_count<T, typename boost::disable_if<is_entity<T> >::type>
        : boost::mpl::int_<1>
    {};
}

#endif // #if !defined( CRYO_DETAIL_SELECT_COLUMN_COUNT_HPP )
