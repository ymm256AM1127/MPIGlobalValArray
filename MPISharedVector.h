#ifndef MPISHAREDVECTOR_H
#define MPISHAREDVECTOR_H

#include <vector>
#include "MPISharedAllocator.h"

template < class T >
class MPISharedVector : private std::vector<T, MPISharedAllocator<T> >
{
public:
    using STLVector = std::vector<T, MPISharedAllocator<T> >;
    explicit MPISharedVector( const typename STLVector::allocator_type& alloc = typename STLVector::allocator_type()) : STLVector( alloc ){}
    explicit MPISharedVector( typename STLVector::size_type n) :  STLVector( n ){}

    MPISharedVector( typename STLVector::size_type n,
                     const typename STLVector::value_type& val,
                     const typename STLVector::allocator_type& alloc = typename STLVector::allocator_type())
            : STLVector( n, val, alloc ){}

    template <class InputIterator>
    MPISharedVector( InputIterator first, InputIterator last,
                       const typename STLVector::allocator_type& alloc = typename STLVector::allocator_type() )
            : STLVector( first, last, alloc ){}

    MPISharedVector( const MPISharedVector& x ) : STLVector( x ){}
    MPISharedVector( const MPISharedVector& x, const typename STLVector::allocator_type& alloc ) : STLVector( x, alloc ){}
    MPISharedVector( MPISharedVector&& x ) : STLVector( x ){}
    MPISharedVector( MPISharedVector&& x, const typename STLVector::allocator_type& alloc ) : STLVector( x ){}
    MPISharedVector( std::initializer_list<typename STLVector::value_type> il, const typename STLVector::allocator_type& alloc = typename STLVector::allocator_type() ) : STLVector( il, alloc ){}

    using STLVector::operator=    ;
    using STLVector::begin        ;
    using STLVector::end          ;
    using STLVector::rbegin       ;
    using STLVector::rend         ;
    using STLVector::cbegin       ;
    using STLVector::cend         ;
    using STLVector::crbegin      ;
    using STLVector::crend        ;
    using STLVector::size         ;
    using STLVector::max_size     ;
    using STLVector::resize       ;
    using STLVector::capacity     ;
    using STLVector::empty        ;
    using STLVector::reserve      ;
    using STLVector::shrink_to_fit;
    using STLVector::operator[]   ;
    using STLVector::at           ;
    using STLVector::front        ;
    using STLVector::back         ;
    using STLVector::data         ;
    using STLVector::assign       ;
    using STLVector::push_back    ;
    using STLVector::pop_back     ;
    using STLVector::insert       ;
    using STLVector::erase        ;
    using STLVector::swap         ;
    using STLVector::clear        ;
    using STLVector::emplace      ;
    using STLVector::emplace_back ;
};

#endif // MPISHAREDVECTOR_H

