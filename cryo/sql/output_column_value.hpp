///////////////////////////////////////////////////////////////////////////////
//
//! \file output_column_value.hpp
//! Contains the \ref output_column_value free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_OUTPUT_COLUMN_VALUE_HPP )
#define CRYO_SQL_OUTPUT_COLUMN_VALUE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/detail/switch.hpp>
#include <cryo/extension/output_value_hook.hpp>
#include <cryo/schema/introspect/primary_key_of.hpp>
#include <cryo/schema/introspect/is_entity.hpp>
#include <cryo/schema/tag/nullable.hpp>
#include <cryo/schema/tag/is_wrapper.hpp>
#include <cryo/schema/tag/blob.hpp>
#include <cryo/schema/introspect/find_tag_if.hpp>
#include <cryo/detail/unwrap.hpp>
#include <cryo/detail/make_member_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <sstream>
#include <string>

namespace cryo { namespace sql
{
    template <typename Connection>
    struct output_column_value_impl
        : detail::switch_<
            output_column_value_impl<Connection>,
            void (std::stringstream &),
            boost::mpl::vector<
                is_nullable<cryo::detail::make_member_ptr<boost::mpl::_1> >,
                is_blob<cryo::detail::make_member_ptr<boost::mpl::_1> >,
                is_wrapper<boost::mpl::_1>,
                is_entity<boost::mpl::_1>
            >
        >
    {
        // is_nullable
        template <typename T>
        void case_(std::stringstream & ss, T const& x, boost::mpl::int_<0>) const
        {
            if (x)
            {
                (*this)(ss, *x);
            }
            else
            {
                ss << "NULL";
            }
        }

        // is_blob
        template <typename T>
        void case_(std::stringstream & ss, T const& x, boost::mpl::int_<1>) const
        {
            typedef typename find_tag_if<
                typename cryo::detail::make_member_ptr<T>::type,
                cryo::detail::is_blob_tag<boost::mpl::_1>
            >::type blob_tag;
            typedef typename blob_tag::serializer_type serializer_type;

            serializer_type s;
            s.serialize(ss, cryo::unwrap(x));
        }

        // is_wrapper
        template <typename T>
        void case_(std::stringstream & ss, T const& x, boost::mpl::int_<2>) const
        {
            (*this)(ss, cryo::dereference(x));
        }

        // is_entity
        template <typename T>
        void case_(std::stringstream & ss, T const& x, boost::mpl::int_<3>) const
        {
            (*this)(ss, primary_key_of(x));
        }

        // none-of-the-above
        template <typename T>
        void default_(std::stringstream & ss, T const& x) const
        {
            extension::output_value_hook<Connection, T>()(ss, x);
        }
    };

    //! \brief Outputs a value to a \c std::stringstream as it should appear in the SQL of
    //!        a specific database type.
    //! \remark This function will invoke the \ref output_value_hook<> hook.
    //! \tparam Connection a \ref RelationalDatabaseConnection specifying the database that
    //!                    must recognize the outputted value.
    //! \param ss the \c std::stringstream to output to.
    //! \param x the value to output. The type of this value should retain \ref ColumnTags,
    //!          if retrieved from an entity.
    template <typename Connection, typename T>
    inline void output_column_value(std::stringstream & ss, T const& x)
    {
        output_column_value_impl<Connection>()(ss, x);
    }
}}

#endif // #if !defined( CRYO_SQL_OUTPUT_COLUMN_VALUE_HPP )
