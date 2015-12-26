#ifndef WINDOWOBJECT_H
#define WINDOWOBJECT_H

#include <mpi.h>
#include <vector>
#include "../MPL/is_pod.h"
#include <sstream>
#include <exception>
#include "ErrorCode.h"
#include <iostream>

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class T >
        class WindowObject
        {
        public:
            using value_type = typename std::enable_if< MPL::is_pod< T >::value, T >::type;

            WindowObject( const std::size_t size, const MPI_Comm comm );
            ~WindowObject();

            void            Fence ();
            void            LockShared  ( const int rank );
            void            LockExclusive  ( const int rank );
            void            Unlock( const int rank );

            T               Read( const std::size_t index );
            void            Write( const T& value, const std::size_t index );
            void            Read( T* baseptr, const std::size_t startindex, const std::size_t count );
            void            Write( const T* baseptr, const std::size_t startindex, const std::size_t count );


            T*              GetBasePtr() const;
            std::size_t     GetGlobalSize() const;
            std::size_t     GetLocalSize() const;

//            void            LockAll();
//            void            UnlockAll();
//            void            Flush( int rank );
//            void            FlushAll();
//            void            Sync();

        private:
            MPI_Win         m_WindowObj;
            MPI_Comm        m_Communicator;
            mutable T*      m_BasePtr;
            std::size_t     m_GlobalSize;
            std::size_t     m_LocalSize;
            std::size_t     m_MPIRank;
            std::size_t     m_MPISize;
            std::size_t     m_LocalCapacity;

            std::size_t     m_HaloSpaceSize;
            std::size_t     m_Offset;

            inline void     Create( const std::size_t size, const MPI_Comm comm );

            inline void     Put( const T* baseptr,
                                 const std::size_t offsetfrombaseptr,
                                 const std::size_t count,
                                 const int targetRank );
            inline void     Get( T* baseptr,
                                 const std::size_t offsetfrombaseptr,
                                 const std::size_t count,
                                 const int targetRank );
        };
    }
}

#include "./detail/WindowObjectImpl.h"

#endif // WINDOWOBJECT_H

