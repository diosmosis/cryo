///////////////////////////////////////////////////////////////////////////////
//
//! \file output_boolean_aggregates.hpp
//! Contains the \ref output_boolean_aggregates free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_OUTPUT_BOOLEAN_AGGREGATES_HPP )
#define CRYO_SQL_DYNAMIC_OUTPUT_BOOLEAN_AGGREGATES_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/grammar.hpp>
#include <cryo/sql/dynamic/output_function_args.hpp>
#include <cryo/sql/dynamic/arg.hpp>
#include <cryo/sql/dynamic/condition.hpp>
#include <cryo/extension/abstract_hooks.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    struct output_boolean_aggregate : proto::callable
    {
        typedef int result_type;

        template <typename Expr>
        typename boost::disable_if<
            boost::is_base_of<expr_base, Expr>, int
        >::type operator()(std::stringstream & ss, extension::abstract_hooks const& hooks, Expr const& xpr) const
        {
            typedef typename proto::result_of::value<
                typename proto::result_of::child_c<Expr, 0>::type
            >::type function_type;

            ss << ", ";
            function_type::apply(ss);
            ss << '(';
            output_function_args(ss, hooks, xpr);
            ss << ')';

            return 0;
        }

        template <typename E>
        typename boost::enable_if<
            boost::is_base_of<expr_base, E>, int
        >::type operator()(std::stringstream & ss, extension::abstract_hooks const& hooks, E const& xpr) const
        {
            xpr.output_boolean_aggregates(ss, hooks);
            return 0;
        }
    };

    struct output_boolean_aggregates_impl
        : proto::or_<
            proto::when<
                grammar::boolean_aggregate_call,
                output_boolean_aggregate(_stream, _hooks, proto::_expr)
            >,
            proto::when<
                grammar::abstract_expr,
                output_boolean_aggregate(_stream, _hooks, proto::_value)
            >,
            proto::nary_expr<proto::_, proto::vararg<output_boolean_aggregates_impl> >
        >
    {};

    //! \brief Outputs every boolean aggregate function call in the given cryo expression
    //!        as a list with a comma preceding each function call.
    //! \param ss the \c std::stringstream to output to.
    //! \param hooks contains database specific SQL generation logic used to vary SQL generation
    //!              without knowing what the specific database is.
    //! \param xpr the cryo expression to process.
    template <typename Expr>
    inline void output_boolean_aggregates(std::stringstream & ss, extension::abstract_hooks const& hooks, Expr const& xpr)
    {
        int dummy = 0;
        output_boolean_aggregates_impl()(xpr, dummy, output_sql_data(ss, hooks));
    }
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_OUTPUT_BOOLEAN_AGGREGATES_HPP )
