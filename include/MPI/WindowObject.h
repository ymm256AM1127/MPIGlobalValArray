#ifndef WINDOWOBJECT_H
#define WINDOWOBJECT_H

#include <mpi.h>
#include <vector>
#include "../MPL/is_pod.h"
#include <sstream>
#include <functional>
#include <exception>
#include "ErrorCode.h"
#include <iostream>
#include "Environment.h"
#include "DataTypes.h"
#include "WindowObjAllocator.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class T > struct Base
        {
            T data;
        };

        template < class T, class Allocator = WinAllocate<T> >
        class WindowObject : protected Base<T>
        {
        public:
            using value_type      = typename std::enable_if< MPL::is_pod_with_complex< T >::value, T >::type;
            using pointer         = value_type*;
            using const_pointer   = const value_type*;
            using reference       = value_type&;
            using const_reference = const value_type&;
            using CommPtr         = Environment::CommPtr;
            using allocator_type  = Allocator;
            using size_t          = std::size_t;

            WindowObject();
            explicit WindowObject( CommPtr comm,
                                   const size_t size,
                                   const size_t localhalosize = 0,
                                   const std::string&  windowobjectname = std::string() );

            WindowObject( const WindowObject& rhs );

            ~WindowObject();

            WindowObject&   operator=( const WindowObject& rhs );
            WindowObject&   operator=( const T& val );

            std::function< void() >     Deletor;

            //! Passive Target ======================================================================
            void            LockShared  ( const int rank ) const ;
            void            LockExclusive  ( const int rank ) const;
            void            Unlock( const int rank ) const;
            void            LockAll() const;
            void            UnlockAll() const;
            //! ====================================================================== Passive Target

            //! Acctive Target =-=====================================================================
            void            Fence  () const ;
            //! ======================================================================= Acctive Target

            //! Basic Operation =====================================================================
            value_type      Read ( const size_t index ) const;
            void            Write( const T& value, const size_t index );
            void            Read ( pointer baseptr,
                                   const size_t startindex,
                                   const size_t count );
            void            Write( const_pointer baseptr,
                                   const size_t startindex,
                                   const size_t count );
            const value_type at( const size_t index ) const;
            operator T () const;
            const WindowObject& operator[]( const size_t index ) const;
            WindowObject&   operator []( const size_t index );

            const value_type LocalAt( const size_t index ) const;

            //! ===================================================================== Basic Operation

            //! Getter Property =====================================================================
            pointer         GetBasePtr() const;
            size_t          GetGlobalSize() const;
            size_t          GetLocalSize() const;
            size_t          GetLocalHaloSize() const;
            size_t          GetDisplacementUnitSize() const;
            std::string     GetWindowObjName() const;
            CommPtr         GetCommPtr() const;
            //! ===================================================================== Getter Property

            //! Setter Property =====================================================================
            void            SetWindowObjName( const std::string& windowobjectname );
            //! ===================================================================== Setter Property


        private:
            MPI_Win                 m_WindowObj;
            allocator_type          m_Allocator;
            CommPtr                 m_Communicator;

            mutable value_type*     m_BasePtr;
            std::size_t             m_GlobalSize;
            std::size_t             m_LocalSize;

            std::size_t             m_LocalCapacity;
            std::size_t             m_LocalHaloSize;

            std::size_t             m_CurrentIndex;

            inline void     Put( const_pointer baseptr,
                                 const size_t offsetfrombaseptr,
                                 const size_t count,
                                 const int targetRank ) const;
            inline void     Get( pointer baseptr,
                                 const size_t offsetfrombaseptr,
                                 const size_t count,
                                 const int targetRank ) const;
            inline void     Acc( pointer baseptr,
                                 const size_t offsetfrombaseptr,
                                 const size_t count,
                                 const int targetRank,
                                 const MPI_Op Op ) const;

            //! アドレスを取得できないようにする。
            //!
            void            operator &() const{}

            void            CreateObject( CommPtr comm,
                                          const size_t size,
                                          const size_t localhalosize,
                                          const std::string&  windowobjectname = std::string() );

        };
    }
}

#endif // WINDOWOBJECT_H

#include "./detail/WindowObjectImpl.h"


