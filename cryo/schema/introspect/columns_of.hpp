///////////////////////////////////////////////////////////////////////////////
//
//! \file columns_of.hpp
//! Contains the \ref columns_of free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_COLUMNS_OF_HPP )
#define CRYO_SCHEMA_INTROSPECT_COLUMNS_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/entity_metadata.hpp>
#include <cryo/schema/entity_column.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/fusion/include/as_vector.hpp>

namespace cryo
{
    namespace result_of
    {
        template <typename Entity, typename N>
        struct nth_column
        {
            typedef entity_column<Entity, N::value> type;
        };

        template <typename Entity>
        struct columns_of
            : boost::mpl::transform<
                typename boost::fusion::result_of::as_vector<
                    boost::mpl::range_c<int, 0, entity_metadata<Entity>::column_count>
                >::type,
                nth_column<Entity, boost::mpl::_1>
            >
        {};
    }

    //! \brief Returns a Boost.Fusion random access sequence of \ref ColumnDescriptors for
    //!        a given entity type.
    //! \tparam Entity the entity type for whom \ref ColumnDescriptors will be created.
    //! \returns a Boost.Fusion random access sequence containing a \ref ColumnDescriptor
    //!          for every one of \c Entity's columns.
    template <typename Entity>
    inline typename result_of::columns_of<Entity>::type columns_of()
    {
        typedef typename result_of::columns_of<Entity>::type result_type;

        return result_type();
    }
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_COLUMNS_OF_HPP )
