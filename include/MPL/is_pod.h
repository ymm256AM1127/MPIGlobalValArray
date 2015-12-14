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

        #define ENSEMBLE_IS_POD_BASE( type, flag ) template< > \
        struct is_pod_base< type > \
        { \
            static const bool value = flag; \
        } \

        /*!
         * \brief define Plain Old Data Types
         */
        // void type is not regarded as plain data type in this library.
        //ENSEMBLE_IS_POD_BASE( void,                 true );

        ENSEMBLE_IS_POD_BASE( bool,                 true );
        ENSEMBLE_IS_POD_BASE( char,                 true );
        ENSEMBLE_IS_POD_BASE( short,                true );
        ENSEMBLE_IS_POD_BASE( int,                  true );
        ENSEMBLE_IS_POD_BASE( long,                 true );
        ENSEMBLE_IS_POD_BASE( long long,            true );

        ENSEMBLE_IS_POD_BASE( unsigned char,        true );
        ENSEMBLE_IS_POD_BASE( unsigned short,       true );
        ENSEMBLE_IS_POD_BASE( unsigned int,         true );
        ENSEMBLE_IS_POD_BASE( unsigned long,        true );
        ENSEMBLE_IS_POD_BASE( unsigned long long,   true );

        ENSEMBLE_IS_POD_BASE( float,                true );
        ENSEMBLE_IS_POD_BASE( double,               true );

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
