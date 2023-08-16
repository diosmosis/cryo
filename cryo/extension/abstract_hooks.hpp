///////////////////////////////////////////////////////////////////////////////
//
//! \file abstract_hooks.hpp
//! Contains the \c abstract_hooks type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_ABSTRACT_HOOKS_HPP )
#define CRYO_EXTENSION_ABSTRACT_HOOKS_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <boost/cstdint.hpp>
#include <sstream>

namespace cryo { namespace extension
{
    // TODO: This shouldn't be documented... it's a detail type that just happens to be used in certain free functions.
    //! \brief Abstract type used by cryo's dynamic query EDSL so certain hooks can
    //!        be invoked without access to a connection type.
    //!
    //! \ref abstract_hooks defines several pure virtual functions which map directly
    //! to the \ref output_value_hook<>, \ref output_operator_hook<>, \ref output_is_null_hook<>
    //! and \ref output_table_name_hook<> hooks. \ref concrete_hooks<>, the only
    //! implementation of this type, implements these virtual functions using a connection
    //! type that is defined as a template parameter. This way query expressions can be
    //! stored and used with arbitrary connection types.
    struct abstract_hooks
    {
        virtual ~abstract_hooks() {}

        // output_value
        virtual void output_value(std::stringstream & ss, boost::int32_t value) const = 0;
        virtual void output_value(std::stringstream & ss, boost::uint32_t value) const = 0;

        virtual void output_value(std::stringstream & ss, boost::int64_t value) const = 0;
        virtual void output_value(std::stringstream & ss, boost::uint64_t value) const = 0;

        virtual void output_value(std::stringstream & ss, float value) const = 0;
        virtual void output_value(std::stringstream & ss, double value) const = 0;
        virtual void output_value(std::stringstream & ss, char const* value) const = 0;
        virtual void output_value(std::stringstream & ss, std::string const& value) const = 0;

        // output_operator
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::less) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::greater) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::less_equal) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::greater_equal) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::equal_to) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::not_equal_to) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::logical_or) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::logical_and) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::plus) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::minus) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::multiplies) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::divides) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::modulus) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::bitwise_and) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::bitwise_or) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::bitwise_xor) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::logical_not) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::unary_plus) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::negate) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::complement) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::shift_left) const = 0;
        virtual void output_operator(std::stringstream & ss, boost::proto::tag::shift_right) const = 0;

        // output_is_null
        virtual void output_is_null(std::stringstream & ss, boost::proto::tag::equal_to) const = 0;
        virtual void output_is_null(std::stringstream & ss, boost::proto::tag::not_equal_to) const = 0;

        // output_table_name
        virtual void output_table_name(std::stringstream & ss, char const* value) const = 0;
    };
}}

#endif // #if !defined( CRYO_EXTENSION_ABSTRACT_HOOKS_HPP )
