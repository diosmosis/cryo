///////////////////////////////////////////////////////////////////////////////
//
//! \file cursor.hpp
//! Contains an archetype for the \ref Cursor concept.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_CONCEPT_CURSOR_HPP )
#define CRYO_CONCEPT_CURSOR_HPP

#include <cryo/cryo_fwd.hpp>
#include <string>

namespace cryo { namespace concepts
{
    // TODO: Need to rethink this concept? Should it be CursorReference? What's a cursor then?
    //! \brief This concept describes types that iterate over selected data.
    //!
    //! \ref Cursors are created by their associated \ref RelationalDatabaseConnection type
    //! and iterate over both the rows of selected data and the columns of each row.
    //!
    //! \remark \ref Cursors are reference types that reference an intangible value.
    struct Cursor
    {
        // no archetype for a Boost.Iostreams Source. only for readability, not required.
        typedef int boost_iostreams_source;
        typedef int Cursor::* unspecified_bool_type;

        //! \brief The \ref Cursor's source type for reading blob data.
        //!
        //! This type must model the Boost.Iostreams' \c Source concept.
        typedef boost_iostreams_source blob_source_type;

        //! \brief Reads the current column as an \c Integral and sets \c value to the
        //!        selected column data.
        //!
        //! The behavior of this function is undefined if any of the following are true:
        //! \li the \ref Cursor is at the last row
        //! \li the \ref Cursor is at the last column
        //! \li the column is null
        //! \li the data selected is not an integral
        //! \tparam Integral Any C++ integral type, signed or unsigned.
        //! \param value The integer to assign the current selected data to.
        template <typename Integral>
        void get_column(Integral & value) const
        {
            // empty
        }

        //! \brief Reads the current column as a \c float and sets \c value to the selected
        //! column data.
        //!
        //! The behavior of this function is undefined if any of the following are true:
        //! \li the \ref Cursor is at the last row
        //! \li the \ref Cursor is at the last column
        //! \li the column is null
        //! \li the data selected is not easily convertible to a floating point
        //! \param value The \c float to assign the current selected data to.
        void get_column(float & value) const
        {
            // empty
        }

        //! \brief Reads the current column as a \c double and sets \c value to the selected
        //! column data.
        //!
        //! The behavior of this function is undefined if any of the following are true:
        //! \li the \ref Cursor is at the last row
        //! \li the \ref Cursor is at the last column
        //! \li the column is null
        //! \li the data selected is not a double
        //! \param value The \c double to assign the current selected data to.
        void get_column(double & value) const
        {
            // empty
        }

        // TODO: Need a section on how to non-intrusively get cryo to notice and use unicode types.
        //! \brief Reads the current column as a string and sets \c value to the selected
        //! column data.
        //!
        //! The behavior of this function is undefined if any of the following are true:
        //! \li the \ref Cursor is at the last row
        //! \li the \ref Cursor is at the last column
        //! \li the column is null
        //! \li the data selected is not text data
        //! \param value The \c std::string to assign the current selected data to.
        void get_column(std::string & value) const
        {
            // empty
        }

        // TODO: Need to define how cryo sees 'blobs'
        //! \brief Provides a stream source to read the current selected data, if the data is
        //!        a blob.
        //!
        //! The behavior of this function is undefined if any of the following are true:
        //! \li the \ref Cursor is at the last row
        //! \li the \ref Cursor is at the last column
        //! \li the column is null
        //! \li the data selected is not a blob type
        //! \returns A Boost.Iostreams \c Source type tied to the selected blob. Can be used
        //!          to stream blob data.
        blob_source_type get_column_blob() const
        {
            return blob_source_type();
        }

        //! \brief Tests whether the current selected data is null.
        //!
        //! The behavior of this function is undefined if any of the following are true:
        //! \li the \ref Cursor is at the last row
        //! \li the \ref Cursor is at the last column
        //! \returns \c true if the current selected data is null, \c false otherwise.
        bool is_column_null() const
        {
            return true;
        }

        //! \brief Iterates to the next column.
        //!
        //! The behavior of this function is undefined if any of the following are true:
        //! \li the \ref Cursor is at the last row
        //! \li the \ref Cursor is at the last column
        void next_column()
        {
            // empty
        }

        //! \brief Iterates to the next row.
        //!
        //! If the new row is not the last row, this function resets the current column to the
        //! first of the new row.
        //!
        //! The behavior of this function is undefined if any of the following are true:
        //! \li the \ref Cursor is at the last row
        void next()
        {
            // empty
        }

        //! \brief Tests whether the \ref Cursor is past the last row or not.
        //! \returns \c true if the \ref Cursor is past the last row in the result set, \c false
        //! otherwise.
        bool at_end() const
        {
            return true;
        }

        //! \brief Tests whether the \ref Cursor is past the last column of selected data.
        //!
        //! The behavior of this function is undefined if any of the following are true:
        //! \li the \ref Cursor is at the last row
        bool at_column_end() const
        {
            return true;
        }
    };
}}

#endif // #if !defined( CRYO_CONCEPT_CURSOR_HPP )
