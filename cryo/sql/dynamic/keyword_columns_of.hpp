///////////////////////////////////////////////////////////////////////////////
//
//! \file keyword_columns_of.hpp
//! Contains the \ref keyword_columns_of free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_KEYWORD_COLUMNS_OF_HPP )
#define CRYO_SQL_DYNAMIC_KEYWORD_COLUMNS_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/entity_metadata.hpp>
#include <cryo/schema/entity_column.hpp>
#include <cryo/sql/dynamic/col.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/vector.hpp>

namespace cryo { namespace sql { namespace dynamic
{
    namespace detail
    {
        template <typename T>
        struct make_keyword_column
        {
            template <typename Sig>
            struct result;

            template <typename N>
            struct result<make_keyword_column(N)>
            {
                typedef keyword::column_path<
                    boost::mpl::vector<
                        typename entity_column<T, N::value>::member_type
                    >
                > type;
            };

            template <typename N>
            typename result<make_keyword_column(N)>::type operator()(N) const
            {
                return typename result<make_keyword_column(N)>::type(
                    entity_column<T, N::value>().member(), 0
                );
            }
        };
    }

    namespace result_of
    {
        //! \brief Metafunction that returns the result of the \ref keyword_columns_of
        //!        function.
        //! \tparam T the entity type whose columns will be retrieved.
        //! \returns a Boost.Fusion random access sequence type containing \ref ColumnChains
        //!          describing each column of \c T.
        template <typename T>
        struct keyword_columns_of
            : boost::fusion::result_of::as_vector<
                typename boost::fusion::result_of::transform<
                    boost::mpl::range_c<int, 0, entity_metadata<T>::column_count>,
                    detail::make_keyword_column<T>
                >::type
            >
        {};
    }

    //! \brief Returns a Boost.Fusion random access sequence containing a \ref ColumnChain
    //!        for each column of \c T.
    //! \tparam T the entity type for whom \ref ColumnChains will be created.
    //! \returns a Boost.Fusion random access sequence containing \ref ColumnChains
    //!          describing each column of \c T.
    template <typename T>
    inline typename result_of::keyword_columns_of<T>::type keyword_columns_of()
    {
        return boost::fusion::as_vector(
            boost::fusion::transform(
                boost::mpl::range_c<int, 0, entity_metadata<T>::column_count>(),
                detail::make_keyword_column<T>()
            )
        );
    }
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_KEYWORD_COLUMNS_OF_HPP )
