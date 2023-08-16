///////////////////////////////////////////////////////////////////////////////
//
//! \file join_index_set.hpp
//! Contains the \ref join_index_set type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_JOIN_INDEX_SET_HPP )
#define CRYO_SQL_DYNAMIC_JOIN_INDEX_SET_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/column_node.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/intrusive/options.hpp>
#include <algorithm>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    struct column_node_less
    {
        bool operator()(column_node const& lhs, column_node const& rhs) const
        {
            column_node::column_range_t lhs_rng = lhs.column_range(),
                                        rhs_rng = rhs.column_range();

            return std::lexicographical_compare(lhs_rng.begin(), lhs_rng.end(), rhs_rng.begin(), rhs_rng.end());
        }
    };

    //! \brief An intrusive set that orders \ref ColumnChains that derive from
    //!        \ref column_node.
    //!
    //! Used to ensure no joins are duplicated in generated SQL.
    typedef boost::intrusive::set<
        column_node,
        boost::intrusive::constant_time_size<true>,
        boost::intrusive::compare<column_node_less>
    > join_index_set;
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_JOIN_INDEX_SET_HPP )
