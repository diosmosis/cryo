///////////////////////////////////////////////////////////////////////////////
//
//! \file blob_serializer.hpp
//! Contains an archetype for the \ref BlobSerializer concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_BLOB_SERIALIZER_HPP )
#define CRYO_CONCEPT_BLOB_SERIALIZER_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace concepts
{
    //! \brief Concept that categorizes types used to control how data is serialized
    //!        to and deserialized from \c BLOB columns.
    //!
    //! \remark Types that model \c BlobSerializer can impose requirements on what
    //!         types can and can't be serialized.
    struct BlobSerializer
    {
        //! \brief A metafunction that returns the maximum size in bytes of \c T when
        //!        serialized.
        //!
        //! This metafunction is used by backend implementations to constrain the size
        //! of \c BLOB columns. If this metafunction returns \c -1, no maximum size is
        //! specified when creating tables.
        template <typename T>
        struct max_serialized_size;

        //! \brief Default constructor.
        BlobSerializer() {}

        //! \brief Reads data from \c src that was serialized using this type's \c serialize
        //!        function, and deserializes it into \c value.
        //!
        //! \param src a Boost.Iostreams \c Source type from which blob data is read.
        //! \param value the field value to deserialize into.
        template <typename Source, typename T>
        void deserialize(Source & src, T & value)
        {
            // empty
        }

        //! \brief Serializes the supplied data to a \c std::stringstream as it would
        //!        appear in SQL.
        //!
        //! \param ss the \c std::stringstream to write to.
        //! \param value the data to serialize
        template <typename T>
        void serialize(std::stringstream & ss, T const& value)
        {
            // empty
        }
    };
}}

#endif // #if !defined( CRYO_CONCEPT_BLOB_SERIALIZER_HPP )
