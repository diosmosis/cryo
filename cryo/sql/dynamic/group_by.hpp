///////////////////////////////////////////////////////////////////////////////
//
//! \brief group_by.hpp
//! Contains the \ref group_by free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_GROUP_BY_HPP )
#define CRYO_SQL_DYNAMIC_GROUP_BY_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/col.hpp>
#include <cryo/schema/tag/foreign_key.hpp>
#include <cryo/schema/introspect/foreign_entity_type_of.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <boost/utility/enable_if.hpp>

namespace cryo { namespace sql { namespace dynamic
{
    namespace keyword
    {
        template <typename Columns>
        struct group_by<
            Columns
          , typename boost::disable_if<
                at_dead_end<Columns>
            >::type
        > : column_path<Columns>
        {
            typedef column_path<Columns>                                            base_type;
            typedef typename foreign_entity_type_of<column_path<Columns> >::type    foreign_entity_type;
            typedef typename column_path<Columns>::last_column                      last_column;

            group_by(last_column col, member_ptr_int_t const* rest)
                : base_type(col, rest)
            {}

            template <typename F>
            group_by<
                typename boost::mpl::push_back<columns, F foreign_entity_type::*>::type
            > operator()(F foreign_entity_type::* dependent) const
            {
                typedef group_by<
                    typename boost::mpl::push_back<columns, F foreign_entity_type::*>::type
                > result_type;
                return result_type(dependent, this->chain);
            }
        };

        template <typename Columns>
        struct group_by<
            Columns
          , typename boost::enable_if<
                at_dead_end<Columns>
            >::type
        > : column_path<Columns>
        {
            typedef column_path<Columns>                        base_type;
            typedef typename column_path<Columns>::last_column  last_column;

            group_by(last_column col, member_ptr_int_t const* rest)
                : base_type(col, rest)
            {}
        };
    }

    //! \brief Returns a \ref ColumnChain that consists of the supplied column and signifies
    //!        to cryo that the supplied column should be used to group search results.
    //!
    //! This function can only be used in a call that selects data. When used it
    //! will result in a \c SELECT statement that groups its results using a
    //! <tt>GROUP BY</tt> clause.
    //!
    //! If other columns are selected, and are not aggregated, the use of \ref group_by
    //! will result in invalid SQL.
    //!
    //! \param col a pointer-to-member that describes the column to group by.
    //! \returns a \ref ColumnChain that consists of \c col.
    template <typename T, typename Entity>
    inline keyword::group_by<boost::mpl::vector<T Entity::*> > group_by(T Entity::* col)
    {
        keyword::group_by<boost::mpl::vector<T Entity::*> > result(col, 0);
        return result;
    }
}}}

namespace cryo
{
    template <typename C, typename R>
    struct field_type_of<sql::dynamic::keyword::group_by<C, R> >
        : field_type_of<C>
    {};
}

#endif // #if !defined( CRYO_SQL_DYNAMIC_GROUP_BY_HPP )
