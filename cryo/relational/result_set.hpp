///////////////////////////////////////////////////////////////////////////////
//
//! \file result_set.hpp
//! Contains the \ref result_set<> type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_RELATIONAL_RESULT_SET_HPP )
#define CRYO_RELATIONAL_RESULT_SET_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/detail/read_column.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/move/move.hpp>

namespace cryo
{
    //! \brief A Single Pass Range that iterates over the rows returned by an SQL query.
    //!
    //! \ref result_set<> uses a \ref Cursor to read row data, which is then used to create
    //! a local entity or Boost.Fusion sequence.
    //!
    //! \tparam Entity an entity or Boost.Fusion sequence. This type is used to store
    //!                the data found in one row.
    //! \tparam Cursor the database cursor type. Models \ref Cursor.
    template <typename Entity, typename Cursor>
    struct result_set
    {
        BOOST_MOVABLE_BUT_NOT_COPYABLE(result_set)

    public:
        typedef result_set<Entity, Cursor> this_type;
        typedef Cursor this_type::* unspecified_bool_type;

        struct iterator
            : boost::iterator_facade<
                iterator,
                Entity const,
                boost::forward_traversal_tag
            >    
        {
            iterator()
                : cursor(0)
                , n(0)
            {}

            iterator(iterator const& other)
                : cursor(other.cursor)
                , n(other.n)
                , entity(other.entity)
            {}

            explicit iterator(Cursor & cursor_)
                : cursor(&cursor_)
                , n(0)
                , entity()
            {
                fill_entity();
            }

            iterator & operator = (iterator const& x)
            {
                cursor = x.cursor;
                n = x.n;
                entity = x.entity;
                return *this;
            }

        private:
            friend class boost::iterator_core_access;

            bool equal(iterator const& other) const
            {
                return cursor == other.cursor && n == other.n;
            }

            void increment()
            {
                BOOST_ASSERT(cursor);

                cursor->next();

                fill_entity();
            }

            void fill_entity()
            {
                if (!cursor->at_end())
                {
                    detail::read_column<Cursor> read(cursor);
                    read(entity);
                }
                else
                {
                    cursor = 0;
                    n = 0;
                }
            }

            Entity const& dereference() const
            {
                return entity;
            }

            Cursor * cursor;
            int n;
            Entity entity;
        };

        typedef Entity const            value_type;
        typedef iterator                const_iterator;
        typedef value_type const*       pointer;
        typedef pointer                 const_pointer;
        typedef value_type const&       reference;
        typedef reference               const_reference;

        //! \brief Default constructor. Initializes this instance as an empty \c result_set<>.
        result_set()
            : cursor()
        {}

        //! \brief Move constructor.
        //! \param rs The temporary to move from.
        result_set(BOOST_RV_REF(result_set) rs)
            : cursor(rs.cursor)
        {}

        //! \brief Constructor.
        //! \param cursor_ A reference to the database cursor to read data from.
        explicit result_set(Cursor const& cursor_)
            : cursor(cursor_)
        {}

        //! \brief Move assignment.
        //! \param rs The temporary to move from.
        //! \returns \c *this
        result_set & operator = (BOOST_RV_REF(result_set) rs)
        {
            stmt = rs.stmt;
            return *this;
        }

        //! \brief Returns an iterator referencing the current row in the row set.
        //! If an iterator returned by this function is incremented, every iterator that refereces
        //! this range will change.
        //! \returns an iterator referencing the current row. Models a \c Single Pass Iterator.
        const_iterator begin() const
        {
            return cursor.at_end() ? const_iterator() : const_iterator(cursor);
        }

        //! \brief Returns an iterator representing the end of the row set.
        //! \returns an iterator representing the end of the row set.
        const_iterator end() const
        {
            return const_iterator();
        }

        //! \brief Tests whether this \ref result_set<> holds a valid cursor or not.
        //! \returns a value convertible to \c true if this range references a valid cursor, a value
        //!          convertible to \c false if not.
        operator unspecified_bool_type() const
        {
            return cursor ? &this_type::cursor : 0;
        }

    private:
        mutable Cursor cursor;
    };
}

#endif // #if !defined( CRYO_RELATIONAL_RESULT_SET_HPP )
