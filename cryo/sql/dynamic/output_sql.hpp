///////////////////////////////////////////////////////////////////////////////
//
//! \file output_sql.hpp
//! Contains the \ref output_sql free function and the proto transforms it uses.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_OUTPUT_SQL_HPP )
#define CRYO_SQL_DYNAMIC_OUTPUT_SQL_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/arg.hpp>
#include <cryo/sql/dynamic/grammar.hpp>
#include <cryo/sql/dynamic/condition.hpp>
#include <cryo/sql/dynamic/output_function_args.hpp>
#include <cryo/schema/introspect/column_name_of.hpp>
#include <boost/proto/proto.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/size.hpp>
#include <sstream>
// TODO: Need to go through all files and make sure no line is past a certain length
namespace cryo { namespace sql { namespace dynamic
{
    // TODO: Need to unify all SQL generation mechanisms that use proto. One or two concepts
    // oughtta do it.
    //! \brief A Boost.Proto callable transform that outputs a value to a
    //!        \c std::stringstream.
    struct output_value_xpr : proto::callable
    {
        typedef void result_type;

        template <typename T>
        void operator()(std::stringstream & ss, extension::abstract_hooks const& hooks, T const& value) const
        {
            hooks.output_value(ss, value);
        }
    };

    //! \brief A Boost.Proto callable transform that outputs a column
    //!        reference to a \c std::stringstream.
    struct output_column_reference : proto::callable
    {
        typedef void result_type;

        template <typename Cols>
        void operator()(
            std::stringstream & ss, extension::abstract_hooks const& hooks, keyword::column_path<Cols> const& kwd) const
        {
            // TODO: Why do I need to check the size here? join_idx_of works when not accessing from the root entity...
            typedef typename boost::mpl::size<Cols>::type sz;
            ss << 't' << (sz::value == 1 ? 0 : detail::join_idx_of(kwd)) << '.' << column_name_of(kwd.as_ptr());
        }
    };

    //! \brief A Boost.Proto callable transform that outputs an SQL function
    //!        call to a \c std::stringstream.
    struct output_function_call : proto::callable
    {
        typedef void result_type;

        template <typename Expr>
        void operator()(std::stringstream & ss, extension::abstract_hooks const& hooks, Expr const& xpr) const
        {
            typedef typename proto::result_of::value<
                typename proto::result_of::child_c<Expr, 0>::type
            >::type function_type;

            function_type::apply(ss);
            ss << '(';
            output_function_args(ss, hooks, xpr);
            ss << ')';
        }
    };

    //! \brief A Boost.Proto callable transform that outputs a binary operation
    //!        to a \c std::stringstream.
    struct output_binary_operation : proto::callable
    {
        typedef void result_type;

        template <typename Expr>
        void operator()(std::stringstream & ss, extension::abstract_hooks const& hooks, Expr const& xpr) const
        {
            ss << '(';
            output_sql(ss, hooks, proto::left(xpr));
            hooks.output_operator(ss, typename proto::tag_of<Expr>::type());
            output_sql(ss, hooks, proto::right(xpr));
            ss << ')';
        }

    };

    //! \brief A Boost.Proto callable transform that outputs an <tt>IS NULL</tt> or
    //!        <tt>IS NOT NULL</tt> expression to a \c std::stringstream.
    struct output_is_null : proto::callable
    {
        typedef void result_type;

        template <typename E, typename C>
        void operator()(std::stringstream & ss, extension::abstract_hooks const& hooks, E const& xpr, C const& child) const
        {
            ss << '(';
            output_sql(ss, hooks, child);
            ss << ' ';
            hooks.output_is_null(ss, typename proto::tag_of<E>::type());
            ss << ')';
        }
    };

    //! \brief A Boost.Proto callable transform that outputs a unary operation to
    //!        a \c std::stringstream.
    struct output_unary_operation : proto::callable
    {
        typedef void result_type;

        template <typename Expr>
        void operator()(std::stringstream & ss, extension::abstract_hooks const& hooks, Expr const& xpr) const
        {
            hooks.output_operator(ss, typename proto::tag_of<Expr>::type());
            ss << '(';
            output_sql(ss, hooks, proto::child_c<0>(xpr));
            ss << ')';
        }
    };

    //! \brief A Boost.Proto callable transform that outputs SQL for a stored
    //!        cryo expression.
    //!
    //! This transform invokes \ref output_sql on the stored expression.
    struct output_sql_indirect : proto::callable
    {
        typedef void result_type;

        void operator()(std::stringstream & ss, extension::abstract_hooks const& hooks, expr_base const& xpr) const
        {
            xpr.output_sql(ss, hooks);
        }
    };

    struct output_sql_impl;

    struct output_binary_impl
        : proto::or_<
            proto::when< // when lhs is aggregate && rhs is not, ignore lhs and evaluate rhs
                proto::logical_and<
                    grammar::boolean_aggregate_call,
                    proto::not_<grammar::boolean_aggregate_call>
                >,
                output_sql_impl(proto::_right, proto::_state, proto::_data)
            >,
            proto::when< // when rhs is aggregate && lhs is not, ignore rhs and evaluate lhs
                proto::logical_and<
                    proto::not_<grammar::boolean_aggregate_call>,
                    grammar::boolean_aggregate_call
                >,
                output_sql_impl(proto::_left, proto::_state, proto::_data)
            >,
            proto::when< // when both are aggregates, do nothing
                proto::logical_and<
                    grammar::boolean_aggregate_call,
                    grammar::boolean_aggregate_call
                >
            >,
            proto::when< // when binary w/ a null on left, invoke output_is_null w/ rhs
                proto::binary_expr<
                    proto::_,
                    grammar::null_terminal,
                    proto::_
                >,
                output_is_null(_stream, _hooks, proto::_expr, proto::_right)
            >,
            proto::when< // when binary w/ a null on right, invoke output_is_null w/ lhs
                proto::binary_expr<
                    proto::_,
                    proto::_,
                    grammar::null_terminal
                >,
                output_is_null(_stream, _hooks, proto::_expr, proto::_left)
            >,
            proto::when< // any other binary, invoke output_binary_op
                proto::_,
                output_binary_operation(_stream, _hooks, proto::_expr)
            >
        >
    {};

    struct output_sql_impl
        : proto::or_<
            proto::when<
                grammar::value,
                output_value_xpr(_stream, _hooks, proto::_value)
            >,
            proto::when<
                grammar::column,
                output_column_reference(_stream, _hooks, proto::_value)
            >,
            proto::when<
                grammar::abstract_expr,
                output_sql_indirect(_stream, _hooks, proto::_value)
            >,
            proto::when<
                grammar::any_function_call,
                output_function_call(_stream, _hooks, proto::_expr)
            >,
            proto::when<
                grammar::binary,
                output_binary_impl(proto::_expr, proto::_state, proto::_data)
            >,
            proto::when<
                grammar::unary,
                output_unary_operation(_stream, _hooks, proto::_expr)
            >
        >
    {};

    //! \brief Outputs the SQL equivalent of a cryo expression to a \c std::stringstream.
    //! \param ss the \c std::stringstream to output to.
    //! \param hooks contains database specific SQL generation logic used to vary SQL generation
    //!              without knowing what the specific database is.
    //! \param xpr the cryo expression to process.
    template <typename Expr, typename Hooks>
    inline void output_sql(std::stringstream & ss, Hooks const& hooks, Expr const& xpr)
    {
        int dummy = 0;
        output_sql_impl()(xpr, dummy, output_sql_data(ss, hooks));
    }
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_OUTPUT_SQL_HPP )
