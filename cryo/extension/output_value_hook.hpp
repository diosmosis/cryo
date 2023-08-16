///////////////////////////////////////////////////////////////////////////////
//
//! \file output_value_hook.hpp
//! Contains the \ref output_value_hook<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_OUTPUT_VALUE_HOOK_HPP )
#define CRYO_EXTENSION_OUTPUT_VALUE_HOOK_HPP

#include <cryo/cryo_fwd.hpp>
#include <string>
#include <sstream>

namespace cryo { namespace extension
{
    namespace detail
    {
        inline void output_c_str(std::stringstream & ss, char const* value)
        {
            ss.put('\'');
            for (char const* c = value; *c; ++c)
            {
                switch (*c)
                {
                case '\\':
                    ss.put('\\');

                    ++c;
                    if (*c)
                    {
                        ss.put(*c);
                    }

                    break;
                case '\'':
                    ss.put('\\');
                    ss.put('\'');
                    break;
                default:
                    ss.put(*c);
                    break;
                }
            }
            ss.put('\'');
        }
    }

    //! \brief A hook that allows backends to specify how certain C++ values appear in outputted
    //!        SQL.
    //!
    //! The default implementation of this hook outputs POD values and strings as they would appear
    //! in standard SQL. Strings are escaped before they are outputted.
    //!
    //! Specializations must provide the following operator:
    //! <tt>void operator()(std::stringstream & ss, T const& value) const</tt>
    //! This operator must output \c value as it should appear in an SQL query.
    //!
    //! \remark By default, this hook treats <tt>char const*</tt> values as null-terminated strings.
    //!
    //! \tparam Connection the backend connection type. Models \ref RelationalDatabaseConnection.
    //! \tparam T the value type being outputted.
    template <typename Connection, typename T>
    struct output_value_hook
    {
        void operator()(std::stringstream & ss, T const& value) const
        {
            ss << value;
        }
    };

    template <typename Connection>
    struct output_value_hook<Connection, char const*>
    {
        void operator()(std::stringstream & ss, char const* value) const
        {
            detail::output_c_str(ss, value);
        }
    };

    template <typename Connection, int N>
    struct output_value_hook<Connection, char[N]>
        : output_value_hook<Connection, char const*>
    {};

    template <typename Connection, int N>
    struct output_value_hook<Connection, char const[N]>
        : output_value_hook<Connection, char const*>
    {};

    template <typename Connection>
    struct output_value_hook<Connection, std::string>
    {
        void operator()(std::stringstream & ss, std::string const& value) const
        {
            detail::output_c_str(ss, value.c_str());
        }
    };
}}

#endif // #if !defined( CRYO_EXTENSION_OUTPUT_VALUE_HOOK_HPP )
