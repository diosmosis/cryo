///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2010 dizzy
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( CRYO_DETAIL_FUSION_COPY_HPP )
#define CRYO_DETAIL_FUSION_COPY_HPP

#include <boost/fusion/include/next.hpp>
#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>

namespace cryo { namespace detail
{
    template <typename SourceI, typename SourceEnd, typename DestI>
    inline void fusion_copy(SourceI const& src_i, SourceEnd const& src_end, DestI const& dst_i)
    {
        *dst_i = *src_i;
        copy(boost::fusion::next(src_i), src_end, boost::fusion::next(dst_i));
    }

    template <typename SourceEnd, typename DestI>
    inline void fusion_copy(SourceEnd const&, SourceEnd const&, DestI const&)
    {
        // empty
    }

    template <typename Source, typename Dest>
    inline void fusion_copy(Source const& src, Dest & dst)
    {
        copy(boost::fusion::begin(src), boost::fusion::end(src), boost::fusion::begin(dst));
    }
}}

#endif // #if !defined( CRYO_DETAIL_FUSION_COPY_HPP )
