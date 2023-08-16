///////////////////////////////////////////////////////////////////////////////
//
//! \file calc_join_indices.hpp
//! Contains the \ref calc_join_indices free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_CALC_JOIN_INDICES_HPP )
#define CRYO_SQL_DYNAMIC_CALC_JOIN_INDICES_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/join_index_set.hpp>
#include <cryo/sql/dynamic/condition.hpp>
#include <cryo/sql/dynamic/col.hpp>
#include <cryo/schema/tag/foreign_key.hpp>
#include <boost/proto/proto.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <utility>

namespace cryo { namespace sql { namespace dynamic
{
    template <typename Cols>
    struct set_column_subpath
    {
        set_column_subpath(keyword::column_path<Cols> const& path_, join_index_set & indices_)
            : path(path_)
            , indices(indices_)
        {}

        template <typename N>
        void operator()(N) const
        {
            typedef keyword::column_subpath<Cols, N::value> link_type;

            link_type const& link = static_cast<link_type const&>(path);

            // HACK: need to use const cast here due to a compiler bug in MSVC. When creating expressions, keyword::column
            // is always sent to the const version of whatever operator is being used, and the type of expression created
            // then retains that const. So it's never possible to modify the node unless a const_cast is used.
            column_node & cn = const_cast<column_node &>(static_cast<column_node const&>(link));

            std::pair<join_index_set::iterator, bool> res = indices.insert(cn);

            if (res.second)
            {
                cn.join_idx = indices.size();
            }
            else
            {
                cn.join_idx = res.first->join_idx;
            }
        }

        keyword::column_path<Cols> const& path;
        join_index_set & indices;
    };

    struct set_join_idx : proto::callable
    {
        typedef void result_type;

        template <typename Cols>
        void operator()(keyword::column_path<Cols> const& path, join_index_set & indices) const
        {
            boost::fusion::for_each(
                boost::mpl::range_c<int, 0, keyword::last_join_of<Cols>::value>(),
                set_column_subpath<Cols>(path, indices));
        }

        void operator()(expr_base const& xpr, join_index_set & indices) const
        {
            xpr.calc_join_indices(indices);
        }

        template <typename Expr>
        void operator()(Expr const& xpr, join_index_set & indices, typename Expr::proto_tag * d = 0) const
        {
            calc_join_indices(xpr, indices);
        }
    };

    struct calc_join_indices_impl
        : proto::or_<
            proto::when<
                grammar::column,
                set_join_idx(proto::_value, proto::_data)
            >,
            proto::when<
                grammar::abstract_expr,
                set_join_idx(proto::_value, proto::_data)
            >,
            proto::when<
                proto::terminal<proto::_>
            >,
            proto::when<
                proto::_,
                set_join_idx(proto::_expr, proto::_data)
            >
        >
    {};

    struct calc_join_indices_single
    {
        calc_join_indices_single(join_index_set & join_indices_)
            : join_indices(join_indices_)
        {}

        template <typename Expr>
        typename boost::enable_if<proto::is_expr<Expr>, void>::type operator()(Expr & xpr) const
        {
            int dummy = 0;
            calc_join_indices_impl()(xpr, dummy, boost::ref(join_indices));
        }

        void operator()(expr_base const& xpr) const
        {
            xpr.calc_join_indices(join_indices);
        }

        template <typename Cols>
        void operator()(keyword::column_path<Cols> const& x) const
        {
            set_join_idx sji;
            sji(x, join_indices);
        }

        join_index_set & join_indices;
    };

    //! \brief Determines what joins are needed for a \c SELECT statement and sets the join indices
    //!        of the \ref ColumnChains used.
    //!
    //! This function achieves two results. It makes sure identical joins are not outputted more than
    //! once by ordering every \ref ColumnChain in every part of the query. And it makes sure to
    //! set the join index of each \ref ColumnChain so the table they refer to can be easily and
    //! efficiently recalled.
    //!
    //! \param xprs a Boost.Fusion sequence of cryo expressions. These are the expressions used to
    //!             select data.
    //! \param where_xpr a cryo condition. This parameter is optional and does not have to be supplied.
    //! \param indices the set of all column paths the query described by \c xprs & \c where_xpr takes.
    template <typename ExprTuple, typename WhereExpr, typename IndexSet>
    inline void calc_join_indices(ExprTuple const& xprs, WhereExpr & where_xpr, IndexSet & indices)
    {
        calc_join_indices_single cjis(indices);

        boost::fusion::for_each(xprs, cjis);
        cjis(where_xpr);
    }

    template <typename ExprTuple, typename IndexSet>
    inline void calc_join_indices(ExprTuple const& xprs, IndexSet & indices)
    {
        boost::fusion::for_each(xprs, calc_join_indices_single(indices));
    }
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_CALC_JOIN_INDICES_HPP )
