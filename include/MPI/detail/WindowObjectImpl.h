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
        template < class T, class Allocator >
        WindowObject<T, Allocator>::WindowObject()
            : m_WindowObj( MPI_WIN_NULL ),
              m_GlobalSize( 0 ),
              m_LocalHaloSize( 0 ),
              m_CurrentIndex( 0 )
        {

        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::WindowObject
         * \param size
         * \param comm
         */
        WindowObject<T, Allocator>::WindowObject( CommPtr comm,
                                                  const std::size_t size,
                                                  const std::size_t localhalosize,
                                                  const std::string&  windowobjectname )
            : m_WindowObj( MPI_WIN_NULL ),
              m_Communicator( comm ),
              m_GlobalSize( size ),
              m_LocalHaloSize( localhalosize ),
              m_CurrentIndex( 0 )
        {
            CreateObject( comm, size, localhalosize, windowobjectname );
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject::WindowObject
         * \param rhs
         */
        WindowObject<T, Allocator>::WindowObject(const WindowObject &rhs)
            : m_WindowObj( MPI_WIN_NULL ),
              m_GlobalSize( 0 ),
              m_LocalHaloSize( 0 ),
              m_CurrentIndex( 0 )
        {
            *this = rhs;
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::~WindowObject
         */
        WindowObject<T, Allocator>::~WindowObject()
        {
            m_Communicator->EraseWinObjSet( &Deletor );
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::CreateObject
         * \param comm
         * \param size
         * \param localhalosize
         * \param windowobjectname
         */
        void WindowObject<T, Allocator>::CreateObject( CommPtr comm,
                                                       const std::size_t size,
                                                       const std::size_t localhalosize,
                                                       const std::string &windowobjectname)
        {
            //! ここにアロケータを書く
            m_Allocator.CreateWindowObj( m_BasePtr,
                                         m_WindowObj,
                                         comm,
                                         size,
                                         localhalosize,
                                         windowobjectname );

            //! Window削除用ラムダを記述（thisをキャプチャさせておく）
            Deletor = [this]()
            {
                if( m_WindowObj != MPI_WIN_NULL )
                {
                    m_Allocator.DeleteWindowObj( m_WindowObj );
                }
            };

            m_Communicator->RegisterWinObjSet( &Deletor );

            MPI_Aint* win_size = nullptr;
            int  flag = 0;
            MPI_Win_get_attr( m_WindowObj, MPI_WIN_SIZE, &win_size, &flag );
            m_LocalCapacity = static_cast< std::size_t >(*win_size) / sizeof( value_type );

            m_LocalSize     = m_LocalCapacity - m_LocalHaloSize;
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::operator =
         * \param val
         * \return
         */
        WindowObject<T, Allocator> &WindowObject<T, Allocator>::operator=(const typename WindowObject<T, Allocator>::value_type &val)
        {
            Write( val, m_CurrentIndex );
            return *this;
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::operator =
         * \param rhs
         * \return
         */
        WindowObject<T, Allocator> &WindowObject<T, Allocator>::operator=(const WindowObject<T, Allocator> &rhs)
        {
            if( m_WindowObj != MPI_WIN_NULL )
            {
                m_Communicator->EraseWinObjSet( &Deletor );
            }
            m_Communicator          = rhs.m_Communicator;
            m_GlobalSize            = rhs.m_GlobalSize;
            m_LocalHaloSize         = rhs.m_LocalHaloSize;
            m_CurrentIndex          = 0;

            CreateObject( m_Communicator, m_GlobalSize, m_LocalHaloSize );

            //! このオペレータが呼ばれるときは集団通信が前提なので、ローカルでコピーするだけで良い。
            memcpy( m_BasePtr, rhs.m_BasePtr, ( m_LocalSize + m_LocalHaloSize ) * sizeof( value_type )  );

            return *this;
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::GetBasePtr
         * \return
         */
        typename WindowObject<T, Allocator>::pointer WindowObject<T, Allocator>::GetBasePtr() const
        {
            return m_BasePtr;
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::GetGlobalSize
         * \return
         */
        std::size_t WindowObject<T, Allocator>::GetGlobalSize() const
        {
            return m_GlobalSize;
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::GetLocalSize
         * \return
         */
        std::size_t WindowObject<T, Allocator>::GetLocalSize() const
        {
            return m_LocalSize;
        }


        template < class T, class Allocator >
        //!
        //! \brief WindowObject::GetLocalHaloSize
        //! MPIではHALOと呼ばれるオーバーラップされる領域を指定するが、
        //! その大きさをByteサイズで取得できる。
        //! \return
        //!
        std::size_t WindowObject<T,Allocator>::GetLocalHaloSize() const
        {
            return m_LocalHaloSize;
        }

        template < class T, class Allocator >
        //!
        //! \brief WindowObject::GetDisplacementUnitSize
        //! 変位単位サイズを取得する。
        //! \return
        //!
        std::size_t WindowObject<T, Allocator>::GetDisplacementUnitSize() const
        {
            int* dispunit = nullptr;
            int  flag = 0;
            MPI_Win_get_attr( m_WindowObj, MPI_WIN_DISP_UNIT, &dispunit, &flag );
            return static_cast< std::size_t >(*dispunit);
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject::SetWindowObjName
         * \param windowobjectname
         */
        void WindowObject<T, Allocator>::SetWindowObjName(const std::string &windowobjectname)
        {
            MPI_Win_set_name( m_WindowObj, windowobjectname.c_str() );
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject::GetWindowObjName
         * \return
         */
        std::string WindowObject<T, Allocator>::GetWindowObjName() const
        {
            char name[MPI_MAX_OBJECT_NAME];
            int length = 0;
            MPI_Win_get_name( m_WindowObj, name, &length );
            std::string temp( name );
            temp.shrink_to_fit();
            return temp;
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::LockShared( const int rank ) const
        {
            MPI_Win_lock( MPI_LOCK_SHARED, rank, MPI_WIN_DEFAULT_ASSERTION , m_WindowObj );
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::LockExclusive( const int rank ) const
        {
            MPI_Win_lock( MPI_LOCK_EXCLUSIVE, rank, MPI_WIN_DEFAULT_ASSERTION , m_WindowObj );
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::Unlock(const int rank) const
        {
            MPI_Win_unlock( rank, m_WindowObj );
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::LockAll( ) const
        {
            MPI_Win_lock_all( MPI_WIN_DEFAULT_ASSERTION , m_WindowObj );
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::UnlockAll( ) const
        {
            MPI_Win_unlock_all(  m_WindowObj );
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::Put( const value_type* baseptr,
                                              const std::size_t offsetfrombasept,
                                              const std::size_t count,
                                              const int targetRank ) const
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

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::Get( value_type* baseptr,
                                              const std::size_t offsetfrombasept,
                                              const std::size_t count,
                                              const int targetRank ) const
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

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::Acc( value_type *baseptr,
                                              const std::size_t offsetfrombaseptr,
                                              const std::size_t count,
                                              const int targetRank,
                                              const MPI_Op Op ) const
        {
            this->LockShared( targetRank );
            MPI_Accumulate( baseptr,
                            count,
                            MPIDATATYPE<T>(),
                            targetRank,
                            offsetfrombaseptr,
                            count,
                            MPIDATATYPE<T>(),
                            Op,
                            m_WindowObj );
            this->Unlock( targetRank );
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::Fence() const
        {
            MPI_Win_fence( MPI_WIN_DEFAULT_ASSERTION, m_WindowObj );
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::Read
         * \param index
         * \return
         */
        typename WindowObject<T, Allocator>::value_type WindowObject<T, Allocator>::Read(const std::size_t index) const
        {
            std::size_t iTargetRank = index / m_LocalSize;
            std::size_t iOffset     = index % m_LocalSize ;

            if( iTargetRank != static_cast<std::size_t>( m_Communicator->GetMPIRank() ) )
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

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::operator T
         */
        WindowObject<T, Allocator>::operator T() const
        {
            return Read( m_CurrentIndex );
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::at
         * \param index
         * \return
         */
        const typename WindowObject<T, Allocator>::value_type WindowObject<T, Allocator>::at(const std::size_t index) const
        {
            if( index >= m_GlobalSize )
            {
                throw std::out_of_range("Window object out of range.");
            }
            return Read( index );
        }

        template < class T, class Allocator >
        const WindowObject<T, Allocator> &WindowObject<T, Allocator>::operator[](const std::size_t index) const
        {
            const_cast< std::size_t& >( m_CurrentIndex ) = index;
            return *this;
        }

        template < class T, class Allocator >
        WindowObject<T, Allocator> &WindowObject<T, Allocator>::operator [](const std::size_t index)
        {
            m_CurrentIndex = index;
            return *this;
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::LocalAt
         * \param index
         * \return
         */
        const typename WindowObject<T, Allocator>::value_type WindowObject<T, Allocator>::LocalAt(const std::size_t index) const
        {
            if( index >= m_LocalCapacity )
            {
                throw std::out_of_range("Window object out of range.");
            }
            return m_BasePtr[ index ];
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::Write(const WindowObject<T, Allocator>::value_type &value, const std::size_t index)
        {
            std::size_t iTargetRank = index / m_LocalSize;
            std::size_t iOffset     = index % m_LocalSize;

            if( iTargetRank != static_cast<std::size_t>( m_Communicator->GetMPIRank() ) )
            {
                this->Put( &value, iOffset, 1, iTargetRank );
            }
            else
            {
                m_BasePtr[ iOffset ] = value;
            }
        }

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::Read
         * \param baseptr
         * \param count
         */
        void WindowObject<T, Allocator>::Read(T *baseptr, const std::size_t startindex , const std::size_t count )
        {
            std::size_t iTargetRank = startindex / m_LocalSize;
            std::size_t iOffset     = startindex % m_LocalSize;
            if( ( m_LocalSize - iOffset ) >= count )
            {
                if( m_Communicator->GetMPIRank() == iTargetRank )
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
                if( m_Communicator->GetMPIRank() == iTargetRank )
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

        template < class T, class Allocator >
        /*!
         * \brief WindowObject<T, Allocator>::Read
         * \param baseptr
         * \param count
         */
        void WindowObject<T, Allocator>::Write(const T *baseptr, const std::size_t startindex , const std::size_t count )
        {
            std::size_t iTargetRank = startindex / m_LocalSize;
            std::size_t iOffset     = startindex % m_LocalSize;
            if( ( m_LocalSize - iOffset ) >= count )
            {
                if( m_Communicator->GetMPIRank() == iTargetRank )
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
                if( m_Communicator->GetMPIRank() == iTargetRank )
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
