#ifndef WINDOWOBJECTIMPL_H
#define WINDOWOBJECTIMPL_H

#include "../WindowObject.h"
#include "../DataTypeDefine.h"
#include <iostream>
#include <cstring>

static const int MPI_WIN_DEFAULT_ASSERTION = 0;
static const MPI_Aint MPI_WIN_DEFAULT_DISPLACEMENT = 0;

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class T >
        /*!
         * \brief WindowObject<T>::WindowObject
         * \param size
         * \param comm
         */
        WindowObject<T>::WindowObject( const std::size_t size, const MPI_Comm comm)
            : m_Communicator( comm ), m_HaloSpaceSize( 0 ), m_Offset( 0 )
        {
            int i32rank = -1;
            int i32size = -1;
            MPI_Comm_rank( m_Communicator, &i32rank );
            MPI_Comm_size( m_Communicator, &i32size );

            m_MPIRank = static_cast< std::size_t >( i32rank );
            m_MPISize = static_cast< std::size_t >( i32size );

            Create( size, m_Communicator );
        }

        template < class T >
        /*!
         * \brief WindowObject<T>::~WindowObject
         */
        WindowObject<T>::~WindowObject()
        {

        }

        template < class T >
        /*!
         * \brief WindowObject<T>::Create
         * \param size
         * \param comm
         */
        void WindowObject<T>::Create( const std::size_t size, const MPI_Comm comm )
        {
            m_GlobalSize   = size;
            m_Communicator = comm;
            if( m_GlobalSize == 0 )
            {
                std::stringstream ss;
                ss << "Error Code: " << CANNOTCREATEWINDOW
                   << " on file: " << __FILE__
                   << ", Line: " << __LINE__
                   << ", Rank: " << m_MPIRank << std::endl;
                throw std::range_error( ss.str() );
                return;
            }

            //! あまりを計算
            std::size_t irest = ( (m_GlobalSize % m_MPISize) == 0 ) ? 0
                                                                    : m_MPISize - ( m_GlobalSize % m_MPISize ) ;
            m_GlobalSize      = m_GlobalSize + irest;
            m_LocalSize       = m_GlobalSize / m_MPISize;

//            std::cout << m_MPIRank << "," << m_MPISize << "," << m_GlobalSize << "," << m_LocalSize << std::endl;

            MPI_Win_allocate( m_GlobalSize * sizeof( T ), sizeof( T ),
                              MPI_INFO_NULL, m_Communicator, &m_BasePtr, &m_WindowObj );
        }

        template < class T >
        /*!
         * \brief WindowObject<T>::GetBasePtr
         * \return
         */
        T *WindowObject<T>::GetBasePtr() const
        {
            return m_BasePtr;
        }

        template < class T >
        /*!
         * \brief WindowObject<T>::GetGlobalSize
         * \return
         */
        std::size_t WindowObject<T>::GetGlobalSize() const
        {
            return m_GlobalSize;
        }

        template < class T >
        /*!
         * \brief WindowObject<T>::GetLocalSize
         * \return
         */
        std::size_t WindowObject<T>::GetLocalSize() const
        {
            return m_LocalSize;
        }

        template < class T >
        /*!
         * \brief WindowObject<T>::Fence
         */
        void WindowObject<T>::Fence()
        {
            MPI_Win_fence( MPI_WIN_DEFAULT_ASSERTION, m_Communicator );
        }

        template < class T >
        void WindowObject<T>::LockShared( const int rank )
        {
            MPI_Win_lock( MPI_LOCK_SHARED, rank, MPI_WIN_DEFAULT_ASSERTION , m_WindowObj );
        }

        template < class T >
        void WindowObject<T>::LockExclusive( const int rank )
        {
            MPI_Win_lock( MPI_LOCK_EXCLUSIVE, rank, MPI_WIN_DEFAULT_ASSERTION , m_WindowObj );
        }

        template < class T >
        void WindowObject<T>::Unlock(const int rank)
        {
            MPI_Win_unlock( rank, m_WindowObj );
        }

        template < class T >
        void WindowObject<T>::Put( const T* baseptr,
                                const std::size_t offsetfrombasept,
                                const std::size_t count,
                                const int targetRank )
        {
            this->LockExclusive( targetRank );
            MPI_Put( baseptr,
                     count,
                     MPIDATATYPE<T>(),
                     targetRank,
                     offsetfrombasept,
                     count,
                     MPIDATATYPE<T>(),
                     m_WindowObj );
            this->Unlock( targetRank );
        }

        template < class T >
        void WindowObject<T>::Get( T* baseptr,
                                const std::size_t offsetfrombasept,
                                const std::size_t count,
                                const int targetRank )
        {
            this->LockShared( targetRank );
            MPI_Get( baseptr,
                     count,
                     MPIDATATYPE<T>(),
                     targetRank,
                     offsetfrombasept,
                     count,
                     MPIDATATYPE<T>(),
                     m_WindowObj );
            this->Unlock( targetRank );
        }

        template < class T >
        T WindowObject<T>::Read(const std::size_t index)
        {
            std::size_t iTargetRank = index / m_LocalSize;
            std::size_t iOffset     = index % m_LocalSize ;

            if( iTargetRank != m_MPIRank )
            {
                auto val = MPL::ZeroType<T>();
                this->Get( &val, iOffset, 1, iTargetRank );
                return val;
            }
            else
            {
                return m_BasePtr[ iOffset ];
            }
        }

        template < class T >
        void WindowObject<T>::Write(const T &value, const std::size_t index)
        {
            std::size_t iTargetRank = index / m_LocalSize;
            std::size_t iOffset     = index % m_LocalSize;

            if( iTargetRank != m_MPIRank )
            {
                this->Put( &value, iOffset, 1, iTargetRank );
            }
            else
            {
                m_BasePtr[ iOffset ] = value;
            }
        }

        template < class T >
        /*!
         * \brief WindowObject<T>::Read
         * \param baseptr
         * \param count
         */
        void WindowObject<T>::Read(T *baseptr, const std::size_t startindex , const std::size_t count )
        {
            std::size_t iTargetRank = startindex / m_LocalSize;
            std::size_t iOffset     = startindex % m_LocalSize;
            if( ( m_LocalSize - iOffset ) >= count )
            {
                if( m_MPIRank == iTargetRank )
                {
                    std::memcpy( m_BasePtr, (void*)baseptr, sizeof( T ) * count );
                }
                else
                {
                    this->Get( baseptr, iOffset, count, iTargetRank );
                }
            }
            else
            {
                if( m_MPIRank == iTargetRank )
                {
                    std::memcpy( m_BasePtr, (void*)baseptr, sizeof( T ) * m_LocalSize );
                }
                else
                {
                    this->Get( baseptr, iOffset, m_LocalSize, iTargetRank );
                }
                //! 再帰で処理
                this->Read( baseptr + m_LocalSize, m_LocalSize, count - m_LocalSize );
            }
        }

        template < class T >
        /*!
         * \brief WindowObject<T>::Read
         * \param baseptr
         * \param count
         */
        void WindowObject<T>::Write(const T *baseptr, const std::size_t startindex , const std::size_t count )
        {
            std::size_t iTargetRank = startindex / m_LocalSize;
            std::size_t iOffset     = startindex % m_LocalSize;
            if( ( m_LocalSize - iOffset ) >= count )
            {
                if( m_MPIRank == iTargetRank )
                {
                    std::memcpy( (void*)baseptr, m_BasePtr, sizeof( T ) * count );
                }
                else
                {
                    this->Put( baseptr, iOffset, count, iTargetRank );
                }
            }
            else
            {
                if( m_MPIRank == iTargetRank )
                {
                    std::memcpy( (void*)baseptr, m_BasePtr, sizeof( T ) * count );
                }
                else
                {
                    this->Put( baseptr, iOffset, m_LocalSize, iTargetRank );
                }
                //! 再帰で処理
                this->Write( baseptr + m_LocalSize, m_LocalSize, count - m_LocalSize );
            }
        }

    }

}

#endif // WINDOWOBJECTIMPL_H
