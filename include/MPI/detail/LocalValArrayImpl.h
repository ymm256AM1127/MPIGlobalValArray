#ifndef LOCALVALARRAYIMPL_H
#define LOCALVALARRAYIMPL_H

#include "../LocalValArray.h"
#include "../../Utility/MathHelper.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        using namespace Utility;
        template < typename T >
        ForwardAccessIterator<T> LocalValArray<T>::begin()
        {
            return ForwardAccessIterator<T>( m_BasePtr, 0, m_LocalSize );
        }

        template < typename T >
        ForwardAccessIterator<T> LocalValArray<T>::end()
        {
            return ForwardAccessIterator<T>( );
        }

        template < class T >
        LocalValArray<T>::LocalValArray(typename LocalValArray<T>::pointer baseptr,
                                        const typename LocalValArray<T>::size_t localSize,
                                        const typename LocalValArray<T>::size_t HaloSize)
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
        typename LocalValArray<T>::reference LocalValArray<T>::operator [](const typename LocalValArray<T>::size_t index)
        {
            return m_BasePtr[ index ];
        }

        template < class T >
        typename LocalValArray<T>::const_reference LocalValArray<T>::operator [](const typename LocalValArray<T>::size_t index) const
        {
            return m_BasePtr[ index ];
        }

        template < class T >
        const typename LocalValArray<T>::value_type LocalValArray<T>::at(const typename LocalValArray<T>::size_t index) const
        {
            return m_BasePtr[ index ];
        }

        template < class T >
        typename LocalValArray<T>::size_t LocalValArray<T>::size() const
        {
            return m_LocalSize;
        }

        template < class T >
        typename LocalValArray<T>::size_t LocalValArray<T>::halo_size() const
        {
            return m_HaloSize;
        }

        template < class T >
        const typename LocalValArray<T>::value_type LocalValArray<T>::norm() const
        {
            using type = typename LocalValArray<T>::value_type;
            type val = MPL::ZeroType< type >();

            for( auto ii = 0UL; ii < m_LocalSize; ii++ )
            {
                val += std::abs( m_BasePtr[ii] );
            }
            return static_cast< const type >( val );
        }

        template < class T >
        const typename LocalValArray<T>::value_type LocalValArray<T>::norm2() const
        {
            using type = typename LocalValArray<T>::value_type;
            type val = MPL::ZeroType< type >();

            for( auto ii = 0UL; ii < m_LocalSize; ii++ )
            {
                val += ( m_BasePtr[ii] * m_BasePtr[ii] );
            }
            return static_cast< const type >( val );
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

        template < class T >
        const typename LocalValArray<T>::value_type LocalValArray<T>::min() const
        {
            using type = typename LocalValArray<T>::value_type;
            type val = m_BasePtr[0];

            for( auto ii = 1UL; ii < m_LocalSize; ii++ )
            {
                if( val > m_BasePtr[ii] )
                {
                    val = m_BasePtr[ii];
                }
            }
            return static_cast< const type >( val );
        }

        template < class T >
        const typename LocalValArray<T>::value_type LocalValArray<T>::max() const
        {
            using type = typename LocalValArray<T>::value_type;
            type val = m_BasePtr[0];

            for( auto ii = 0UL; ii < m_LocalSize; ii++ )
            {
                if( val < m_BasePtr[ii] )
                {
                    val = m_BasePtr[ii];
                }
            }
            return static_cast< const type >( val );
        }

        template < class T >
        LocalValArray<T>&  LocalValArray<T>::operator +=(const LocalValArray<T> &rhs)
        {
            for( auto ii = 0UL; ii < size(); ii++ )
            {
                (*this)[ii] += rhs[ii];
            }
            return *this;
        }

        template < class T >
        LocalValArray<T>&  LocalValArray<T>::operator -=(const LocalValArray<T> &rhs)
        {
            for( auto ii = 0UL; ii < size(); ii++ )
            {
                (*this)[ii] -= rhs[ii];
            }
            return *this;
        }

        template < class T >
        LocalValArray<T>&  LocalValArray<T>::operator *=(const LocalValArray<T> &rhs)
        {
            for( auto ii = 0UL; ii < size(); ii++ )
            {
                (*this)[ii] *= rhs[ii];
            }
            return *this;
        }

        template < class T >
        LocalValArray<T>& LocalValArray<T>::operator /=(const LocalValArray<T> &rhs)
        {
            for( auto ii = 0UL; ii < size(); ii++ )
            {
                (*this)[ii] /= rhs[ii];
            }
            return *this;
        }

        template < class T >
        template< class expression >
        LocalValArray<T>& LocalValArray<T>::operator=( const expression& rhs )
        {
            for( auto ii = 0UL; ii < this->GetLocalSize(); ii++ )
            {
                (*this)[ii] = rhs[ii];
            }
            return *this;
        }

        template < class T >
        const typename LocalValArray<T>::value_type LocalValArray<T>::innter_product( const LocalValArray<T>& rhs ) const
        {
            using type = typename LocalValArray<T>::value_type;
            type val = MPL::ZeroType< type >();

            const auto lhsptr = this->data();
            const auto rhsptr = rhs.data();

            for( auto ii = 0UL; ii < m_LocalSize; ii++ )
            {
                val += lhsptr[ii] * rhsptr[ii];
            }
            return static_cast< const type >( val );
        }

        template < class T >
        typename LocalValArray<T>::const_pointer LocalValArray<T>::data() const
        {
            return static_cast< LocalValArray<T>::const_pointer >(m_BasePtr);
        }


    }
}

#endif // LOCALVALARRAYIMPL_H
