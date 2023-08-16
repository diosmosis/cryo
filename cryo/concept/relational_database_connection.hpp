///////////////////////////////////////////////////////////////////////////////
//
//! relational_database_connection.hpp
//! Contains an archetype for the \c RelationalDatabaseConnection concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_RELATIONAL_DATABASE_CONNECTION_HPP )
#define CRYO_CONCEPT_RELATIONAL_DATABASE_CONNECTION_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/concept/cursor.hpp>
#include <cryo/concept/transaction_scope.hpp>
#include <string>

namespace cryo { namespace concepts
{
    //! \brief Concept that categorizes types which create and manage connections to
    //!        relational database systems.
    struct RelationalDatabaseConnection
    {
        typedef int Integer;
        typedef unsigned int UnsignedInteger;

        //! \brief The signed integer type used by the database system the model connects to.
        //! 
        //! This should be an signed version of \ref uint_type.
        typedef Integer int_type;

        //! \brief The unsigned integer type used by the database system the model connects to.
        //! 
        //! This will be the integer type returned from a \c COUNT SQL call.
        typedef UnsignedInteger uint_type;

        //! \brief The cursor type for the database system the model connects to.
        typedef Cursor cursor_type;

        //! \brief The \ref TransactionScope type that can be used with this connection to start and manage
        //!        transactions.
        typedef TransactionScope transaction_scope_type;

        // not required for concept
        typedef int RelationalDatabaseConnection::* safe_bool_type;

        //! \brief Sends SQL to the database system for execution.
        //! \param sql The SQL to execute. If \c sql is an empty string, the call is a no-op.
        //! \returns A \ref Cursor that can be used to read selected data. If the SQL used did not
        //!          select any data, a null Cursor is returned for which <tt>static_cast<bool>(cursor) == false</tt>.
        //! \throws error if the connection is inactive.
        cursor_type execute_query(std::string const& sql) const
        {
            return cursor_type();
        }

        //! \brief Returns the number of rows affected by an \c UPDATE or \c DELETE statement.
        //! \remark If the connection is not active, the behavior of this function is undefined.
        //! \remark If no query has been executed or the last query executed was not an \c UPDATE
        //!         or \c DELETE, the behavior of this function is undefined.
        //! \returns The number of rows affected by the last query if it was an \c UPDATE or
        //!          \c DELETE statement.
        uint_type affected_row_count() const
        {
            return 0;
        }

        //! \brief Tests whether the connection is active or not.
        //! \returns A value convertible to \c true if the connection is valid, convertible to
        //!          \c false if otherwise.
        operator safe_bool_type() const
        {
            return 0;
        }
    };
}}

#endif // #if !defined( CRYO_CONCEPT_RELATIONAL_DATABASE_CONNECTION_HPP )
