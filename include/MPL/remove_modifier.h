#ifndef REMOVE_MODIFIER_H
#define REMOVE_MODIFIER_H

#include "../SharedLibraryDefine.h"
#include <type_traits>

namespace _MYNAMESPACE_
{
    namespace MPL
    {
        /*!
         * \brief The remove_modifier struct
         */
        template< class T >
        struct remove_modifier
        {
            typedef typename std::remove_cv< typename std::remove_reference< typename std::remove_pointer<T>::type >::type >::type type;
        };
    }
}

#endif // REMOVE_MODIFIER_H

