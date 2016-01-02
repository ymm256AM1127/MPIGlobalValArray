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
              m_LocalHaloSize( localhalosize )
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
            m_LocalSize =  static_cast< std::size_t >(*win_size);
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
         * \brief WindowObject<T, Allocator>::GetBasePtr
         * \return
         */
        T *WindowObject<T, Allocator>::GetBasePtr() const
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
        void WindowObject<T, Allocator>::LockShared( const int rank )
        {
            MPI_Win_lock( MPI_LOCK_SHARED, rank, MPI_WIN_DEFAULT_ASSERTION , m_WindowObj );
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::LockExclusive( const int rank )
        {
            MPI_Win_lock( MPI_LOCK_EXCLUSIVE, rank, MPI_WIN_DEFAULT_ASSERTION , m_WindowObj );
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::Unlock(const int rank)
        {
            MPI_Win_unlock( rank, m_WindowObj );
        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::Put( const value_type* baseptr,
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

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::Get( value_type* baseptr,
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

        template < class T, class Allocator >
        T WindowObject<T, Allocator>::Read(const std::size_t index)
        {
            std::size_t iTargetRank = index / m_LocalSize;
            std::size_t iOffset     = index % m_LocalSize ;

            if( iTargetRank != m_Communicator->GetMPIRank() )
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

//        template < class T, class Allocator >
//        /*!
//         * \brief WindowObject<T, Allocator>::operator []
//         * \param index
//         * \return
//         */
//        T WindowObject<T, Allocator>::operator[](const std::size_t index) const
//        {
//            return this->Read( index );
//        }

        template < class T, class Allocator >
        void WindowObject<T, Allocator>::Write(const T &value, const std::size_t index)
        {
            std::size_t iTargetRank = index / m_LocalSize;
            std::size_t iOffset     = index % m_LocalSize;

            if( iTargetRank != m_Communicator->GetMPIRank() )
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
