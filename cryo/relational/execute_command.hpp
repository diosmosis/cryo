///////////////////////////////////////////////////////////////////////////////
//
//! \file execute_command.hpp
//! Contains the \ref execute_command function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_RELATIONAL_EXECUTE_COMMAND_HPP )
#define CRYO_RELATIONAL_EXECUTE_COMMAND_HPP

#include <cryo/cryo_fwd.hpp>
#include <string>

namespace cryo
{
    //! \brief Executes the supplied SQL query and assumes there is no result.
    //! \param conn the backend connection. Models \ref RelationalDatabaseConnection.
    //! \param sql the SQL to execute.
    //! \throws error if \c conn is inactive or \c sql is somehow invalid.
    template <typename Connection>
    inline void execute_command(Connection const& conn, std::string const& sql)
    {
        conn.execute_query(sql);
    }
}

#endif // #if !defined( CRYO_RELATIONAL_EXECUTE_COMMAND_HPP )
