///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_DETAIL_READ_COLUMN_HPP )
#define CRYO_DETAIL_READ_COLUMN_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/tag/is_wrapper.hpp>
#include <cryo/detail/unwrap.hpp>
#include <cryo/schema/tag/nullable.hpp>
#include <cryo/schema/tag/blob.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <cryo/schema/columns_view.hpp>
#include <cryo/detail/select_column_count.hpp>
#include <cryo/detail/make_member_ptr.hpp>
#include <cryo/sql/detail/switch.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/not.hpp>
#include <boost/fusion/include/filter_if.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/assert.hpp>
#include <string>

namespace cryo { namespace detail
{
    template <typename Cursor>
    struct read_column
        : cryo::sql::detail::switch_<
            read_column<Cursor>,
            void(),
            boost::mpl::vector<
                is_nullable<make_member_ptr<boost::mpl::_1> >,
                is_blob<make_member_ptr<boost::mpl::_1> >,
                is_wrapper<boost::mpl::_1>,
                is_entity<boost::mpl::_1>,
                boost::fusion::traits::is_sequence<boost::mpl::_1>
            >
        >
    {
        read_column(Cursor * cursor_)
            : entity(0)
            , cursor(cursor_)
        {}

        // is_nullable
        template <typename T>
        void case_(T & value, boost::mpl::int_<0>) const
        {
            typedef typename cryo::result_of::unwrap<T>::type unwrapped_type;

            if (cursor->is_column_null())
            {
                cryo::set_null(value);
                // TODO: If I make cursor's iterators (over columns & rows (multi dimensional iterator?)), I could just
                // std::advance my way ahead.
                for (unsigned int i = 0; i != select_column_count<unwrapped_type>::value; ++i)
                {
                    cursor->next_column();
                }
            }
            else
            {
                cryo::allocate_and_construct(value);
                (*this)(cryo::dereference(value));
            }
        }

        // is_blob
        template <typename T>
        void case_(T & value, boost::mpl::int_<1>) const
        {
            typedef typename find_tag_if<
                typename make_member_ptr<T>::type,
                is_blob_tag<boost::mpl::_1>
            >::type blob_tag;
            typedef typename blob_tag::serializer_type serializer_type;

            // create the serializer
            serializer_type s;

            // get the blob data
            typename Cursor::blob_source_type blob_source = cursor->get_column_blob();
            cursor->next_column();

            // attempt to deserialize the blob data into the value
            s.deserialize(blob_source, value);
        }

        // is_wrapper
        template <typename T>
        void case_(T & value, boost::mpl::int_<2>) const
        {
            (*this)(cryo::dereference(value));
        }

        // is_entity
        template <typename T>
        void case_(T & value, boost::mpl::int_<3>) const
        {
            typedef boost::mpl::not_<boost::is_same<blob_reference, boost::mpl::_1> > pred_type;

            void * store = entity;
            entity = &value;

            boost::fusion::for_each(boost::fusion::filter_if<pred_type>(columns_view<T>(value)), *this);

            entity = store;
        }

        // is a fusion sequence
        template <typename T>
        void case_(T & value, boost::mpl::int_<4>) const
        {
            boost::fusion::for_each(value, *this);
        }

        // is a primitive type or unrecognized type
        template <typename T>
        void default_(T & value) const
        {
            cursor->get_column(value);
            cursor->next_column();
        }

        mutable void * entity;
        Cursor * cursor;
    };
}}

#endif // #if !defined( CRYO_DETAIL_READ_COLUMN_HPP )
