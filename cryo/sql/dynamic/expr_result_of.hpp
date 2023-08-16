///////////////////////////////////////////////////////////////////////////////
//
//! \file expr_result_of.hpp
//! Contains the \ref expr_result_of<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_EXPR_RESULT_OF_HPP )
#define CRYO_SQL_DYNAMIC_EXPR_RESULT_OF_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/tag/nullable.hpp>
#include <cryo/sql/dynamic/grammar.hpp>
#include <cryo/sql/dynamic/function_result_of.hpp>
#include <cryo/detail/unwrap.hpp>
#include <boost/optional.hpp>
#include <boost/utility/result_of.hpp>
#include <string>

#define CRYO_EXPR_RESULT_OF_ARITH(arithtag)                                                                           \
        template <typename C, typename E, typename T0, typename T1>                                                   \
        struct binary_result_of_impl<C, E, T0, T1, proto::tag::arithtag>                                              \
            : result_of_arithmetic<C, T0, T1>                                                                         \
        {}

namespace cryo { namespace sql { namespace dynamic
{
    namespace detail
    {
        template <typename T>
        struct expr_value_of
        {
            typedef T type;
        };

        template <typename T>
        struct expr_value_of<T &>
            : expr_value_of<T>
        {};

        template <typename T>
        struct expr_value_of<T const>
            : expr_value_of<T>
        {};

        template <size_t N>
        struct expr_value_of<char[N]>
        {
            typedef std::string type;
        };

        template <size_t N>
        struct expr_value_of<char const[N]>
        {
            typedef std::string type;
        };

        template <size_t N>
        struct expr_value_of<char(&)[N]>
        {
            typedef std::string type;
        };

        template <size_t N>
        struct expr_value_of<char const(&)[N]>
        {
            typedef std::string type;
        };

        template <>
        struct expr_value_of<char *>
        {
            typedef std::string type;
        };

        template <>
        struct expr_value_of<char const*>
        {
            typedef std::string type;
        };

        template <typename C, typename T0, typename T1>
        struct result_of_arithmetic
        {
            typedef typename C::int_type type;
        };

        template <typename C, typename E, typename T0, typename T1, typename Tag = typename E::proto_tag>
        struct binary_result_of_impl;

        CRYO_EXPR_RESULT_OF_ARITH(multiplies);
        CRYO_EXPR_RESULT_OF_ARITH(modulus);
        CRYO_EXPR_RESULT_OF_ARITH(plus);
        CRYO_EXPR_RESULT_OF_ARITH(minus);

        template <typename C, typename E, typename T0, typename T1>
        struct binary_result_of_impl<C, E, T0, T1, proto::tag::divides>
        {
            typedef double type;
        };

        template <typename C, typename E, typename T0, typename T1>
        struct binary_result_of_impl<C, E, T0, T1, proto::tag::less>
        {
            typedef bool type;
        };

        template <typename C, typename E, typename T0, typename T1>
        struct binary_result_of_impl<C, E, T0, T1, proto::tag::greater>
        {
            typedef bool type;
        };

        template <typename C, typename E, typename T0, typename T1>
        struct binary_result_of_impl<C, E, T0, T1, proto::tag::less_equal>
        {
            typedef bool type;
        };

        template <typename C, typename E, typename T0, typename T1>
        struct binary_result_of_impl<C, E, T0, T1, proto::tag::greater_equal>
        {
            typedef bool type;
        };

        template <typename C, typename E, typename T0, typename T1>
        struct binary_result_of_impl<C, E, T0, T1, proto::tag::equal_to>
        {
            typedef bool type;
        };

        template <typename C, typename E, typename T0, typename T1>
        struct binary_result_of_impl<C, E, T0, T1, proto::tag::not_equal_to>
        {
            typedef bool type;
        };

        template <typename C, typename E, typename T0, typename T1>
        struct binary_result_of_impl<C, E, T0, T1, proto::tag::logical_or>
        {
            typedef bool type;
        };

        template <typename C, typename E, typename T0, typename T1>
        struct binary_result_of_impl<C, E, T0, T1, proto::tag::logical_and>
        {
            typedef bool type;
        };

        template <typename C, typename E, typename T0, typename T1>
        struct binary_result_of
            : binary_result_of_impl<C, E, T0, T1>
        {};
    }

    template <typename Connection, typename E>
    struct expr_result_of;

    template <typename Connection>
    struct expr_result_of_impl
        : proto::or_<
            proto::when<
                grammar::value,
                detail::expr_value_of<proto::_value>()
            >,
            proto::when<
                grammar::null_terminal,
                keyword::null()
            >,
            proto::when<
                grammar::column,
                expr_result_of<Connection, proto::_value>()
            >,
            proto::when<
                grammar::abstract_expr,
                expr_result_of<Connection, proto::_value>()
            >,
            proto::when<
                grammar::any_function_call,
                function_result_of<Connection, proto::_expr>()
            >,
            proto::when<
                grammar::binary,
                detail::binary_result_of<
                    Connection,
                    proto::_expr,
                    expr_result_of_impl<Connection>(proto::_left),
                    expr_result_of_impl<Connection>(proto::_right)
                >()
            >,
            proto::when<
                grammar::unary,
                expr_result_of_impl<Connection>(proto::_child_c<0>)
            >
        >
    {};

    //! \brief A metafunction that computes the result of a cryo expression when its
    //!        equivalent SQL is executed in a certain database.
    //! \tparam Connection a \ref RelationalDatabaseConnection specifying the database in
    //!                    which generated SQL will be executed.
    //! \tparam E a proto expression that matches \c cryo::sql::dynamic::grammar::expr.
    //! \returns the result of \c E. This is the type that will appear in a \ref result_set<>
    //!          when \c E's generated SQL is executed.
    template <typename Connection, typename E>
    struct expr_result_of
        : boost::result_of<expr_result_of_impl<Connection>(E)>
    {};

    template <typename Connection, typename T>
    struct expr_result_of<Connection, expr<T> >
    {
        typedef T type;
    };

    template <typename Connection>
    struct expr_result_of<Connection, condition>
    {
        typedef bool type;
    };

    template <typename Connection, typename Cols>
    struct expr_result_of<Connection, keyword::column_path<Cols> >
        : boost::mpl::if_<
            is_nullable<keyword::column_path<Cols> >,
            boost::optional<
                typename cryo::result_of::unwrap<
                    typename field_type_of<keyword::column_path<Cols> >::type
                >::type
            >,
            typename cryo::result_of::unwrap<
                typename field_type_of<keyword::column_path<Cols> >::type
            >::type
        >
    {};

    template <typename Connection, typename Cols>
    struct expr_result_of<Connection, keyword::group_by<Cols> >
        : expr_result_of<Connection, keyword::column_path<Cols> >
    {};
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_EXPR_RESULT_OF_HPP )
