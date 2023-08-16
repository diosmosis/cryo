///////////////////////////////////////////////////////////////////////////////
//
//! \file execute_query.hpp
//! Contains the \ref execute_query function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_RELATIONAL_EXECUTE_QUERY_HPP )
#define CRYO_RELATIONAL_EXECUTE_QUERY_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/relational/result_set.hpp>
#include <string>

namespace cryo
{
    //! \brief Executes the supplied SQL and returns a \ref result_set<> that interprets the
    //!        data as a \c Result type.
    //! \tparam Result a containing structure for the data returned by the query. This can be
    //!                a Boost.Fusion sequence or a cryo entity type.
    //! \param conn the database connection. Models \ref RelationalDatabaseConnection.
    //! \param sql the SQL to execute. The data this query returns must be able to be read
    //!            into a \c Result instance.
    //! \returns a \ref result_set<> that iterates over the rows returned by \c sql.
    template <typename Result, typename Connection>
    inline result_set<Result, typename Connection::cursor_type> execute_query(
        Connection const& conn, std::string const& sql)
    {
        typedef result_set<Result, typename Connection::cursor_type> result_type;
        return result_type(conn.execute_query(sql));
    }
}

#endif // #if !defined( CRYO_RELATIONAL_EXECUTE_QUERY_HPP )
