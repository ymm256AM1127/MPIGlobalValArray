#ifndef HAS_MEMBER_FUNCTION_H
#define HAS_MEMBER_FUNCTION_H

#define HAS_MEMBER_FUNCTION( targetFunction ) template< class T > class has_member_function_##targetFunction \
{ \
    template < class U, U > struct TypeCheck; \
    typedef char Yes;\
    typedef long No;\
    template < class U > struct helperFptr \
    { \
        typedef void (U::*fptr)(); \
    }; \
    template <typename U> static Yes hasfunction(TypeCheck< typename helperFptr<U>::fptr, &U:: targetFunction >*); \
    template <typename U> static No  hasfunction(...); \
public:\
    static bool const value = (sizeof(hasfunction<T>(0)) == sizeof(Yes)); \
   \
} \

#endif // HAS_MEMBER_FUNCTION_H
