///////////////////////////////////////////////////////////////////////////////
//
//! \file output_column_datatype.hpp
//! Contains the \ref output_column_datatype free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_OUTPUT_COLUMN_DATATYPE_HPP )
#define CRYO_SQL_OUTPUT_COLUMN_DATATYPE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension/output_datatype_hook.hpp>
#include <cryo/schema/entity_column.hpp>
#include <cryo/schema/introspect/column_type_of.hpp>
#include <cryo/schema/introspect/all_tags_of.hpp>
#include <cryo/schema/tag/foreign_key.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

namespace cryo
{
    namespace sql
    {
        namespace detail
        {
            template <typename Connection, typename T, typename Tag>
            struct make_datatype_hook
            {
                typedef extension::output_datatype_hook<Connection, T, Tag> type;
            };
        }

        template <typename Connection, typename T>
        struct unsupported_datatype_specialization
        {
            void operator()(std::stringstream &) const
            {
                static_assert(false, "T is not a recognized column type for Connection.");
            }
        };

        //! \brief Outputs the data type name of a column as it should appear in SQL for
        //!        a specific datatype.
        //! \remark This function finds the first \ref ColumnTag associated with \c c for
        //!         which the \ref output_datatype_hook<> hook is specialized, then invokes the
        //!         specialized hook.
        //! \tparam Connection a \ref RelationalDatabaseConnection specifying the database
        //!                    that must recognize the outputted data type.
        //! \param ss the \c std::stringstream to output to.
        //! \param c a \ref ColumnDescriptor describing the column whose data type's name is
        //!          to be outputted.
        template <typename Connection, typename C>
        inline void output_column_datatype(std::stringstream & ss, C c)
        {
            // TODO: Is there no way to clean this up?
            typedef typename column_type_of<C>::type column_type;

            typedef typename all_tags_of<typename C::field_type>::type all_tags;

            // create hooks for each tag
            typedef typename boost::mpl::transform<
                all_tags, detail::make_datatype_hook<Connection, column_type, boost::mpl::_1>
            >::type all_hooks;

            // find the first tag for which output_datatype_hook is specialized
            typedef typename boost::mpl::find_if<
                all_hooks,
                boost::mpl::not_<
                    boost::is_base_of<extension::unspecialized_hook, boost::mpl::_1>
                >
            >::type first_hook_i;

            // check if a hook was found
            typedef typename boost::mpl::not_<
                typename boost::is_same<
                    first_hook_i,
                    typename boost::mpl::end<all_tags>::type
                >::type
            >::type hook_is_found;

            // select a hook to invoke; if no specialization was found use one that gives an error
            typedef typename boost::mpl::eval_if<
                hook_is_found,
                boost::mpl::deref<first_hook_i>,
                boost::mpl::identity<unsupported_datatype_specialization<Connection, column_type> >
            >::type hook;

            // invoke the found hook type
            hook()(ss);
        }
    }

    namespace extension
    {
        template <typename Connection, typename T, typename F>
        struct output_datatype_hook<Connection, T, tag::foreign_key<F> >
        {
            void operator()(std::stringstream & ss) const
            {
                sql::output_column_datatype<Connection>(ss, entity_column<F, 0>());
            }
        };
    }
}

#endif // #if !defined( CRYO_SQL_OUTPUT_COLUMN_DATATYPE_HPP )
