///////////////////////////////////////////////////////////////////////////////
//
//! \file output_table_name.hpp
//! Contains the \ref output_table_name free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_OUTPUT_TABLE_NAME_HPP )
#define CRYO_SQL_OUTPUT_TABLE_NAME_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension/output_table_name_hook.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    //! \brief Outputs a table name to a \c std::stringstream as it should appear in
    //!        SQL for a certain database type.
    //! \remark This function invokes the \ref output_table_name_hook<> hook.
    //! \remark All occurances of \c '::' or \c ':' will be replaced with underscores.
    //! \tparam Connection a \ref RelationalDatabaseConnection specifying the database
    //!                    that must recognize the outputted table name.
    //! \param ss the stream to output to.
    //! \param name the table name as a null terminated string. Cannot be null. Must have
    //!             the following format: <tt>[A-za-z0-9_:]+</tt>.
    template <typename Connection>
    inline void output_table_name(std::stringstream & ss, char const* name)
    {
        extension::output_table_name_hook<Connection>()(ss, name);
    }
}}

#endif // #if !defined( CRYO_SQL_OUTPUT_TABLE_NAME_HPP )
