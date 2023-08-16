///////////////////////////////////////////////////////////////////////////////
//
//! \file expr_base.hpp
//! Contains the \ref expr_base type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_EXPR_BASE_HPP )
#define CRYO_SQL_DYNAMIC_EXPR_BASE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/dynamic/join_index_set.hpp>
#include <cryo/sql/dynamic/output_boolean_aggregates.hpp>
#include <boost/proto/deep_copy.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    namespace detail
    {
        struct abstract_expr
        {
            virtual void output_sql(std::stringstream & ss, extension::abstract_hooks const& hooks) const = 0;
            virtual void output_boolean_aggregates(std::stringstream & ss, extension::abstract_hooks const& hooks) const = 0;
            virtual void calc_join_indices(join_index_set & indices) const = 0;
            virtual abstract_expr * clone() const = 0;
        };

        template <typename Expr>
        struct concrete_expr
            : abstract_expr
        {
            typedef typename boost::proto::result_of::deep_copy<Expr>::type expr_type;

            concrete_expr(Expr const& xpr_)
                : xpr(boost::proto::deep_copy(xpr_))
            {}

            concrete_expr(expr_type const& xpr_)
                : xpr(boost::proto::deep_copy(xpr_))
            {}

            void output_sql(std::stringstream & ss, extension::abstract_hooks const& hooks) const
            {
                cryo::sql::dynamic::output_sql(ss, hooks, xpr);
            }

            void output_boolean_aggregates(std::stringstream & ss, extension::abstract_hooks const& hooks) const
            {
                cryo::sql::dynamic::output_boolean_aggregates(ss, hooks, xpr);
            }

            void calc_join_indices(join_index_set & indices) const
            {
                calc_join_indices_single cjis(indices);
                cjis(xpr);
            }

            abstract_expr * clone() const
            {
                return new concrete_expr<Expr>(xpr);
            }

            expr_type xpr;
        };
    }

    //! \brief Stores any cryo expression.
    //!
    //! \remark This type should not be used, instead either \ref expr<> or \ref condition
    //!         should be used.
    //! \remark \ref expr_base is not thread safe and must be copied before being used by
    //!         more than one thread.
    struct expr_base
    {
        typedef boost::scoped_ptr<detail::abstract_expr> expr_base::* unspecified_bool_t;

        //! \brief Default constructor. Creates an uninitialized instance.
        expr_base()
            : impl()
        {}

        //! \brief Copy constructor. Performs a deep copy.
        //! \param x the \ref expr_base instance to copy.
        expr_base(expr_base const& x)
            : impl(x.impl ? x.impl->clone() : 0)
        {}

        //! \brief Constructor.
        //! \param x a cryo expression to store. A deep copy is performed before storing.
        template <typename Expr>
        expr_base(Expr const& x, typename Expr::proto_tag * d = 0)
            : impl(new detail::concrete_expr<Expr>(x))
        {
            static_assert(proto::matches<Expr, grammar::expr>::value, "Expr is not a valid cryo expression.");
        }

        //! \brief Generates and outputs SQL using the stored cryo expression.
        //! \param ss the \c std::stringstream to output to.
        //! \param hooks contains database specific SQL generation logic. This parameter allows
        //!              cryo expressions to be stored without knowing which database they will be
        //!              used with.
        void output_sql(std::stringstream & ss, extension::abstract_hooks const& hooks) const
        {
            if (impl)
            {
                impl->output_sql(ss, hooks);
            }
        }

        //! \brief Outputs every boolean aggregate used in the stored expression as a comma
        //!        delimited list.
        //! \param ss the \c std::stringstream to output to.
        //! \param hooks contains database specific SQL generation logic. This parameter allows
        //!              cryo expressions to be stored without knowing which database they will be
        //!              used with.
        void output_boolean_aggregates(std::stringstream & ss, extension::abstract_hooks const& hooks) const
        {
            if (impl)
            {
                impl->output_boolean_aggregates(ss, hooks);
            }
        }

        //! \brief Generates SQL using the stored cryo expression.
        //! \param hooks contains database specific SQL generation logic. This parameter allows
        //!              cryo expressions to be stored without knowing which database they will be
        //!              used with.
        //! \returns the generated SQL.
        std::string to_sql(extension::abstract_hooks const& hooks) const
        {
            std::stringstream ss;
            output_sql(ss, hooks);
            return ss.str();
        }

        //! \brief Calculates the join indices of each \ref ColumnChain used by the stored expression.
        //! \param indices the intrusive set of \ref column_nodes used to make sure each join is only
        //!                outputted once, and in the correct order.
        void calc_join_indices(join_index_set & indices) const
        {
            if (impl)
            {
                impl->calc_join_indices(indices);
            }
        }

        //! \brief Tests whether this object stores an expression or not.
        //! \returns a value convertible to \c true if an expression is stored, a value convertible to
        //!          \c false if otherwise.
        operator unspecified_bool_t() const
        {
            return impl ? &expr_base::impl : 0;
        }

        boost::scoped_ptr<detail::abstract_expr> impl;
    };
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_EXPR_BASE_HPP )
