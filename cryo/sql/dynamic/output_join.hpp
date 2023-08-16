///////////////////////////////////////////////////////////////////////////////
//
//! \file output_join.hpp
//! Contains the \ref output_join free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_OUTPUT_JOIN_HPP )
#define CRYO_SQL_DYNAMIC_OUTPUT_JOIN_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/table_name_of.hpp>
#include <cryo/schema/introspect/column_name_of.hpp>
#include <cryo/schema/introspect/primary_key_column_of.hpp>
#include <cryo/sql/output_column_name.hpp>
#include <cryo/extension/abstract_hooks.hpp>
#include <boost/mpl/or.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    //! \brief Outputs a left or inner join.
    //! \param ss the \c std::stringstream to output to.
    //! \param hooks contains database specific SQL generation logic used to vary SQL generation
    //!              without knowing the database type.
    //! \param in_left_join \c true if a left join should be outputted, \c false if an inner join
    //!                     should be outputted.
    //! \param join_idx the join index. This number determines what the table alias of this join
    //!                 will be.
    //! \param base_join the join index of the table this join is being joined from.
    //! \param base_cal a \ref PartialColumnDescriptor describing the column of the
    //!                 base join table being joined on.
    template <typename Entity, typename C>
    inline void output_join(
        std::stringstream & ss
      , extension::abstract_hooks const& hooks
      , bool in_left_join
      , unsigned int join_idx
      , unsigned int base_join
      , C base_col
    )
    {
        // output join type
        if (in_left_join)
        {
            ss << " LEFT";
        }
        else
        {
            ss << " INNER";
        }
        ss << " JOIN ";

        // output table name & alias
        hooks.output_table_name(ss, table_name_of<Entity>());
        ss << " t" << join_idx;

        // output join 'on' clause
        ss << " ON t" << join_idx << '.';
        ss << column_name_of(primary_key_column_of<Entity>());
        ss << "=t" << base_join << '.' << column_name_of(base_col);
    }
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_OUTPUT_JOIN_HPP )
