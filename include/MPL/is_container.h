#ifndef IS_CONTAINER_H
#define IS_CONTAINER_H

#include "../SharedLibraryDefine.h"
#include <type_traits>

//#include <vector>
//#include <list>
//#include <map>
//#include <set>
//#include <deque>
//#include <string>
//#include <map>

namespace _MYNAMESPACE_
{
    namespace MPL
    {
        #define HAS_COMMON_TYPE_DEFINE( datatype ) template< typename T > \
        class has_##datatype \
        { \
            typedef char yes; \
            typedef struct { char a[2]; } no; \
                                \
            template< class C > \
            static yes test( typename C::datatype * ); \
            \
            template< class C > \
            static no test( ... ); \
            \
        public: \
            static const bool value = sizeof( test< T >(0) ) == sizeof( yes );\
        }

        HAS_COMMON_TYPE_DEFINE( size_type );
        HAS_COMMON_TYPE_DEFINE( difference_type );
        HAS_COMMON_TYPE_DEFINE( value_type );
        HAS_COMMON_TYPE_DEFINE( reference );
        HAS_COMMON_TYPE_DEFINE( const_reference );
        HAS_COMMON_TYPE_DEFINE( iterator );
        HAS_COMMON_TYPE_DEFINE( const_iterator );

        template< typename T >
        struct is_std_compatible_container
        {
            static const bool value = has_size_type< T >::value         &&
                                      has_difference_type< T >::value   &&
                                      has_value_type< T >::value        &&
                                      has_iterator< T >::value          &&
                                      has_const_iterator< T >::value    ;
        };

        HAS_COMMON_TYPE_DEFINE( Iterator );
        HAS_COMMON_TYPE_DEFINE( ConstIterator );
//        template< typename T >
//        struct is_qt_compatible_container
//        {
//            static const bool value = is_std_compatible_container< T >::value &&
//                                      has_Iterator< T >::value                &&
//                                      has_ConstIterator< T >::value           ;
//        };

        HAS_COMMON_TYPE_DEFINE( key_type );
        HAS_COMMON_TYPE_DEFINE( mapped_type );

        // this meta type retuns the terget type which has the specified type or not.
        // if the target type has the specified type, then return type is as the specified type.
        // else if not the type, then return the original type.
        #define POSSIBLE_TYPE_DEFINE( datatype ) template< bool Cond, typename T = void > \
        struct possible_##datatype##_c{ };          \
        template< typename T >                      \
        struct possible_##datatype##_c< true, T >   \
        {                                           \
            typedef typename T::datatype type;    \
        };                                          \
        template< typename T >                      \
        struct possible_##datatype##_c< false, T >  \
        {                                           \
            typedef T type;                         \
        };                                          \
        template< typename T >                      \
        struct possible_##datatype :                \
        public possible_##datatype##_c< has_##datatype <T>::value, T >{}

        POSSIBLE_TYPE_DEFINE( value_type );
        POSSIBLE_TYPE_DEFINE( key_type );
        POSSIBLE_TYPE_DEFINE( mapped_type );
    }
}

#endif // IS_CONTAINER_H
