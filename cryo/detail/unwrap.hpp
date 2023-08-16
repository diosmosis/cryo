///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_DETAIL_UNWRAP_HPP )
#define CRYO_DETAIL_UNWRAP_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/tag/is_wrapper.hpp>
#include <cryo/schema/introspect/wrapped_type_of.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/utility/enable_if.hpp>

namespace cryo
{
    namespace result_of
    {
        template <typename T>
        struct unwrap
            : boost::mpl::eval_if<
                is_wrapper<T>,
                wrapped_type_of<T>,
                boost::mpl::identity<T>
            >
        {};
    }

    template <typename T>
    inline typename boost::enable_if<
        is_wrapper<T>,
        typename result_of::unwrap<T>::type &
    >::type unwrap(T & x)
    {
        return unwrap(cryo::dereference(x));
    }

    template <typename T>
    inline typename boost::disable_if<
        is_wrapper<T>,
        T &
    >::type unwrap(T & x)
    {
        return x;
    }
}

#endif // #if !defined( CRYO_DETAIL_UNWRAP_HPP )
