///////////////////////////////////////////////////////////////////////////////
//
//! \file concrete_hooks.hpp
//! Contains the \c concrete_hooks<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_CONCRETE_HOOKS_HPP )
#define CRYO_EXTENSION_CONCRETE_HOOKS_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension/abstract_hooks.hpp>
#include <cryo/extension/output_is_null_hook.hpp>
#include <cryo/extension/output_operator_hook.hpp>
#include <cryo/extension/output_table_name_hook.hpp>
#include <cryo/extension/output_value_hook.hpp>
#include <boost/proto/tags.hpp>
#include <boost/cstdint.hpp>
#include <sstream>

namespace cryo { namespace extension
{
    // TODO: This type shouldn't be documented...
    //! \brief A concrete implementation of \ref abstract_hooks.
    //! \tparam Connection the backend connection type. Models \ref RelationalDatabaseConnection.
    template <typename Connection>
    struct concrete_hooks
        : abstract_hooks
    {
        // output_value
        void output_value(std::stringstream & ss, boost::int32_t value) const
        {
            output_value_hook<Connection, boost::int32_t>()(ss, value);
        }

        void output_value(std::stringstream & ss, boost::uint32_t value) const
        {
            output_value_hook<Connection, boost::uint32_t>()(ss, value);
        }

        void output_value(std::stringstream & ss, boost::int64_t value) const
        {
            output_value_hook<Connection, boost::int64_t>()(ss, value);
        }

        void output_value(std::stringstream & ss, boost::uint64_t value) const
        {
            output_value_hook<Connection, boost::uint64_t>()(ss, value);
        }

        void output_value(std::stringstream & ss, float value) const
        {
            output_value_hook<Connection, float>()(ss, value);
        }

        void output_value(std::stringstream & ss, double value) const
        {
            output_value_hook<Connection, double>()(ss, value);
        }

        void output_value(std::stringstream & ss, char const* value) const
        {
            output_value_hook<Connection, char const*>()(ss, value);
        }

        void output_value(std::stringstream & ss, std::string const& value) const
        {
            output_value_hook<Connection, std::string>()(ss, value);
        }

        // output_operator
        void output_operator(std::stringstream & ss, boost::proto::tag::less) const
        {
            output_operator_hook<Connection, boost::proto::tag::less>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::greater) const
        {
            output_operator_hook<Connection, boost::proto::tag::greater>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::less_equal) const
        {
            output_operator_hook<Connection, boost::proto::tag::less_equal>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::greater_equal) const
        {
            output_operator_hook<Connection, boost::proto::tag::greater_equal>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::equal_to) const
        {
            output_operator_hook<Connection, boost::proto::tag::equal_to>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::not_equal_to) const
        {
            output_operator_hook<Connection, boost::proto::tag::not_equal_to>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::logical_or) const
        {
            output_operator_hook<Connection, boost::proto::tag::logical_or>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::logical_and) const
        {
            output_operator_hook<Connection, boost::proto::tag::logical_and>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::plus) const
        {
            output_operator_hook<Connection, boost::proto::tag::plus>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::minus) const
        {
            output_operator_hook<Connection, boost::proto::tag::minus>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::multiplies) const
        {
            output_operator_hook<Connection, boost::proto::tag::multiplies>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::divides) const
        {
            output_operator_hook<Connection, boost::proto::tag::divides>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::modulus) const
        {
            output_operator_hook<Connection, boost::proto::tag::modulus>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::bitwise_and) const
        {
            output_operator_hook<Connection, boost::proto::tag::bitwise_and>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::bitwise_or) const
        {
            output_operator_hook<Connection, boost::proto::tag::bitwise_or>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::bitwise_xor) const
        {
            output_operator_hook<Connection, boost::proto::tag::bitwise_xor>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::logical_not) const
        {
            output_operator_hook<Connection, boost::proto::tag::logical_not>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::unary_plus) const
        {
            output_operator_hook<Connection, boost::proto::tag::unary_plus>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::negate) const
        {
            output_operator_hook<Connection, boost::proto::tag::negate>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::complement) const
        {
            output_operator_hook<Connection, boost::proto::tag::complement>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::shift_left) const
        {
            output_operator_hook<Connection, boost::proto::tag::shift_left>()(ss);
        }

        void output_operator(std::stringstream & ss, boost::proto::tag::shift_right) const
        {
            output_operator_hook<Connection, boost::proto::tag::shift_right>()(ss);
        }

        // output_is_null
        void output_is_null(std::stringstream & ss, boost::proto::tag::equal_to) const
        {
            output_is_null_hook<Connection, boost::proto::tag::equal_to>()(ss);
        }

        void output_is_null(std::stringstream & ss, boost::proto::tag::not_equal_to) const
        {
            output_is_null_hook<Connection, boost::proto::tag::not_equal_to>()(ss);
        }

        // output_table_name
        void output_table_name(std::stringstream & ss, char const* value) const
        {
            output_table_name_hook<Connection>()(ss, value);
        }
    };
}}

#endif // #if !defined( CRYO_EXTENSION_CONCRETE_HOOKS_HPP )
