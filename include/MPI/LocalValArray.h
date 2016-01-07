#ifndef LOCALVALARRAY_H
#define LOCALVALARRAY_H

#include <sstream>
#include <vector>
#include "../MPL/is_pod.h"
#include "../MPL/ZeroValue.h"

std::vector<double> temp;

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class T >
        class LocalValArray
        {
        public:
            using value_type = typename std::enable_if< MPL::is_pod_with_complex< T >::value, T >::type;
            using pointer    = value_type*;
            using reference  = value_type&;
            using size_t     = std::size_t;

            explicit LocalValArray( pointer baseptr, const size_t localSize, const size_t HaloSize = 0 );
            LocalValArray( const LocalValArray& rhs );
            LocalValArray( LocalValArray&& rhs );

            ~LocalValArray();

            pointer             data();

            LocalValArray&      operator =( const LocalValArray& rhs )
            {
                if( m_LocalSize == rhs.m_LocalSize )
                {
                    memcpy( m_BasePtr, rhs.data(), sizeof( value_type ) * m_LocalSize );
                }

                return *this;
            }

            LocalValArray&      operator =( LocalValArray&& rhs )
            {
                if( this != rhs )
                {
                    m_BasePtr   = rhs.m_BasePtr;
                    m_LocalSize = rhs.size();
                    m_HaloSize  = rhs.halo_size();

                    rhs.m_BasePtr   = nullptr;
                    rhs.m_LocalSize = 0;
                    rhs.m_HaloSize  = 0;
                }

                return *this;
            }

            reference           operator []( const size_t index );
            const reference     operator []( const size_t index ) const;
            const value_type    at( const size_t index ) const;

            const size_t        size() const;
            const size_t        halo_size() const;

            const value_type    sum() const;

        private:
            pointer         m_BasePtr;
            size_t          m_LocalSize;
            size_t          m_HaloSize;
        };

        template < class T >
        LocalValArray<T>::LocalValArray(LocalValArray::pointer baseptr,
                                        const LocalValArray::size_t localSize,
                                        const LocalValArray::size_t HaloSize)
            : m_BasePtr( baseptr ), m_LocalSize( localSize ), m_HaloSize( HaloSize )
        {

        }

        template < class T >
        LocalValArray<T>::LocalValArray(const LocalValArray &rhs)
            : m_BasePtr( rhs.data() ), m_LocalSize( rhs.m_LocalSize ), m_HaloSize( rhs.m_HaloSize )
        {

        }

        template < class T >
        LocalValArray<T>::LocalValArray( LocalValArray &&rhs)
            : m_BasePtr( rhs.data() ), m_LocalSize( rhs.m_LocalSize ), m_HaloSize( rhs.m_HaloSize )
        {
            rhs.m_BasePtr   = nullptr;
            rhs.m_LocalSize = 0;
            rhs.m_HaloSize  = 0;
        }

        template < class T >
        LocalValArray<T>::~LocalValArray()
        {

        }

        template < class T >
        typename LocalValArray<T>::pointer LocalValArray<T>::data()
        {
            return m_BasePtr;
        }

        template < class T >
        typename LocalValArray<T>::reference LocalValArray<T>::operator [](const LocalValArray::size_t index)
        {
            return m_BasePtr[ index ];
        }

        template < class T >
        const typename LocalValArray<T>::reference LocalValArray<T>::operator [](const LocalValArray::size_t index) const
        {
            return m_BasePtr[ index ];
        }

        template < class T >
        const typename LocalValArray<T>::value_type LocalValArray<T>::at(const LocalValArray::size_t index) const
        {
            return m_BasePtr[ index ];
        }

        template < class T >
        const typename LocalValArray<T>::size_t LocalValArray<T>::size() const
        {
            return m_LocalSize;
        }

        template < class T >
        const typename LocalValArray<T>::size_t LocalValArray<T>::halo_size() const
        {
            return m_HaloSize;
        }

        template < class T >
        const typename LocalValArray<T>::value_type LocalValArray<T>::sum() const
        {
            using type = typename LocalValArray<T>::value_type;
            type val = MPL::ZeroType< type >();

            for( auto ii = 0UL; ii < m_LocalSize; ii++ )
            {
                val += m_BasePtr[ii];
            }
            return static_cast< const type >( val );
        }



    }
}

#endif // LOCALVALARRAY_H
