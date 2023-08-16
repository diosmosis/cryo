///////////////////////////////////////////////////////////////////////////////
//
//! \file is_entity.hpp
//! Contains the \ref is_entity<> metafunction.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_IS_ENTITY_HPP )
#define CRYO_SCHEMA_INTROSPECT_IS_ENTITY_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/mpl/bool.hpp>

namespace cryo
{
    //! \brief Tests whether a type is an entity or not.
    //!
    //! This type must be specialized when creating a new entity type. If, however, the
    //! \ref CRYO_DECLARE_ENTITY macro is used, there is no need as the macro will
    //! specialize this type itself.
    //!
    //! \tparam E the type to test.
    //! \returns \c boost::mpl::true_ if \c E is an entity type, \c boost::mpl::false_
    //!          if otherwise.
    template <typename E>
    struct is_entity
        : boost::mpl::false_
    {};

    template <typename E>
    struct is_entity<E const>
        : is_entity<E>
    {};
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_IS_ENTITY_HPP )
