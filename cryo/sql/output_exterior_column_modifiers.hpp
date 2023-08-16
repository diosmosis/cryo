///////////////////////////////////////////////////////////////////////////////
//
//! \ref output_exterior_column_modifiers.hpp
//! Contains the \ref output_exterior_column_modifiers free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_OUTPUT_EXTERIOR_COLUMN_MODIFIERS_HPP )
#define CRYO_SQL_OUTPUT_EXTERIOR_COLUMN_MODIFIERS_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension/output_exterior_column_modifier_hook.hpp>
#include <cryo/schema/introspect/column_type_of.hpp>
#include <cryo/sql/detail/collect_specialized_hooks.hpp>
#include <cryo/sql/for_each.hpp>
#include <cryo/detail/indirect_invoke.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    namespace detail
    {
        template <typename Connection, typename C, typename T, typename Tag>
        struct make_exterior_column_modifier_hook
        {
            typedef extension::output_exterior_column_modifier_hook<Connection, C, T, Tag> type;
        };
    }

    //! \brief Outputs every exterior column modifier needed to describe the column \c c
    //!        as a comma delimited list for a specific database type.
    //! \remark This function will invoke the \ref output_exterior_column_modifier_hook<> hook for
    //!         every \ref ColumnTag \c c is decorated with.
    //! \tparam Connection a \ref RelationalDatabaseConnection specifying the database that
    //!                    must recognize the outputted column modifiers.
    //! \param ss the \c std::stringstream to output to.
    //! \param c the \ref ColumnDescriptor describing the column for whom exterior column
    //!          modifiers are to be outputted.
    template <typename Connection, typename C>
    inline void output_exterior_column_modifiers(std::stringstream & ss, C c)
    {
        typedef typename column_type_of<C>::type column_type;

        // get the hooks to invoke
        typedef typename detail::collect_specialized_hooks<
            C, detail::make_exterior_column_modifier_hook<Connection, C, column_type, boost::mpl::_1>
        >::type hooks;

        // invoke the hooks
        for_each(hooks(), cryo::detail::indirect_invoke(), ss, ',');
    }
}}

#endif // #if !defined( CRYO_SQL_OUTPUT_EXTERIOR_COLUMN_MODIFIERS_HPP )
