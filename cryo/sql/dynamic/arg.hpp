///////////////////////////////////////////////////////////////////////////////
//
//! \file arg.hpp
//! Contains two utility types used to ease the definition of proto transforms.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_ARG_HPP )
#define CRYO_SQL_DYNAMIC_ARG_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/proto/proto.hpp>

namespace cryo { namespace sql { namespace dynamic
{
    struct output_sql_data
    {
        output_sql_data(std::stringstream & ss_, extension::abstract_hooks const& hooks_)
            : ss(ss_)
            , hooks(hooks_)
        {}

        std::stringstream & ss;
        extension::abstract_hooks const& hooks;
    };

    struct _stream : boost::proto::transform<_stream>
    {
        template <typename Expr, typename State, typename Data>
        struct impl : boost::proto::transform_impl<Expr, State, Data>
        {
            typedef std::stringstream & result_type;

            std::stringstream & operator()(
                typename impl::expr_param
              , typename impl::state_param
              , typename impl::data_param d
            ) const
            {
                return d.ss;
            }
        };
    };

    struct _hooks : boost::proto::transform<_hooks>
    {
        template <typename Expr, typename State, typename Data>
        struct impl : boost::proto::transform_impl<Expr, State, Data>
        {
            typedef extension::abstract_hooks const& result_type;

            extension::abstract_hooks const& operator()(
                typename impl::expr_param
              , typename impl::state_param
              , typename impl::data_param d
            ) const
            {
                return d.hooks;
            }
        };
    };
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_ARG_HPP )
