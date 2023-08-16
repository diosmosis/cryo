///////////////////////////////////////////////////////////////////////////////
//
//! \file col.hpp
//! Contains the \ref col free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_COL_HPP )
#define CRYO_SQL_DYNAMIC_COL_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/column_node.hpp>
#include <cryo/sql/dynamic/output_join.hpp>
#include <cryo/schema/tag/foreign_key.hpp>
#include <cryo/schema/introspect/foreign_entity_type_of.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <cryo/detail/unwrap.hpp>
#include <boost/proto/proto.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/vector.hpp>
#include <algorithm>

namespace cryo { namespace sql { namespace dynamic
{
    namespace proto = boost::proto;

    namespace keyword
    {
        template <typename Columns>
        struct last_join_of
            : boost::mpl::if_<
                is_foreign_key<typename boost::mpl::back<Columns>::type>,
                typename boost::mpl::size<Columns>::type,
                boost::mpl::int_<
                    boost::mpl::size<Columns>::value - 1
                >
            >::type
        {};

        template <typename Columns>
        struct at_dead_end
            : boost::mpl::bool_<
                last_join_of<Columns>::value != boost::mpl::size<Columns>::value
            >
        {};

        template <typename Columns>
        struct column_path;

        template <typename Cols, int N>
        struct column_subpath
            : column_node
        {
            typedef column_path<Cols>                                           derived_type;
            typedef typename boost::mpl::at_c<Cols, N>::type                    column;
            typedef typename boost::mpl::size<Cols>::type                       path_size;
            typedef typename foreign_entity_type_of<column>::type               foreign_entity;
            typedef column_subpath<Cols, N - 1>                                 previous_link;

            enum
            {
                in_left_join = is_nullable<column>::value || previous_link::in_left_join
            };

            column_range_t column_range() const
            {
                return column_range_t(derived().chain, derived().chain + N + 1);
            }

            void output_join(std::stringstream & ss, extension::abstract_hooks const& hooks) const
            {
                sql::dynamic::output_join<foreign_entity>(ss, hooks, in_left_join, join_idx, previous().join_idx, as_ptr());
            }

            column as_ptr() const
            {
                return *reinterpret_cast<column const*>(derived().chain + N);
            }

            derived_type const& derived() const { return *static_cast<derived_type const*>(this); }
            previous_link const& previous() const { return derived(); }
        };

        template <typename Cols>
        struct column_subpath<Cols, 0>
            : column_node
        {
            typedef column_path<Cols>                                           derived_type;
            typedef typename boost::mpl::at_c<Cols, 0>::type                    column;
            typedef typename boost::mpl::size<Cols>::type                       path_size;
            typedef typename foreign_entity_type_of<column>::type               foreign_entity;

            enum
            {
                in_left_join = is_nullable<column>::value
            };

            column_range_t column_range() const
            {
                return column_range_t(derived().chain, derived().chain + 1);
            }

            void output_join(std::stringstream & ss, extension::abstract_hooks const& hooks) const
            {
                sql::dynamic::output_join<foreign_entity>(ss, hooks, in_left_join, join_idx, 0, as_ptr());
            }

            column as_ptr() const
            {
                return *reinterpret_cast<column const*>(derived().chain);
            }

            derived_type const& derived() const { return *static_cast<derived_type const*>(this); }
        };

        template <typename Cols, typename N>
        struct make_column_subpath
        {
            typedef column_subpath<Cols, N::value> type;
        };

        template <typename Columns>
        struct column_path
            : boost::mpl::inherit_linearly<
            boost::mpl::range_c<int, 0, last_join_of<Columns>::value>,
            boost::mpl::inherit<
                boost::mpl::_1,
                make_column_subpath<Columns, boost::mpl::_2>
            >
        >::type
        {
            typedef Columns                                                 columns;
            typedef column_path<Columns>                                    self_type;
            typedef typename boost::mpl::size<Columns>::type                path_size;
            typedef typename boost::mpl::back<Columns>::type                last_column;

            column_path(last_column last, unsigned int const* front)
            {
                std::copy(front, front + path_size::value - 1, chain);
                chain[path_size::value - 1] = *reinterpret_cast<unsigned int const*>(&last);
            }

            column_path(self_type const& x)
            {
                std::copy(x.chain, x.chain + path_size::value, chain);
            }

            last_column as_ptr() const
            {
                return *reinterpret_cast<last_column const*>(chain + path_size::value - 1);
            }

            unsigned int chain[path_size::value];
        };

        template <typename ColumnPath, typename Enable = void>
        struct column_terminal;

        template <typename ColumnImpl>
        struct column_terminal<
            ColumnImpl,
            typename boost::disable_if<
                at_dead_end<typename ColumnImpl::columns>
            >::type
        > : proto::extends<typename proto::terminal<ColumnImpl>::type, column_terminal<ColumnImpl> >
        {
            typedef typename ColumnImpl::columns                        columns;
            typedef typename foreign_entity_type_of<ColumnImpl>::type   foreign_entity;

            typedef
                typename proto::terminal<ColumnImpl>::type
            terminal_type;

            typedef
                proto::extends<typename proto::terminal<ColumnImpl>::type, column_terminal<ColumnImpl> >
            base_type;

            column_terminal(terminal_type const& term)
                : base_type(term)
            {}

            column_terminal(column_terminal const& x)
                : base_type(x)
            {}

            template <typename FieldType>
            column_terminal<
                column_path<
                    typename boost::mpl::push_back<columns, FieldType foreign_entity::*>::type
                >
            > operator()(FieldType foreign_entity::* dependent) const
            {
                typedef column_path<
                    typename boost::mpl::push_back<columns, FieldType foreign_entity::*>::type
                > column_type;
                typedef column_terminal<column_type> result_type;

                typename result_type::terminal_type term = {column_type(dependent, proto::value(*this).chain)};
                return result_type(term);
            }
        };

        template <typename ColumnImpl>
        struct column_terminal<
            ColumnImpl,
            typename boost::enable_if<
                at_dead_end<typename ColumnImpl::columns>
            >::type
        > : proto::extends<typename proto::terminal<ColumnImpl>::type, column_terminal<ColumnImpl> >
        {
            typedef
                typename proto::terminal<ColumnImpl>::type
            terminal_type;

            typedef
                proto::extends<terminal_type, column_terminal<ColumnImpl> >
            base_type;

            column_terminal(terminal_type const& term)
                : base_type(term)
            {}

            column_terminal(column_terminal const& x)
                : base_type(x)
            {}
        };
    }

    namespace detail
    {
        template <typename Columns>
        inline typename boost::enable_if_c<
            keyword::last_join_of<Columns>::value != 0, unsigned int
        >::type join_idx_of(keyword::column_path<Columns> const& col)
        {
            typedef keyword::column_subpath<
                Columns, keyword::last_join_of<Columns>::value - 1
            > link_type;

            return static_cast<link_type const&>(col).join_idx;
        }

        template <typename Columns>
        inline typename boost::disable_if_c<
            keyword::last_join_of<Columns>::value != 0, unsigned int
        >::type join_idx_of(keyword::column_path<Columns> const& col)
        {
            return 0;
        }
    }

    //! \brief Creates and returns a \ref ColumnChain that can be used to navigate
    //!        through an entity and the entities it references.
    //! \param col a pointer-to-member describing the starting point of the column
    //!        chain.
    //! \returns a \ref ColumnChain that refers to \c col.
    template <typename Entity, typename T>
    inline keyword::column_terminal<
        keyword::column_path<
            boost::mpl::vector<T Entity::*>
        >
    > col(T Entity::* col)
    {
        typedef keyword::column_path<boost::mpl::vector<T Entity::*> > keyword_type;
        typedef keyword::column_terminal<keyword_type> result_type;

        typename result_type::terminal_type term = {keyword_type(col, 0)};
        return result_type(term);
    }
}}}

namespace cryo
{
    template <typename C>
    struct field_type_of<sql::dynamic::keyword::column_path<C> >
        : field_type_of<
            typename boost::mpl::back<C>::type
        >
    {};

    template <typename C>
    struct field_type_of<sql::dynamic::keyword::column_terminal<C> >
        : field_type_of<C>
    {};
}

#endif // #if !defined( CRYO_SQL_DYNAMIC_COL_HPP )
