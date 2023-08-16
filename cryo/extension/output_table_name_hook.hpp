///////////////////////////////////////////////////////////////////////////////
//
//! \file output_table_name_hook.hpp
//! Contains the \ref output_table_name_hook<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_OUTPUT_TABLE_NAME_HOOK_HPP )
#define CRYO_EXTENSION_OUTPUT_TABLE_NAME_HOOK_HPP

#include <cryo/cryo_fwd.hpp>
#include <sstream>

namespace cryo { namespace extension
{
    namespace detail
    {
        // NOTE: Will replace ':' or '::' w/ '_'. If more than two colons appear together, however, it
        // will output more than one underscore.
        inline void output_table_name(std::stringstream & ss, char const* name)
        {
            for (char const* c = name; *c; ++c)
            {
                switch (*c)
                {
                case ':':
                    ss.put('_');

                    ++c;
                    if (*c && *c != ':')
                    {
                        ss.put(*c);
                    }
                    break;
                default:
                    ss.put(*c);
                    break;
                }
            }
        }
    }

    //! \brief Allows backends to specify customize the way entity table names are outputted.
    //!
    //! By default, this hook outputs the string its given with all ocurrances of <tt>"::"</tt>
    //! or <tt>":"</tt> replaced with underscores. If more than two colons appear together, they
    //! will be replaced by more than one underscore.
    //!
    //! Specializations of this type must define the following operator:
    //! <tt>void operator()(std::stringstream &, char const* value) const</tt>
    //! This operator outputs an SQL friendly version of \c value.
    //!
    //! \tparam Connection the backend connection type.
    template <typename Connection>
    struct output_table_name_hook
    {
        void operator()(std::stringstream & ss, char const* value) const
        {
            detail::output_table_name(ss, value);
        }
    };
}}

#endif // #if !defined( CRYO_EXTENSION_OUTPUT_TABLE_NAME_HOOK_HPP )
