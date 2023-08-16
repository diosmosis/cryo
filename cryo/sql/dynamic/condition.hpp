///////////////////////////////////////////////////////////////////////////////
//
//! \file condition.hpp
//! Contains the \ref condition type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_CONDITION_HPP )
#define CRYO_SQL_DYNAMIC_CONDITION_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/expr.hpp>

namespace cryo { namespace sql { namespace dynamic
{
    //! \brief Stores any cryo expression that results in a \c bool.
    //! \remark Aggregate function calls are not allowed in expressions stored by this type.
    //! \remark \ref condition is not thread safe and must be copied before being used by
    //!         more than one thread.
    struct condition : expr<bool>
    {
        typedef expr<bool> base_type;

        //! \brief Default constructor.
        condition()
            : base_type()
        {}

        //! \brief Copy constructor. Perform's a deep copy on q's stored expression.
        //! \param q the \ref condition to copy.
        condition(condition const& q)
            : base_type(q)
        {}

        //! \brief Constructor. Stores a deep copy of \c xpr.
        //! \param the cryo expression to store. Must match the \c cryo::sql::dynamic::grammar::condition
        //!        grammar, which disallows aggregate function calls.
        template <typename Expr>
        condition(Expr const& xpr, typename Expr::proto_tag * d = 0)
            : base_type(xpr)
        {
            static_assert(proto::matches<Expr, grammar::where_clause>::value, "Expr is not a valid condition.");
        }
    };
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_CONDITION_HPP )
