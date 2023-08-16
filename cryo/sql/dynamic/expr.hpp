///////////////////////////////////////////////////////////////////////////////
//
//! \file expr.hpp
//! Contains the \ref expr<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_EXPR_HPP )
#define CRYO_SQL_DYNAMIC_EXPR_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/expr_base.hpp>
#include <cryo/sql/dynamic/expr_result_of.hpp>
#include <cryo/concept/relational_database_connection.hpp>
#include <boost/type_traits/is_convertible.hpp>

namespace cryo { namespace sql { namespace dynamic
{
    //! \brief Stores any cryo expression that evaluates to a type convertible to \c Result.
    //! \remark \ref expr is not thread safe and must be copied before being used by
    //!         more than one thread.
    //! \param Result the result type of the stored expression.
    template <typename Result>
    struct expr : expr_base
    {
        //! \brief Default constructor.
        expr()
            : expr_base()
        {}

        //! \brief Copy constructor. Performs a deep copy on \x's stored expression.
        //! \param x the \ref expr<> instance to copy.
        expr(expr<Result> const& x)
            : expr_base(x)
        {}

        //! \brief Constructor. Stores the supplied expression.
        //! \param x the cryo expression to store. Must match the \c cryo::sql::dynamic::grammar::expr
        //!          grammar and <tt>expr_result_of<concepts::RelationalDatabaseConnection, Expr>::type</tt>
        //!          must be convertible to \c Result.
        template <typename Expr>
        expr(Expr const& x)
            : expr_base(x)
        {
            // HACK: database_connection shouldn't be used here, should be the actual connection type
            static_assert(
                boost::is_convertible<
                    typename expr_result_of<
                        concepts::RelationalDatabaseConnection, Expr
                    >::type,
                    Result
                >::value,
                "The result of the expression Expr must be convertible to the Result type."
            );
        }
    };
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_EXPR_HPP )
