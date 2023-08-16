///////////////////////////////////////////////////////////////////////////////
//
//! \file output_datatype_hook.hpp
//! Contains the \ref output_datatype_hook<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_EXTENSION_OUTPUT_DATATYPE_HOOK_HPP )
#define CRYO_EXTENSION_OUTPUT_DATATYPE_HOOK_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension/unspecialized_hook.hpp>

namespace cryo { namespace extension
{
    // TODO: Need to define 'unwrapped field type'/'wrapped field type' somewhere. Also need to define what constitutes a primitive type.
    //! \brief Allows backends to map C++ types to backend types based on a column's
    //!        field type and associated field tags.
    //! 
    //! This hook is executed by \ref output_column_datatype when creating a <tt>CREATE TABLE</tt>
    //! statement. It's specialized by the backend connection, each column's unwrapped field type and
    //! each column's field tags, but only the first specialization found is executed. In this way,
    //! it is possible for the presence of a field tag to output a different datatype than would
    //! be outputted w/o the tag. One case this can be useful is in mapping BLOB types which are
    //! signified not by a C++ datatype, but by the \ref tag::blob tag.
    //!
    //! Specializations must define the following operator:
    //! <tt>void operator()(std::stringstream &) const</tt>
    //! This operator outputs the necessary SQL.
    //!
    //! \remarks Field tag order is important. See \ref all_tags_of<> for more info.
    //!
    //! \tparam Connection the backend connection type. Models \ref RelationalDatabaseConnection.
    //! \tparam T the unwrapped column field type. Will be a cryo primitive or entity.
    //! \tparam Tag one of the column field type's tags.
    template <typename Connection, typename T, typename Tag>
    struct output_datatype_hook
        : unspecialized_hook
    {};
}}

#endif // #if !defined( CRYO_EXTENSION_OUTPUT_DATATYPE_HOOK_HPP )
