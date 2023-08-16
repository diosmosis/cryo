///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_DETAIL_UPDATABLE_COLUMNS_OF_HPP )
#define CRYO_DETAIL_UPDATABLE_COLUMNS_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <cryo/schema/introspect/columns_of.hpp>
#include <cryo/schema/introspect/is_primary_key.hpp>
#include <cryo/schema/blob_reference.hpp>
#include <boost/mpl/remove_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace cryo
{
    namespace result_of
    {
        template <typename T>
        struct updatable_columns_of
            : boost::mpl::remove_if<
                typename result_of::columns_of<T>::type,
                boost::mpl::or_<
                    boost::is_same<blob_reference, field_type_of<boost::mpl::_1> >,
                    is_primary_key<boost::mpl::_1>
                >
            >
        {};
    }

    template <typename T>
    inline typename result_of::updatable_columns_of<T>::type updatable_columns_of()
    {
        return typename result_of::updatable_columns_of<T>::type();
    }
}

#endif // #if !defined( CRYO_DETAIL_UPDATABLE_COLUMNS_OF_HPP )
