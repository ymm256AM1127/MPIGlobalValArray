#ifndef IS_COMPLEX_H
#define IS_COMPLEX_H

#include "../SharedLibraryDefine.h"
#include <type_traits>
#include <complex>
#include "remove_modifier.h"

namespace _MYNAMESPACE_
{
    namespace MPL
    {
        template< class T >
        struct is_complex_base
        {
            static const bool value = false;
        };

        template< class T>
        struct is_complex_base< typename std::complex<T> >
        {
            static const bool value = true;
        };

        template< class T >
        struct is_complex
        {
            static const bool value = is_complex_base< typename remove_modifier< T >::type >::value;
        };
    }
}


#endif // IS_COMPLEX_H

