#ifndef IS_STRING_H
#define IS_STRING_H

#include <string>
#include "remove_modifier.h"

namespace _MYNAMESPACE_
{
    namespace MPL
    {
        template< typename T >
        /*!
         * \brief The is_string_base struct
         */
        struct is_string_base
        {
           static const bool value = false;
        };

        template<>
        /*!
         * \brief The is_string_base<std::string> struct
         */
        struct is_string_base< std::string >
        {
            static const bool value = true;
        };

        template< typename T >
        /*!
         * \brief The is_string struct
         */
        struct is_string : public is_string_base< typename remove_modifier<T>::type >{};

        template< typename T >
        /*!
         * \brief The is_std_string struct
         */
        struct is_std_string
        {
           static const bool value = false;
        };

        template<>
        /*!
         * \brief The is_std_string<std::string> struct
         */
        struct is_std_string<std::string>
        {
           static const bool value = true;
        };
    }
}

#endif // IS_STRING_H
