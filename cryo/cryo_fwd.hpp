///////////////////////////////////////////////////////////////////////////////
//
//! \file cryo_fwd.hpp
//! Contains forward declarations for select bits of cryo.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CRYO_FWD_HPP )
#define CRYO_CRYO_FWD_HPP

#include <cryo/config.hpp>
#include <boost/mpl/bool.hpp>

namespace cryo
{
    namespace concepts
    {
        struct RelationalDatabaseConnection;
    }

    namespace tag
    {
        template <int Length>
        struct varchar;

        struct autogenerate;

        template <typename ForeignEntity>
        struct foreign_key;

        template <typename Serializer>
        struct blob;

        struct primitive {};

        struct primary_key {};

        struct nullable;

        struct date;
        struct datetime;
    }

    template <typename T>
    struct is_entity;

    namespace sql { namespace dynamic
    {
        namespace keyword
        {
            template <typename Columns>
            struct column_path;

            template <typename Columns, typename Enable = void>
            struct group_by;
        }

        struct expr_base;

        template <typename Result>
        struct expr;

        struct condition;

        template <typename Expr, typename Hooks>
        inline void output_sql(std::stringstream & ss, Hooks const& hooks, Expr const& xpr);

        template <typename ExprTuple, typename WhereExpr, typename IndexSet>
        inline void calc_join_indices(ExprTuple const& xprs, WhereExpr & where_xpr, IndexSet & indices);

        template <typename ExprTuple, typename IndexSet>
        inline void calc_join_indices(ExprTuple const& xprs, IndexSet & indices);
    }}
}

#endif // #if !defined( CRYO_CRYO_FWD_HPP )
