///////////////////////////////////////////////////////////////////////////////
//
//! \file declare_entity.hpp
//! Contains the \ref CRYO_DECLARE_ENTITY and \ref CRYO_DECLARE_ENTITY_SINGLE
//! macros.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_DECLARE_ENTITY_HPP )
#define CRYO_SCHEMA_DECLARE_ENTITY_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/tag/is_wrapper.hpp>
#include <cryo/schema/introspect/wrapped_type_of.hpp>
#include <cryo/schema/introspect/primary_key_of.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <cryo/schema/entity_metadata.hpp>
#include <cryo/schema/entity_column.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>

#define CRYO_COLUMN_SPEC(r, data, i, elem)                                                                            \
    template <>                                                                                                       \
    struct entity_column<data, i + 1>                                                                                 \
    {                                                                                                                 \
        enum { index = i + 1 };                                                                                       \
                                                                                                                      \
        typedef BOOST_TYPEOF(&data::elem) member_type;                                                                \
        typedef data entity_type;                                                                                     \
        typedef field_type_of<member_type>::type field_type;                                                          \
                                                                                                                      \
        member_type member() const                                                                                    \
        {                                                                                                             \
            return &data::elem;                                                                                       \
        }                                                                                                             \
                                                                                                                      \
        char const* name() const                                                                                      \
        {                                                                                                             \
            return BOOST_PP_STRINGIZE(elem);                                                                          \
        }                                                                                                             \
    };

#define CRYO_PRIMARY_COLUMN_SPEC(entity, primary)                                                                     \
    template <>                                                                                                       \
    struct entity_column<entity, 0>                                                                                   \
    {                                                                                                                 \
        enum { index = 0 };                                                                                           \
                                                                                                                      \
        typedef BOOST_TYPEOF(&entity::primary) member_type;                                                           \
        typedef entity entity_type;                                                                                   \
        typedef field_type_of<member_type>::type field_type;                                                          \
                                                                                                                      \
        member_type member() const                                                                                    \
        {                                                                                                             \
            return &entity::primary;                                                                                  \
        }                                                                                                             \
                                                                                                                      \
        char const* name() const                                                                                      \
        {                                                                                                             \
            return BOOST_PP_STRINGIZE(primary);                                                                       \
        }                                                                                                             \
    };

#define CRYO_ENTITY_METADATA_SPEC(entity, column_count_value)                                                         \
        template <>                                                                                                   \
        struct entity_metadata<entity>                                                                                \
        {                                                                                                             \
            static const int column_count = column_count_value;                                                       \
                                                                                                                      \
            static char const* table_name()                                                                           \
            {                                                                                                         \
                return BOOST_PP_STRINGIZE(entity);                                                                    \
            }                                                                                                         \
        };                                                                                                            \

//! \brief This macro creates the metadata required to map entities to
//!        database tables and entitity fields to database columns.
//!
//! By default, entities and fields are mapped to tables and columns of
//! the same name with any \c '::' bits replaced with underscores.
//!
//! \param entity the entity type.
//! \param primary a pointer-to-member describing the primary column of
//!                the entity.
//! \param cols a Boost.Preprocessor sequence holding pointer-to-members
//!             for all of the entity's columns, i.e. <tt>(&T::col0)(&T::col1)</tt>.
#define CRYO_DECLARE_ENTITY(entity, primary, cols)                                                                    \
    namespace cryo                                                                                                    \
    {                                                                                                                 \
        template <>                                                                                                   \
        struct is_entity<entity>                                                                                      \
            : boost::mpl::true_                                                                                       \
        {};                                                                                                           \
                                                                                                                      \
        CRYO_PRIMARY_COLUMN_SPEC(entity, primary)                                                                     \
                                                                                                                      \
        BOOST_PP_SEQ_FOR_EACH_I(CRYO_COLUMN_SPEC, entity, cols)                                                       \
                                                                                                                      \
        CRYO_ENTITY_METADATA_SPEC(entity, BOOST_PP_SEQ_SIZE(cols) + 1)                                                \
    }

//! \brief This macro creates the metadata required to map entities with
//!        only one column to database tables.
//!
//! By default, entities and fields are mapped to tables and columns of
//! the same name with any \c '::' bits replaced with underscores.
//!
//! \param entity the entity type.
//! \param primary a pointer-to-member describing the primary column of
//!                the entity.
#define CRYO_DECLARE_ENTITY_SINGLE(entity, primary)                                                                   \
    namespace cryo                                                                                                    \
    {                                                                                                                 \
        template <>                                                                                                   \
        struct is_entity<entity>                                                                                      \
            : boost::mpl::true_                                                                                       \
        {};                                                                                                           \
                                                                                                                      \
        CRYO_PRIMARY_COLUMN_SPEC(entity, primary)                                                                     \
                                                                                                                      \
        CRYO_ENTITY_METADATA_SPEC(entity, 1)                                                                          \
    }

#endif // #if !defined( CRYO_SCHEMA_DECLARE_ENTITY_HPP )
