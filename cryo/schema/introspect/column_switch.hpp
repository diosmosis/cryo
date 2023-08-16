///////////////////////////////////////////////////////////////////////////////
//
//! \file column_switch.hpp
//! Contains the \ref column_switch free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SCHEMA_INTROSPECT_COLUMN_SWITCH_HPP )
#define CRYO_SCHEMA_INTROSPECT_COLUMN_SWITCH_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/schema/introspect/columns_of.hpp>
#include <cryo/schema/introspect/field_type_of.hpp>
#include <cryo/schema/entity_metadata.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/filter_if.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/switch.hpp>

namespace cryo
{
    namespace detail
    {
        template <typename T, typename Entity>
        struct find_column_index
        {
            find_column_index(T Entity::* column_)
                : column(column_)
                , result(-1)
            {}

            template <typename C>
            void operator()(C c) const
            {
                if (column == c.member())
                {
                    result = C::index;
                }
            }

            T Entity::* column;
            mutable int result;
        };

        // NOTE: Currently this function is O(N). Unfortunately, since this function is used for every function
        // that accesses entity metadata at runtime, this has to change. It could easily be O(1) IF it were
        // possible to get the integer representation of a pointer-to-member at compile time. I don't think its
        // possible, though...
        template <typename Entity, typename T>
        inline unsigned int column_index_of(T Entity::* column)
        {
            typedef boost::is_same<T, field_type_of<boost::mpl::_1> > predicate_type;

            detail::find_column_index<T, Entity> fci(column);
            boost::fusion::for_each(boost::fusion::filter_if<predicate_type>(columns_of<Entity>()), fci);

            return fci.result;
        }
    }

    //! \brief Invokes \c function with a Boost.MPL integral constant representing the
    //!        the index of the column specified by \c column.
    //!
    //! This function can be used to get information only available from a \ref ColumnDescriptor
    //! when all that is available is a \ref PartialColumnDescriptor.
    //!
    //! \param column a \ref PartialColumnDescriptor. Must be a valid pointer-to-member.
    //! \param function a unary function object that accepts Boost.MPL integral constants
    //!        as arguments.
    //! \throws boost::bad_switch<unsigned int> if \c column does not reference a valid
    //!         entity column.
    template <typename Entity, typename T, typename F>
    inline typename F::result_type column_switch(T Entity::* column, F & function)
    {
        typedef boost::mpl::range_c<unsigned int, 0, entity_metadata<Entity>::column_count> column_range;

        unsigned int ci = detail::column_index_of(column);

        return boost::switch_<column_range>(ci, function);
    }
}

#endif // #if !defined( CRYO_SCHEMA_INTROSPECT_COLUMN_SWITCH_HPP )
