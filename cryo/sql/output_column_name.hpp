///////////////////////////////////////////////////////////////////////////////
//
//! \file output_column_name.hpp
//! Contains the \ref output_column_name free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_OUTPUT_COLUMN_NAME_HPP )
#define CRYO_SQL_OUTPUT_COLUMN_NAME_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/column_name_of.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    //! \brief Outputs the name of a column as it should appear in SQL for a specific
    //!        database type.
    //! \remark This function invokes the \ref column_name_of free function.
    //! \tparam Connection a \ref RelationalDatabaseConnection specifying the database
    //!                    that must recognize the outputted column name.
    //! \param ss the \c std::stringstream to output to.
    //! \param c a \ref PartialColumnDescriptor describing the column whose name is to be
    //!          outputted.
    template <typename Connection, typename C>
    inline void output_column_name(std::stringstream & ss, C c)
    {
        ss << column_name_of<C>();
    }

    template <typename Connection, typename T, typename Entity>
    inline void output_column_name(std::stringstream & ss, T Entity::* c)
    {
        ss << column_name_of(c);
    }
}}

#endif // #if !defined( CRYO_SQL_OUTPUT_COLUMN_NAME_HPP )
