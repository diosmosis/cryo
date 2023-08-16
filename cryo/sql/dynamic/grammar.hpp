///////////////////////////////////////////////////////////////////////////////
//
//! \file grammar.hpp
//! Contains a Boost.Proto grammar that describes cryo's query DSL.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_GRAMMAR_HPP )
#define CRYO_SQL_DYNAMIC_GRAMMAR_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/null.hpp>
#include <cryo/sql/dynamic/col.hpp>
#include <cryo/sql/dynamic/is_function.hpp>
#include <cryo/sql/dynamic/is_value.hpp>
#include <boost/proto/proto.hpp>

#define CRYO_SQL_DYNAMIC_UNARY_CASE(op)                                                                               \
    template <> struct expr_cases::case_<proto::tag::op> : proto::op<expr> {}
#define CRYO_SQL_DYNAMIC_BINARY_CASE(op)                                                                              \
    template <> struct expr_cases::case_<proto::tag::op> : proto::op<expr, expr> {}
#define CRYO_SQL_DYNAMIC_BINARY_COND_CASE(op)                                                                         \
    template <>                                                                                                       \
    struct expr_cases::case_<proto::tag::op>                                                                          \
        : proto::op<                                                                                                  \
            proto::or_<null_terminal, expr>,                                                                          \
            proto::or_<null_terminal, expr>                                                                           \
        >                                                                                                             \
    {};

namespace cryo { namespace sql { namespace dynamic
{
    namespace proto = boost::proto;

    namespace detail
    {
        // is_column
        template <typename T>
        struct is_column
            : boost::mpl::false_
        {};

        template <typename T>
        struct is_column<T const>
            : is_column<T>
        {};

        template <typename Cols>
        struct is_column<keyword::column_path<Cols> >
            : boost::mpl::true_
        {};
    }

    namespace grammar
    {
        using proto::_;

        //! \brief A proto grammar that matches an SQL expression.
        //!
        //! This grammar will allow any type of function call, including aggregate functions.
        struct expr;

        //! \brief A proto grammar that matches condition expressions that appear in \c WHERE
        //!        clauses.
        //!
        //! This grammar will not allow aggregate function calls, only boolean aggregates.
        struct where_clause;

        struct column
            : proto::and_<
                proto::terminal<_>,
                proto::if_<detail::is_column<proto::_value>()>
            >
        {};

        struct value
            : proto::and_<
                proto::terminal<_>,
                proto::if_<is_value<proto::_value>()>
            >
        {};

        struct abstract_expr
            : proto::and_<
                proto::terminal<_>,
                proto::if_<boost::is_base_of<expr_base, proto::_value>()>
            >
        {};

        struct abstract_condition
            : proto::and_<
                proto::terminal<_>,
                proto::if_<boost::is_same<condition, proto::_value>()>
            >
        {};

        struct null_terminal
            : proto::terminal<keyword::null>
        {};

        template <typename Predicate>
        struct function_call
          : proto::function<
              proto::and_<
                proto::terminal<_>,
                proto::if_<Predicate>
              >,
              proto::vararg<expr>
          >
        {};

        //! 
        typedef function_call<is_function<proto::_value>()> nonaggregate_function_call;
        typedef function_call<is_aggregate_function<proto::_value>()> aggregate_function_call;
        typedef function_call<is_boolean_aggregate<proto::_value>()> boolean_aggregate_call;

        typedef function_call<
            boost::mpl::or_<
                is_function<proto::_value>,
                is_boolean_aggregate<proto::_value>
            >()
        > query_function_call;

        typedef function_call<boost::mpl::true_()> any_function_call;

        struct expr_cases
        {
            template <typename Tag>
            struct case_;
        };

        CRYO_SQL_DYNAMIC_BINARY_CASE(shift_left);
        CRYO_SQL_DYNAMIC_BINARY_CASE(shift_right);
        CRYO_SQL_DYNAMIC_BINARY_CASE(multiplies);
        CRYO_SQL_DYNAMIC_BINARY_CASE(divides);
        CRYO_SQL_DYNAMIC_BINARY_CASE(modulus);
        CRYO_SQL_DYNAMIC_BINARY_CASE(plus);
        CRYO_SQL_DYNAMIC_BINARY_CASE(minus);
        CRYO_SQL_DYNAMIC_BINARY_CASE(less);
        CRYO_SQL_DYNAMIC_BINARY_CASE(greater);
        CRYO_SQL_DYNAMIC_BINARY_CASE(less_equal);
        CRYO_SQL_DYNAMIC_BINARY_CASE(greater_equal);
        CRYO_SQL_DYNAMIC_BINARY_CASE(logical_or);
        CRYO_SQL_DYNAMIC_BINARY_CASE(logical_and);
        CRYO_SQL_DYNAMIC_BINARY_CASE(bitwise_and);
        CRYO_SQL_DYNAMIC_BINARY_CASE(bitwise_or);
        CRYO_SQL_DYNAMIC_BINARY_CASE(bitwise_xor);

        CRYO_SQL_DYNAMIC_BINARY_COND_CASE(equal_to);
        CRYO_SQL_DYNAMIC_BINARY_COND_CASE(not_equal_to);

        CRYO_SQL_DYNAMIC_UNARY_CASE(unary_plus);
        CRYO_SQL_DYNAMIC_UNARY_CASE(negate);
        CRYO_SQL_DYNAMIC_UNARY_CASE(complement);
        CRYO_SQL_DYNAMIC_UNARY_CASE(logical_not);

        template <>
        struct expr_cases::case_<proto::tag::function>
          : any_function_call
        {};

        template <>
        struct expr_cases::case_<proto::tag::terminal>
          : proto::or_<
              column,
              value,
              abstract_expr
          >
        {};

        struct expr
          : proto::switch_<expr_cases>
        {};

        struct conditional_cases
        {
            template <typename Tag>
            struct case_;
        };

        template <>
        struct conditional_cases::case_<proto::tag::equal_to>
          : proto::equal_to<
              proto::or_<expr, null_terminal>,
              proto::or_<expr, null_terminal>
          >
        {};

        template <>
        struct conditional_cases::case_<proto::tag::not_equal_to>
          : proto::not_equal_to<
              proto::or_<expr, null_terminal>,
              proto::or_<expr, null_terminal>
          >
        {};

        template <>
        struct conditional_cases::case_<proto::tag::less>
          : proto::less<expr, expr>
        {};

        template <>
        struct conditional_cases::case_<proto::tag::greater>
          : proto::greater<expr, expr>
        {};

        template <>
        struct conditional_cases::case_<proto::tag::less_equal>
          : proto::less_equal<expr, expr>
        {};

        template <>
        struct conditional_cases::case_<proto::tag::greater_equal>
          : proto::greater_equal<expr, expr>
        {};

        struct conditional
          : proto::switch_<conditional_cases>
        {};

        template <typename Tag>
        struct where_clause_cases_impl
            : conditional_cases::case_<Tag>
        {};

        struct where_clause_cases
        {
            template <typename Tag>
            struct case_
              : where_clause_cases_impl<Tag>
            {};
        };

        template <>
        struct where_clause_cases_impl<proto::tag::logical_and>
          : proto::logical_and<
              proto::or_<
                  where_clause,
                  aggregate_function_call
              >,
              proto::or_<
                  where_clause,
                  aggregate_function_call
              >
          >
        {};

        template <>
        struct where_clause_cases_impl<proto::tag::logical_or>
          : proto::logical_or<where_clause, where_clause>
        {};

        template <>
        struct where_clause_cases_impl<proto::tag::logical_not>
          : proto::logical_not<where_clause>
        {};

        template <>
        struct where_clause_cases_impl<proto::tag::function>
          : query_function_call
        {};

        template <>
        struct where_clause_cases_impl<proto::tag::terminal>
          : abstract_condition
        {};

        struct where_clause
          : proto::switch_<where_clause_cases>
        {};

        // extra rules
        struct binary
            : proto::binary_expr<_, _, _>
        {};

        struct unary
            : proto::unary_expr<_, _>
        {};
    }
}}}

#undef CRYO_SQL_DYNAMIC_UNARY_CASE
#undef CRYO_SQL_DYNAMIC_BINARY_CASE

#endif // #if !defined( CRYO_SQL_DYNAMIC_GRAMMAR_HPP )
