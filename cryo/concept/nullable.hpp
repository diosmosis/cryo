///////////////////////////////////////////////////////////////////////////////
//
//! \file nullable.hpp
//! Contains an archetype for the \ref Nullable concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_NULLABLE_HPP )
#define CRYO_CONCEPT_NULLABLE_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace concepts
{
    //! \brief Concept categorizing types that both wrap another type and can be set
    //!        to a 'null' value.
    //! The null value is determined by the specific \ref Nullable implementation.
    struct Nullable
    {
        typedef unspecified int;
        typedef unspecified_bool_type bool;

        //! \brief Tests whether the \ref Nullable instance is null or not.
        //! \returns a value convertible to \c true if \c *this is not null, false if
        //!          otherwise.
        operator unspecified_bool_type() const
        {
            return true;
        }

        //! \brief Sets the supplied \ref Nullable instance to a value that is
        //!        recognized by the \ref Nullable model as null.
        //! \param value the \ref Nullable instance to modify.
        friend void set_null(Nullable & value)
        {
            // empty
        }

        //! \brief Gets the data wrapped by the \ref Nullable instance.
        //! \remark If \c value is set to a null value, the behavior of this function
        //!         is undefined.
        //! \param value the \ref Nullable instance to access.
        //! \returns the data \c value references.
        friend unspecified & dereference(Nullable & value)
        {
            return 0;
        }

        //! \brief Default constructs the type wrapped by the supplied \ref Nullable type
        //!        and stores it in the supplied \ref Nullable instance.
        //! \param value the \ref Nullable instance to modify.
        friend void allocate_and_construct(Nullable & value)
        {
            // empty
        }
    };
}}

#endif // #if !defined( CRYO_CONCEPT_NULLABLE_HPP )
