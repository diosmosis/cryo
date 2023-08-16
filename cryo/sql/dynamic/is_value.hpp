///////////////////////////////////////////////////////////////////////////////
//
//! \file is_value.hpp
//! Contains the \ref is_value<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_IS_VALUE_HPP )
#define CRYO_SQL_DYNAMIC_IS_VALUE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/tag/wrapper.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/mpl/bool.hpp>

namespace cryo { namespace sql { namespace dynamic
{
    //! \brief A metafunction predicate that tells whether a C++ type can be used
    //!        as a value type in SQL.
    //!
    //! \tparam T the C++ type in question.
    //! \returns \c boost::mpl::true_ if \c T can be represented in SQL, \c boost::mpl::false_
    //!          if otherwise.
    template <typename T, typename Enable = void>
    struct is_value
        : boost::mpl::false_
    {};

    template <typename T>
    struct is_value<T const>
        : boost::mpl::false_
    {};

    template <typename T>
    struct is_value<T, typename boost::enable_if<boost::is_integral<T> >::type>
        : boost::mpl::true_
    {};

    template <typename T>
    struct is_value<T, typename boost::enable_if<boost::is_floating_point<T> >::type>
        : boost::mpl::true_
    {};

    template <> struct is_value<std::string> : boost::mpl::true_ {};
    template <> struct is_value<char *> : boost::mpl::true_ {};
    template <> struct is_value<char const*> : boost::mpl::true_ {};
    template <int N> struct is_value<char[N]> : boost::mpl::true_ {};
    template <int N> struct is_value<char const[N]> : boost::mpl::true_ {};

    template <typename T, typename Tags>
    struct is_value<wrapper<T, Tags> >
        : is_value<T>
    {};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_IS_VALUE_HPP )
