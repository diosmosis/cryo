///////////////////////////////////////////////////////////////////////////////
//
//! \file output_is_null_hook.hpp
//! Contains the \c output_is_null_hook<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_OUTPUT_IS_NULL_HOOK_HPP )
#define CRYO_EXTENSION_OUTPUT_IS_NULL_HOOK_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <sstream>

namespace cryo { namespace extension
{
    //! \brief Allows backends to specify how <tt>IS NULL</tt> and <tt>IS NOT NULL</tt>
    //!        expressions appear in SQL strings.
    //!
    //! The default implementations of this hook output <tt>IS NULL</tt> and <tt>IS NOT NULL</tt>
    //! when appropriate. Backends will most likely not need to specialize this hook.
    //!
    //! Specializations must define the following operator:
    //! <tt>void operator()(std::stringstream & ss) const</tt>
    //! This operator must output the appropriate SQL based on whether an <tt>IS NULL</tt>
    //! is being requested or an <tt>IS NOT NULL</tt> is being requested.
    //!
    //! \tparam Connection the backend connection type.
    //! \tparam Tag a Boost.Proto expression tag. This will either be
    //!             \c boost::proto::tag::equal_to signifying an <tt>IS NULL</tt> should be
    //!             outputted or \c boost::proto::tag::not_equal_to signifying an
    //!             <tt>IS NOT NULL</tt> should be outputted.
    template <typename Connection, typename Tag>
    struct output_is_null_hook;

    template <typename Connection>
    struct output_is_null_hook<Connection, boost::proto::tag::equal_to>
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "IS NULL";
        }
    };

    template <typename Connection>
    struct output_is_null_hook<Connection, boost::proto::tag::not_equal_to>
    {
        void operator()(std::stringstream & ss) const
        {
            ss << "IS NOT NULL";
        }
    };
}}

#endif // #if !defined( CRYO_EXTENSION_OUTPUT_IS_NULL_HOOK_HPP )
