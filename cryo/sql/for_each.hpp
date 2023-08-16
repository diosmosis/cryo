///////////////////////////////////////////////////////////////////////////////
//
//! \file for_each.hpp
//! Contains the \ref for_each free function.
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_SQL_FOR_EACH_HPP )
#define CRYO_SQL_FOR_EACH_HPP

#include <cryo/cryo_fwd.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <sstream>

namespace cryo { namespace sql
{
    namespace detail
    {
        template <typename F>
        struct wrap_for_each
        {
            wrap_for_each(F const& f_, std::stringstream & ss_)
                : f(f_)
                , ss(ss_)
            {}

            template <typename T>
            void operator()(T const& x) const
            {
                f(ss, x);
            }

            F const& f;
            std::stringstream & ss;
        };

        template <typename F>
        struct wrap_for_each_sep
        {
            wrap_for_each_sep(F const& f_, std::stringstream & ss_, char sep_)
                : f(f_)
                , ss(ss_)
                , sep(sep_)
            {}

            template <typename T>
            void operator()(T const& x) const
            {
                ss.put(sep);
                f(ss, x);
            }

            F const& f;
            std::stringstream & ss;
            char sep;
        };
    }

    //! \brief Calls \c f using \c ss and every value in \c seq.
    //! \param seq a Boost.Fusion sequence.
    //! \param f a function object that accepts a \c std::stringstream as its first argument
    //!          and an unspecified value as another.
    //! \param ss the \c std::stringstream to pass to \c f.
    template <typename S, typename F>
    inline void for_each(S const& seq, F const& f, std::stringstream & ss)
    {
        boost::fusion::for_each(seq, detail::wrap_for_each<F>(f, ss));
    }

    //! \brief Calls \c f using \c ss and every value in \c seq. Outputs \c sep to \c ss
    //!        before every invocation of \c f.
    //! \param seq a Boost.Fusion sequence.
    //! \param f a function object that accepts a \c std::stringstream as its first argument
    //!          and an unspecified value as another.
    //! \param ss the \c std::stringstream to pass to \c f.
    //! \param sep a character to output before each call to \c f.
    template <typename S, typename F>
    inline void for_each(S const& seq, F const& f, std::stringstream & ss, char sep)
    {
        boost::fusion::for_each(seq, detail::wrap_for_each_sep<F>(f, ss, sep));
    }
}}

#endif // #if !defined( CRYO_SQL_FOR_EACH_HPP )
