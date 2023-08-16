///////////////////////////////////////////////////////////////////////////////
//
//! \file enumerate.hpp
//! Contains the \ref enumerate helper function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_ENUMERATE_HPP )
#define CRYO_SQL_ENUMERATE_HPP

#include <cryo/cryo_fwd.hpp>
#include <cryo/sql/for_each.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/pop_front.hpp>
#include <boost/mpl/empty.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    //! \brief Calls \c f using \c ss and every value in \c seq. \c sep is outputted to \ss
    //!        between each invocation of \c f.
    //! \param seq a Boost.Fusion sequence.
    //! \param f a function object that accepts a \c std::stringstream as its first argument
    //!          and an unspecified value as another.
    //! \param ss the \c std::stringstream to pass to \c f.
    //! \param sep a character to output between each call to \c f.
    template <typename S, typename F>
    inline typename boost::disable_if<
        boost::mpl::empty<S>, void
    >::type enumerate(S const& seq, F const& f, std::stringstream & ss, char sep)
    {
        f(ss, boost::fusion::front(seq));

        for_each(boost::fusion::pop_front(seq), f, ss, sep);
    }

    template <typename S, typename F>
    inline typename boost::enable_if<
        boost::mpl::empty<S>, void
    >::type enumerate(S const& seq, F const& f, std::stringstream & ss, char sep)
    {
        // empty
    }
}}

#endif // #if !defined( CRYO_SQL_ENUMERATE_HPP )
