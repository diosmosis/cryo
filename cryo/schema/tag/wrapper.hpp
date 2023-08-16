///////////////////////////////////////////////////////////////////////////////
//
//! \file wrapper.hpp
//! Contains the \ref wrapper<> utility type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_TAG_WRAPPER_HPP )
#define CRYO_SCHEMA_TAG_WRAPPER_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/all_tags_of.hpp>
#include <cryo/schema/tag/is_wrapper.hpp>
#include <cryo/schema/introspect/wrapped_type_of.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/type_traits/is_scalar.hpp>
#include <boost/utility/enable_if.hpp>

namespace cryo
{
    //! \brief A type that wraps another type in order to associate a set of
    //!        \ref ColumnTags with a field type.
    //!
    //! This type will behave in the same way as the supplied field type, \c T.
    //!
    //! \tparam T the type being wrapped.
    //! \tparam Tags an Boost.MPL random access sequence of \ref ColumnTags.
    template <typename T, typename Tags, typename Enable = void>
    struct wrapper;

    template <typename T, typename Tags>
    struct wrapper<T, Tags, typename boost::disable_if<boost::is_scalar<T> >::type>
        : public T
    {
        wrapper()
            : T()
        {}

        wrapper(T const& x)
            : T(x)
        {}

        T & operator = (wrapper const& x)
        {
            *static_cast<T *>(this) = static_cast<T const&>(x);
            return *this;
        }

        T & operator = (T const& x)
        {
            *static_cast<T *>(this) = x;
            return *this;
        }
    };

    template <typename T, typename Tags>
    struct wrapper<T, Tags, typename boost::enable_if<boost::is_scalar<T> >::type>
    {
        wrapper() : data() {}
        wrapper(T x) : data(x) {}
        wrapper(wrapper const& x) : data(x.data) {}

        operator T &()
        {
            return data;
        }

        operator T const&() const
        {
            return data;
        }

        T & operator = (T x)
        {
            data = x;
            return data;
        }

        T data;
    };

    template <typename T, typename Tags>
    inline typename boost::enable_if<boost::is_scalar<T>, T &>::type dereference(wrapper<T, Tags> & x)
    {
        return x.data;
    }

    template <typename T, typename Tags>
    inline typename boost::enable_if<boost::is_scalar<T>, T const&>::type dereference(wrapper<T, Tags> const& x)
    {
        return x.data;
    }

    template <typename T, typename Tags>
    inline typename boost::disable_if<boost::is_scalar<T>, T &>::type dereference(wrapper<T, Tags> & x)
    {
        return x;
    }

    template <typename T, typename Tags>
    inline typename boost::disable_if<boost::is_scalar<T>, T const&>::type dereference(wrapper<T, Tags> const& x)
    {
        return x;
    }

    template <typename T, typename Tag>
    struct is_wrapper<wrapper<T, Tag> >
        : boost::mpl::true_
    {};

    template <typename T, typename Tags>
    struct wrapped_type_of<wrapper<T, Tags> >
    {
        typedef typename T type;
    };

    template <typename T, typename Tag>
    struct make_wrapper
    {
        typedef wrapper<T, boost::mpl::vector<Tag> > type;
    };

    template <typename T, typename Tags, typename Tag>
    struct make_wrapper<wrapper<T, Tags>, Tag>
    {
        typedef wrapper<T, typename boost::mpl::push_front<Tags, Tag>::type> type;
    };

    template <typename T, typename Tags>
    struct all_tags_of<wrapper<T, Tags> >
        : boost::mpl::insert_range<
            typename all_tags_of<T>::type,
            typename boost::mpl::begin<
                typename all_tags_of<T>::type
            >::type,
            Tags
        >
    {};
}

#endif // #if !defined( CRYO_SCHEMA_TAG_WRAPPER_HPP )
