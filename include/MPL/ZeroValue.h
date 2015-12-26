#ifndef ZEROVALUE_H
#define ZEROVALUE_H

#include <complex>
#include "../SharedLibraryDefine.h"

namespace _MYNAMESPACE_
{
    namespace MPL
    {
        template < class T >
        inline T ZeroType()
        {
            return 0;
        }

        #define MY_MPL_ZERO_VALUE( type, zeroval ) template<> \
        inline type ZeroType< type >() \
        { \
            return zeroval; \
        } \

        MY_MPL_ZERO_VALUE( char,                 0 )
        MY_MPL_ZERO_VALUE( short,                0 )
        MY_MPL_ZERO_VALUE( int,                  0 )
        MY_MPL_ZERO_VALUE( long,                 0L )
        MY_MPL_ZERO_VALUE( long long,            0L )

        MY_MPL_ZERO_VALUE( unsigned char,        0U )
        MY_MPL_ZERO_VALUE( unsigned short,       0U )
        MY_MPL_ZERO_VALUE( unsigned int,         0U )
        MY_MPL_ZERO_VALUE( unsigned long,        0UL )
        MY_MPL_ZERO_VALUE( unsigned long long,   0UL )

        MY_MPL_ZERO_VALUE( float,                0.0f )
        MY_MPL_ZERO_VALUE( double,               0.0 )

        template<>
        inline std::complex< float > ZeroType< std::complex< float > >()
        {
            return std::complex< float >( 0.0f, 0.0f );
        }

        template<>
        inline std::complex< double > ZeroType< std::complex< double > >()
        {
            return std::complex< double >( 0.0, 0.0 );
        }
    }
}

#endif // ZEROVALUE_H
