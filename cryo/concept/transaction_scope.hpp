///////////////////////////////////////////////////////////////////////////////
//
//! \file transaction.hpp
//! Contains an archetype for the \ref TransactionScope concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_TRANSACTION_SCOPE_HPP )
#define CRYO_CONCEPT_TRANSACTION_SCOPE_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/noncopyable.hpp>

namespace cryo { namespace concepts
{
    //! \brief Concept categorizing types that start and manage transactions for
    //!        relational databases.
    //!
    //! Models of \ref TransactionScope start a transaction upon construction
    //! and automatically rollback when destructing if the \ref commit member function
    //! was not called. Models must also avoid executing a rollback if an implicit
    //! rollback has occurred.
    //!
    //! \remark \ref TransactionScopes must be noncopyable.
    struct TransactionScope
        : boost::noncopyable
    {
        //! \brief Constructor. Creates the \ref TransactionScope instance and starts a
        //!        transaction using \c conn.
        //! \param conn The database connection to use. Must model
        //!        \ref RelationalDatabaseConnection and must be an active connection (
        //!        <tt>static_cast<bool>(conn) == true</tt>).
        TransactionScope(RelationalDatabaseConnection const& conn)
        {
            // empty
        }

        // TODO: If the rollback fails, how will anyone be notified if the destructor doesn't throw?
        //! \brief Destructor.
        //! \remark Must not throw.
        ~TransactionScope()
        {
            // empty
        }

        //! \brief Commits the transaction that was started in the constructor.
        void commit()
        {
            // empty
        }
    };
}}

#endif // #if !defined( CRYO_CONCEPT_TRANSACTION_SCOPE_HPP )
