///////////////////////////////////////////////////////////////////////////////
//
//! \file blob_reference.hpp
//! Contains the \ref blob_reference type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_BLOB_REFERENCE_HPP )
#define CRYO_SCHEMA_BLOB_REFERENCE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/all_tags_of.hpp>

namespace cryo
{
    //! \brief A special type used to denote a \c BLOB column that can only be
    //!        streamed.
    //! \remark cryo will ignore these columns when selecting data.
    struct blob_reference {};

    template <>
    struct all_tags_of<blob_reference>
    {
        typedef boost::mpl::vector<tag::nullable, tag::blob<void> > type;
    };
}

#endif // #if !defined( CRYO_SCHEMA_BLOB_REFERENCE_HPP )
