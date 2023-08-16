///////////////////////////////////////////////////////////////////////////////
//
//! \file columns_view.hpp
//! Contains the \ref columns_view<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_COLUMNS_VIEW_HPP )
#define CRYO_SCHEMA_COLUMNS_VIEW_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/entity_metadata.hpp>
#include <cryo/schema/entity_column.hpp>
#include <boost/fusion/include/iterator_facade.hpp>
#include <boost/fusion/include/sequence_facade.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_const.hpp>

namespace cryo
{
    template <typename Entity>
    struct columns_view;

    template <typename Entity, int N>
    struct columns_view_iterator
        : boost::fusion::iterator_facade<
            columns_view_iterator<Entity, N>,
            boost::fusion::random_access_traversal_tag
        >
    {
        typedef boost::mpl::int_<N> index;
        typedef columns_view<Entity> sequence_type;

        columns_view_iterator(Entity & entity_)
            : entity(entity_)
        {}

        Entity & entity;

        template <typename I>
        struct value_of;

        template <typename E, int N_>
        struct value_of<columns_view_iterator<E, N_> >
        {
            typedef typename entity_column<E, N_>::field_type type;
        };

        template <typename I>
        struct deref;

        template <typename E, int N_>
        struct deref<columns_view_iterator<E, N_> >
        {
            typedef entity_column<E, N_> column;

            typedef typename
                boost::mpl::if_<
                    boost::is_const<E>,
                    typename column::field_type const&,
                    typename column::field_type &
                >::type
            type;

            static type call(columns_view_iterator<E, N_> const& i)
            {
                return i.entity.*(column().member());
            }
        };

        template <typename I>
        struct next;

        template <typename E, int N_>
        struct next<columns_view_iterator<E, N_> >
        {
            typedef columns_view_iterator<E, N_ + 1> type;

            static type call(columns_view_iterator<E, N_> const& i)
            {
                return type(i.entity);
            }
        };

        template <typename I>
        struct prior;

        template <typename E, int N_>
        struct prior<columns_view_iterator<E, N_> >
        {
            typedef columns_view_iterator<E, N_ - 1> type;

            static type call(columns_view_iterator<E, N_> const& i)
            {
                return type(i.entity);
            }
        };

        template <typename I0, typename I1>
        struct distance
        {
            typedef boost::mpl::int_<I1::index::value - I0::index::value> type;

            static type call(I0 const&, I1 const&)
            {
                return type();
            }
        };

        template<typename It, typename M>
        struct advance;

        template <typename E, int N_, typename M>
        struct advance<columns_view_iterator<E, N_>, M>
        {
            typedef columns_view_iterator<E, N_ + M::value> type;

            static type call(columns_view_iterator<E, N_> const& i)
            {
                return type(i.entity);
            }
        };
    };

    //! \brief A Boost.Fusion random access view that iterates over an entity's
    //!        columns.
    //! \tparam Entity the entity type to wrap.
    template <typename Entity>
    struct columns_view
        : boost::fusion::sequence_facade<
            columns_view<Entity>,
            boost::fusion::random_access_traversal_tag,
            boost::mpl::true_
        >
    {
        typedef Entity entity_type;

        //! \brief Constructor.
        //! \param entity_ the entity whose columns will be accessed.
        columns_view(Entity & entity_)
            : entity(entity_)
        {}

        Entity & entity;

        template <typename S>
        struct begin
        {
            typedef columns_view_iterator<typename S::entity_type, 0> type;

            static type call(S & sq)
            {
                return type(sq.entity);
            }
        };

        template <typename S>
        struct end
        {
            typedef typename S::entity_type entity_type;

            typedef columns_view_iterator<
                entity_type, entity_metadata<entity_type>::column_count
            > type;

            static type call(S & sq)
            {
                return type(sq.entity);
            }
        };

        template<typename Sq>
        struct size
            : boost::mpl::int_<
                entity_metadata<typename Sq::entity_type>::column_count
            >
        {};

        template <typename Sq, typename N>
        struct value_at
        {
            typedef typename
                entity_column<typename Sq::entity_type, N::value>::field_type
            type;
        };

        template <typename Sq, typename N>
        struct at
        {
            typedef entity_column<typename Sq::entity_type, N::value> column;

            typedef typename
                boost::mpl::if_<
                    boost::is_const<Sq>,
                    typename column::field_type const,
                    typename column::value_type
                >::type
            type;

            static type call(Sq & sq)
            {
                return sq.entity.*(column().member());
            }
        };
    };
}

#endif // #if !defined( CRYO_SCHEMA_COLUMNS_VIEW_HPP )
