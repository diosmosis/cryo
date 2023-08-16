///////////////////////////////////////////////////////////////////////////////
//
//! \file error.hpp
//! Holds the \ref error exception type.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_ERROR_HPP )
#define CRYO_ERROR_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/system/system_error.hpp>

namespace cryo
{
    //! \brief The exception type used throughout cryo.
    //!
    //! Other than being an exception type, \ref error does nothing but hold the
    //! exception message and the error code supplied by the underlying backend.
    //! \ref error derives from Boost.System's \c system_error type, so the error code,
    //! a Boost.System \c error_code instance, can be accessed via the inherited
    //! \c code() member function.
    struct error
        : boost::system::system_error
    {
        //! \brief Constructor.
        //! \remark Assumes no error code available.
        //! \param what_arg The error message. Should explain why this error was thrown.
        error(std::string const& what_arg)
            : boost::system::system_error(boost::system::error_code(), what_arg)
        {}

        //! \brief Constructor.
        //! \remark Uses blank error message.
        //! \param ec The \c error_code describing the type of error that occurred.
        error(boost::system::error_code ec)
            : boost::system::system_error(ec)
        {}

        //! \brief Constructor.
        //! \param ec The \c error_code describing the type of error that occurred.
        //! \param what_arg The error message. Should explain why this error was thrown.
        error(boost::system::error_code ec, std::string const& what_arg)
            : boost::system::system_error(ec, what_arg)
        {}

        //! \brief Constructor.
        //! \remark Uses blank error message.
        //! \param ev An integer representing the type of error that occurred. This value is
        //!           interpreted by \c ecat.
        //! \param ecat The \c error_category type that interprets the meaning of \c ev.
        error(int ev, boost::system::error_category const& ecat)
            : boost::system::system_error(ev, ecat)
        {}

        //! \brief Constructor.
        //! \param ev An integer representing the type of error that occurred. This value is
        //!           interpreted by \c ecat.
        //! \param ecat The \c error_category type that interprets the meaning of \c ev.
        //! \param what_arg The error message. Should explain why this error was thrown.
        error(int ev, boost::system::error_category const& ecat, std::string const& what_arg)
            : boost::system::system_error(ev, ecat, what_arg)
        {}

        //! \brief Retrieves the error message along with the backend name and a description of
        //! the error type (if available).
        //! \returns The name of the backend that threw this error, followed by a description of
        //!          the error type and the error message supplied during construction.
        const char * what() const throw()
        {
            if (buff.empty())
            {
                try
                {
                    buff = code().category().name();

                    buff += ": ";
                    buff += this->std::runtime_error::what();

                    std::string ec_what = code().message();

                    if (!ec_what.empty())
                    {
                        if (!buff.empty())
                        {
                            buff += ": ";
                        }

                        buff += ec_what;
                    }
                }
                catch (...)
                {
                    return this->std::runtime_error::what();
                }
            }

            return buff.c_str();
        }

    protected:
        // NOTE: If system_error's version of this member (m_what) were made protected, I wouldn't need this...
        mutable std::string buff;
    };
}

#endif // #if !defined( CRYO_ERROR_HPP )
