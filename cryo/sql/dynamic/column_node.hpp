///////////////////////////////////////////////////////////////////////////////
//
//! \file column_node.hpp
//! Contains the \ref column_node type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_DYNAMIC_COLUMN_NODE_HPP )
#define CRYO_SQL_DYNAMIC_COLUMN_NODE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/extension/abstract_hooks.hpp>
#include <boost/intrusive/set_hook.hpp>
#include <boost/range/iterator_range.hpp>
#include <sstream>

namespace cryo { namespace sql { namespace dynamic
{
    typedef unsigned int member_ptr_int_t;

    //! \brief The base type of all \ref ColumnChain types that can be held in a
    //!        \ref join_index_set.
    struct column_node
        : boost::intrusive::set_base_hook<>
    {
        typedef boost::iterator_range<member_ptr_int_t const*> column_range_t;

        //! \brief Default constructor. Initializes the stored join index to \c 0, which
        //!        refers to the \c FROM table.
        column_node()
            : join_idx(0)
        {}

        //! \brief Copy constructor.
        //! \param x the \ref column_node to copy.
        column_node(column_node const& x)
            : join_idx(x.join_idx)
        {}

        //! \brief Destructor.
        virtual ~column_node() {}

        //! \brief Pure virtual function that provides access to a range of pointer-to-member
        //!        \ref PartialColumnDescriptors converted to unsigned integers.
        //! This range identifies one path that can be taken starting from the \c FROM entity.
        virtual column_range_t column_range() const = 0;

        //! \brief Outputs the join described by this \ref ColumnChain.
        //! \remark This method will not work correctly if \ref calc_join_indices has not
        //!         been called with the expression that contains this node.
        //! \param ss the \c std::stringstream to output to.
        //! \param hooks contains database specific SQL generation logic used to vary SQL generation
        //!              without knowing what the specific database is.
        virtual void output_join(std::stringstream & ss, extension::abstract_hooks const& hooks) const = 0;

        //! \brief The index referring to the join this \ref ColumnChain describes.
        unsigned int join_idx;
    };
}}}

#endif // #if !defined( CRYO_SQL_DYNAMIC_COLUMN_NODE_HPP )
