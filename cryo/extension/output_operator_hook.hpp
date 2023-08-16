///////////////////////////////////////////////////////////////////////////////
//
//! \file output_operator_hook.hpp
//! Contains the \ref output_operator_hook<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_OUTPUT_OPERATOR_HOOK_HPP )
#define CRYO_EXTENSION_OUTPUT_OPERATOR_HOOK_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/proto/tags.hpp>
#include <sstream>

#define CRYO_BASIC_OPERATOR_HOOK(tag_type, literal)                                                                   \
    template <typename Connection>                                                                                    \
    struct output_operator_hook<Connection, boost::proto::tag::tag_type>                                              \
    {                                                                                                                 \
        void operator()(std::stringstream & ss) const                                                                 \
        {                                                                                                             \
            ss << literal;                                                                                            \
        }                                                                                                             \
    }

namespace cryo { namespace extension
{
    //! \brief Allows backends to determine how C++ operators appear in SQL output.
    //!
    //! This hook has already been specialized for all standard SQL operators, so backends
    //! should not need to specialize it unless the backend's specific brand of SQL differs
    //! from the norm.
    //!
    //! Specializations of this type must supply the following operator:
    //! <tt>void operator()(std::stringstream & ss) const</tt>
    //! This operator outputs the SQL equivalent of the C++ operator signified by \c Tag.
    //!
    //! \tparam Connection the backend connection type.
    //! \tparam Tag a Boost.Proto expression tag that represents a C++ operator, such as
    //!             \c boost::proto::tag::less or \c boost::proto::tag::plus.
    template <typename Connection, typename Tag>
    struct output_operator_hook;

    // binary
    CRYO_BASIC_OPERATOR_HOOK(less, '<');
    CRYO_BASIC_OPERATOR_HOOK(greater, '>');
    CRYO_BASIC_OPERATOR_HOOK(less_equal, "<=");
    CRYO_BASIC_OPERATOR_HOOK(greater_equal, ">=");
    CRYO_BASIC_OPERATOR_HOOK(equal_to, '=');
    CRYO_BASIC_OPERATOR_HOOK(not_equal_to, "<>");
    CRYO_BASIC_OPERATOR_HOOK(logical_or, "OR");
    CRYO_BASIC_OPERATOR_HOOK(logical_and, "AND");
    CRYO_BASIC_OPERATOR_HOOK(plus, '+');
    CRYO_BASIC_OPERATOR_HOOK(minus, '-');
    CRYO_BASIC_OPERATOR_HOOK(multiplies, '*');
    CRYO_BASIC_OPERATOR_HOOK(divides, '/');
    CRYO_BASIC_OPERATOR_HOOK(modulus, '%');
    CRYO_BASIC_OPERATOR_HOOK(bitwise_and, '&');
    CRYO_BASIC_OPERATOR_HOOK(bitwise_or, '|');
    CRYO_BASIC_OPERATOR_HOOK(bitwise_xor, '^');
    CRYO_BASIC_OPERATOR_HOOK(shift_left, "<<");
    CRYO_BASIC_OPERATOR_HOOK(shift_right, ">>");

    // unary
    CRYO_BASIC_OPERATOR_HOOK(logical_not, "NOT");
    CRYO_BASIC_OPERATOR_HOOK(unary_plus, '+');
    CRYO_BASIC_OPERATOR_HOOK(negate, '-');
    CRYO_BASIC_OPERATOR_HOOK(complement, '~');
}}

#undef CRYO_BASIC_OPERATOR_HOOK

#endif // #if !defined( CRYO_EXTENSION_OUTPUT_OPERATOR_HOOK_HPP )
