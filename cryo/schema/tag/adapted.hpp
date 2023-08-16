///////////////////////////////////////////////////////////////////////////////
//
//! \file adapted.hpp
//! Contains types and functions that map the \c boost::optional<> and
//! \c boost::shared_ptr<> types as types that model the \ref Nullable concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_TAG_ADAPTED_HPP )
#define CRYO_SCHEMA_TAG_ADAPTED_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/all_tags_of.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/bool.hpp>

namespace cryo
{
    // boost::optional<T>
    template <typename T>
    struct wrapped_type_of<boost::optional<T> >
    {
        typedef T type;
    };

    template <typename T>
    struct all_tags_of<boost::optional<T> >
        : boost::mpl::push_front<
            typename all_tags_of<T>::type,
            tag::nullable
        >
    {};

    template <typename T>
    struct is_wrapper<boost::optional<T> >
        : boost::mpl::true_
    {};

    template <typename T>
    inline void set_null(boost::optional<T> & x)
    {
        x = boost::none;
    }

    template <typename T>
    inline void allocate_and_construct(boost::optional<T> & x)
    {
        x = T();
    }

    template <typename T>
    inline T & dereference(boost::optional<T> & x)
    {
        return *x;
    }

    // boost::shared_ptr<T>
    template <typename T>
    struct wrapped_type_of<boost::shared_ptr<T> >
    {
        typedef T type;
    };

    template <typename T>
    struct all_tags_of<boost::shared_ptr<T> >
        : boost::mpl::push_front<
            typename all_tags_of<T>::type,
            tag::nullable
        >
    {};

    template <typename T>
    struct is_wrapper<boost::shared_ptr<T> >
        : boost::mpl::true_
    {};

    template <typename T>
    inline void set_null(boost::shared_ptr<T> & x)
    {
        x.reset();
    }

    template <typename T>
    inline void allocate_and_construct(boost::shared_ptr<T> & x)
    {
        x.reset(new T());
    }

    template <typename T>
    inline T & dereference(boost::shared_ptr<T> & x)
    {
        return *x;
    }
}

#endif // #if !defined( CRYO_SCHEMA_TAG_ADAPTED_HPP )
