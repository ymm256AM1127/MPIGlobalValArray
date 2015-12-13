#ifndef COMMUNICATORTRAITS_H
#define COMMUNICATORTRAITS_H

#include <list>
#include "../SharedLibraryDefine.h"
#include "../MPL/meta_function.h"


namespace _MYNAMESPACE_
{
    using namespace MPL;
    namespace  MPI
    {
        //! データタイプによるトレイツタグの定義
        struct no_traits_tag{ static const int id = 0; };
        struct pod_traits_tag{ static const int id = 1; };
        struct std_container_traits_tag{ static const int id = 2; };

        template< class T>
        struct comm_trais
        {
            typedef typename std::conditional< is_std_compatible_container< T >::value,
                                               std_container_traits_tag,
                                               typename std::conditional< is_pod_with_complex< T >::value, pod_traits_tag, no_traits_tag >::type >::type tag;
        };

        template < typename T > struct pod_list { typedef T list_type;  };
        template < typename T > struct std_list { typedef std::list<T> list_type;   };

        template<typename T>
        struct list_trais
        {
            typedef typename std::conditional< is_std_compatible_container< T >::value, std_list<T>, pod_list<T> >::list_type type;
        };

        template < typename T > struct is_key_type_or_srting
        {
            static const bool value = has_key_type<T>::value || is_string<T>::value;
        };

        template < typename T > struct is_reducible_type
        {
            static const bool value = is_pod_with_complex<T>::value ||
                                      ( ( !is_key_type_or_srting<T>::value ) &&
                                        is_pod_with_complex< typename possible_value_type<T>::type >::value );
        };

        template < typename T > struct reducible_type
        {
            typedef typename std::enable_if< is_reducible_type<T>::value, T >::type type;
        };
    }
}

#endif // COMMUNICATORTRAITS_H

