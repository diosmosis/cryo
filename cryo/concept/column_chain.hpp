///////////////////////////////////////////////////////////////////////////////
//
//! \brief column_chain.hpp
//! Contains an archetype for the \ref ColumnChain concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_COLUMN_CHAIN_HPP )
#define CRYO_CONCEPT_COLUMN_CHAIN_HPP

#include <cryo/cryo_fwd.hpp>

namespace cryo { namespace concepts
{
    //! \brief A compile time list of chained \ref PartialColumnDescriptors that describe
    //!        a possible path that can be taken when selecting column data.
    struct ColumnChain
        : PartialColumnDescriptor
    {
        //! \brief Appends \c c to this \ref ColumnChain's list of \ref PartialColumnDescriptors
        //!        and returns the result.
        //! \param c a \ref PartialColumnDescriptor to append. Types that model \ref ColumnChain
        //!          must make sure \c c describes a column of the field type of the last column in
        //!          this chain. If the field type is not an entity, this operator should not be
        //!          defined.
        //! \returns a new \ref ColumnChain with \c c appended to this \ref ColumnChain's list of
        //!          \ref PartialColumnDescriptors.
        ColumnChain operator()(PartialColumnDescriptor c) const
        {
            return ColumnChain();
        }
    };
}}

#endif // #if !defined( CRYO_CONCEPT_COLUMN_CHAIN_HPP )
