///////////////////////////////////////////////////////////////////////////////
//
//! \file output_function_args.hpp
//! Contains the \ref output_function_args free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_OUTPUT_FUNCTION_ARGS_HPP )
#define CRYO_SQL_DYNAMIC_OUTPUT_FUNCTION_ARGS_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/enumerate.hpp>
#include <boost/proto/proto.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    struct output_function_arg_impl
    {
        output_function_arg_impl(extension::abstract_hooks const& hooks_)
            : hooks(hooks_)
        {}

        template <typename Expr>
        void operator()(std::stringstream & ss, Expr const& arg_xpr) const
        {
            output_sql(ss, hooks, arg_xpr);
        }

        extension::abstract_hooks const& hooks;
    };

    //! \brief Outputs the arguments of a function call used in a cryo expression as a comma
    //!        delimited list.
    //! 
    //! This function outputs SQL for each child of the given proto expression, except the first.
    //!
    //! \param ss the \c std::stringstream to output to.
    //! \param hooks contains database specific SQL generation logic used to vary SQL generation
    //!              without knowing what the specific database is.
    //! \param xpr the proto function call expression to output the arguments of.
    template <typename Expr>
    inline void output_function_args(std::stringstream & ss, extension::abstract_hooks const& hooks, Expr const& xpr)
    {
        sql::enumerate(boost::fusion::pop_front(xpr), output_function_arg_impl(hooks), ss, ',');
    }
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_OUTPUT_FUNCTION_ARGS_HPP )
