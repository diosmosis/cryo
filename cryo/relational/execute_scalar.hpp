///////////////////////////////////////////////////////////////////////////////
//
//! \file execute_scalar.hpp
//! Contains the \ref execute_scalar<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_RELATIONAL_EXECUTE_SCALAR_HPP )
#define CRYO_RELATIONAL_EXECUTE_SCALAR_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <string>

namespace cryo
{
    //! \brief Executes the supplied SQL query and assumes the query returned one result
    //!        as an integral, which is returned.
    //! \tparam Result the integral type to return.
    //! \param conn the database connection. Models \ref RelationalDatabaseConnection.
    //! \param sql The SQL query to execute. The result of this query should be an integral
    //!            type.
    //! \returns The result of \c sql.
    //! \throws error if \c conn is inactive or \c sql is an invalid query.
    template <typename Result, typename Connection>
    inline Result execute_scalar(Connection const& conn, std::string const& sql)
    {
        static_assert(boost::is_integral<Result>::value, "execute_scalar can only return an integral type.");

        // the result
        Result result;

        // execute the query
        typename Connection::cursor_type cursor(conn.execute_query(sql));

        // get & return the only result
        cursor.get_column(result);
        return result;
    }

    //! \brief Executes the supplied SQL query and assumes the query returns one result
    //!        as a signed integral.
    //! \param conn the database connection. Models \ref RelationalDatabaseConnection.
    //! \param sql The SQL query to execute. The result of this query should be an integral
    //!            type.
    //! \returns The result of \c sql as the database's native integer type.
    //! \throws error if \c conn is inactive or \c sql is an invalid query.
    template <typename Connection>
    inline typename Connection::int_type execute_scalar(Connection const& conn, std::string const& sql)
    {
        return execute_scalar<typename Connection::int_type>(conn, sql);
    }
}

#endif // #if !defined( CRYO_RELATIONAL_EXECUTE_SCALAR_HPP )
