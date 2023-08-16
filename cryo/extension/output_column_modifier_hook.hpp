///////////////////////////////////////////////////////////////////////////////
//
//! \file output_column_modifier_hook.hpp
//! Contains the \ref output_column_modifier_hook<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_OUTPUT_COLUMN_MODIFIER_HOOK_HPP )
#define CRYO_EXTENSION_OUTPUT_COLUMN_MODIFIER_HOOK_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension/unspecialized_hook.hpp>

namespace cryo { namespace extension
{
    //! \brief Allows backends to append SQL to column definitions in a <tt>CREATE TABLE</tt>
    //!        statement based on a column's field type and its associated field tags.
    //!
    //! This hook is executed by \ref create_table_sql after each column definition is
    //! outputted. It's specialized using the specific backend's connection, every column of the
    //! table being created, and every tag of each column's entity field type. Each specialization
    //! that does not derive from \ref unspecialized_hook is executed. cryo backends must use either
    //! this hook or \ref output_exterior_column_modifier_hook to guarantee that the schema provides
    //! the behavior promised by field tags such as \ref primary_key or \ref foreign_key<>.
    //! 
    //! Specializations must define the following operator:
    //! <tt>void operator()(std::stringstream &) const</tt>
    //! This operator outputs the necessary SQL.
    //! 
    //! \tparam Connection the backend Connection type. Models \ref RelationalDatabaseConnection.
    //! \tparam C the column descriptor. Will be a specialization of \ref entity_column<>.
    //! \tparam T the column field type. Same as <tt>field_type_of<C>::type</tt>. This template
    //!           parameter exists so it can be specialized on.
    //! \tparam WrapperTag one of \c T's tags.
    template <typename Connection, typename C, typename T, typename Tag>
    struct output_column_modifier_hook
        : unspecialized_hook
    {};
}}

#endif // #if !defined( CRYO_EXTENSION_OUTPUT_COLUMN_MODIFIER_HOOK_HPP )
