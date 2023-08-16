///////////////////////////////////////////////////////////////////////////////
//
//! \file raw_byte_serializer.hpp
//! Holds the \ref raw_byte_serializer type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SUPPORT_RAW_BYTE_SERIALIZER_HPP )
#define CRYO_SUPPORT_RAW_BYTE_SERIALIZER_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/error.hpp>
#include <boost/mpl/int.hpp>
#include <sstream>

namespace cryo
{
    //! \brief A \ref BlobSerializer that persists C++ types as they are represented
    //!        in memory.
    //!
    //! \remark Only POD types should be used with this serializer. Anything else will
    //!         result in undefined behavior when deserializing.
    struct raw_byte_serializer
    {
        //! \brief Metafunction that returns the maximum serialized size of a type.
        //!
        //! Since a \ref raw_byte_serializer serializes data as it exists in memory,
        //! the size of the stored data will always be <tt>sizeof(T)</tt>.
        template <typename T>
        struct max_serialized_size
            : boost::mpl::int_<sizeof(T)>
        {};

        //! \brief Reads data from a Boost.Iostreams \c Source into a \c T instance.
        //!
        //! \remark If \c value isn't a POD type, this behavior will result in undefined
        //!         behavior.
        //! \param src the Boost.Iostreams \c Source to read from.
        //! \param value the \c T instance to read data into.
        template <typename Source, typename T>
        void deserialize(Source & src, T & value)
        {
            typedef typename Source::char_type char_type;

            char_type * buffer = reinterpret_cast<char_type *>(&value);
            char_type * end = buffer + sizeof(T);

            while (buffer < end)
            {
                std::streamsize sz = src.read(buffer, end - buffer);

                if (sz == -1)
                {
                    break;
                }

                buffer += sz;
            }

            if (buffer != end)
            {
                throw error("BLOB data cannot be deserialized; data found is of incorrect length");
            }
        }

        //! \brief Writes a hex string that represents \c value in memory to a
        //!        \c std::stringstream.
        //! \param ss the \c std::stringstream to write to.
        //! \param value the type to serialize.
        template <typename T>
        void serialize(std::stringstream & ss, T const& value)
        {
            static const char hex[] = {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
            };

            unsigned char const* f = reinterpret_cast<unsigned char const*>(&value);
            unsigned char const* l = f + sizeof(T);

            ss << "X'";
            for (; f != l; ++f)
            {
                ss.put(hex[*f / 16]);
                ss.put(hex[*f % 16]);
            }
            ss << '\'';
        }
    };
}

#endif // #if !defined( CRYO_SUPPORT_RAW_BYTE_SERIALIZER_HPP )
