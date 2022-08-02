//
//  Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef NOWIDE_ENCODING_UTF_HPP_INCLUDED
#    define NOWIDE_ENCODING_UTF_HPP_INCLUDED

#    include <iterator>
#    include "encoding_errors.hpp"
#    include "utf.hpp"
#    include <string>
#    ifdef NOWIDE_MSVC
#        pragma warning(push)
#        pragma warning(disable : 4619 4275 4251 4231 4660)
#    endif

namespace nowide
{
    namespace conv
    {
        ///
        /// Convert a Unicode text in range [begin,end) to other Unicode encoding
        ///
        template<typename CharOut, typename CharIn>
        std::basic_string<CharOut> utf_to_utf(CharIn const* begin, CharIn const* end, method_type how = default_method)
        {
            std::basic_string<CharOut> result;
            result.reserve(end - begin);
            typedef std::back_insert_iterator<std::basic_string<CharOut>> inserter_type;
            inserter_type                                                 inserter(result);
            utf::code_point                                               c;
            while (begin != end) {
                c = utf::utf_traits<CharIn>::template decode<CharIn const*>(begin, end);
                if (c == utf::illegal || c == utf::incomplete) {
                    if (how == stop) throw conversion_error();
                }
                else {
                    utf::utf_traits<CharOut>::template encode<inserter_type>(c, inserter);
                }
            }
            return result;
        }

        ///
        /// Convert a Unicode NULL terminated string \a str other Unicode encoding
        ///
        template<typename CharOut, typename CharIn>
        std::basic_string<CharOut> utf_to_utf(CharIn const* str, method_type how = default_method)
        {
            CharIn const* end = str;
            while (*end) end++;
            return utf_to_utf<CharOut, CharIn>(str, end, how);
        }

        ///
        /// Convert a Unicode string \a str other Unicode encoding
        ///
        template<typename CharOut, typename CharIn>
        std::basic_string<CharOut> utf_to_utf(std::basic_string<CharIn> const& str, method_type how = default_method)
        {
            return utf_to_utf<CharOut, CharIn>(str.c_str(), str.c_str() + str.size(), how);
        }

    }    // namespace conv
}    // namespace nowide

#    ifdef NOWIDE_MSVC
#        pragma warning(pop)
#    endif

#endif

// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4