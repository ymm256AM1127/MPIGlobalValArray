#ifndef IS_POD_H
#define IS_POD_H

#include <type_traits>
#include <complex>
#include "remove_modifier.h"

namespace _MYNAMESPACE_
{
    namespace MPL
    {
        template< typename T >
        struct is_pod_base
        {
            static const bool value = false;
        };

        #define MY_MPL_IS_POD_BASE( type, flag ) template< > \
        struct is_pod_base< type > \
        { \
            static const bool value = flag; \
        } \

        /*!
         * \brief define Plain Old Data Types
         */
        // void type is not regarded as plain data type in this library.
        //MY_MPL_IS_POD_BASE( void,                 true );

        MY_MPL_IS_POD_BASE( bool,                 true );
        MY_MPL_IS_POD_BASE( char,                 true );
        MY_MPL_IS_POD_BASE( short,                true );
        MY_MPL_IS_POD_BASE( int,                  true );
        MY_MPL_IS_POD_BASE( long,                 true );
        MY_MPL_IS_POD_BASE( long long,            true );

        MY_MPL_IS_POD_BASE( unsigned char,        true );
        MY_MPL_IS_POD_BASE( unsigned short,       true );
        MY_MPL_IS_POD_BASE( unsigned int,         true );
        MY_MPL_IS_POD_BASE( unsigned long,        true );
        MY_MPL_IS_POD_BASE( unsigned long long,   true );

        MY_MPL_IS_POD_BASE( float,                true );
        MY_MPL_IS_POD_BASE( double,               true );

        template< typename T >
        /*!
         * \brief The is_pod struct
         */
        struct is_pod
        {
            static const bool value = is_pod_base< typename remove_modifier<T>::type >::value;
        };

        template< typename T >
        /*!
         * \brief The is_pod_base_with_complex struct
         */
        struct is_pod_base_with_complex : public is_pod_base< T >  {   };

        template< typename T >
        /*!
         * \brief The is_pod_base_with_complex<std::complex<T> > struct
         */
        struct is_pod_base_with_complex< typename std::complex< T > > : public is_pod_base< typename std::complex< T >::value_type > { };


        template< typename T >
        /*!
         * \brief The is_pod_with_complex struct
         */
        struct is_pod_with_complex
        {
            static const bool value = is_pod_base_with_complex< typename remove_modifier<T>::type >::value;
        };
    }
}



#endif // IS_POD_H
