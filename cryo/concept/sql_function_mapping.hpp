///////////////////////////////////////////////////////////////////////////////
//
//! \file sql_function_mapping.hpp
//! Contains an archetype for the \ref SqlFunctionMapping concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_SQL_FUNCTION_MAPPING_HPP )
#define CRYO_CONCEPT_SQL_FUNCTION_MAPPING_HPP

#include <cryo/cryo_fwd.hpp>
#include <sstream>

namespace cryo { namespace concepts
{
    //! \brief Concept that categorizes types that describe SQL functions.
    //!
    //! Types that model this concept can be used as the first terminal in a cryo
    //! function call expression in order to have an SQL function call outputted to
    //! generated SQL.
    struct SqlFunctionMapping
    {
        //! \brief Metafunction that returns the return type of the mapped function
        //!        with regard to a given database and the arguments used.
        //!
        //! If this function cannot be used with the arguments passed, this type should be
        //! undefined for that specific call. cryo will not be able to deduce the result type,
        //! so the invalid function use will be caught at compile time.
        //!
        //! \tparam Connection the \ref RelationalDatabaseConnection specifying the
        //!                    database the function will be called in.
        //! \tparam Sig a function type describing how the function will be called in
        //!             SQL. Will be a type along the lines of <tt>SqlFunctionMapping(int, int, ...)</tt>.
        //! \returns The result type of a call to the function in question with a certain
        //!          number and type of arguments.
        template <typename Connection, typename Sig>
        struct result
        {
            typedef int type;
        };

        //! \brief Outputs the name of the SQL function as it appears in SQL.
        //! \param ss the \c std::stringstream to output to.
        static void apply(std::stringstream & ss)
        {
            // empty
        }
    };
}}

#endif // #if !defined( CRYO_CONCEPT_SQL_FUNCTION_MAPPING_HPP )
