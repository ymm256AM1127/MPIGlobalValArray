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
            using value_type     = typename std::enable_if< MPL::is_pod< T >::value, T >::type;
            using pointer        = value_type*;
            using CommPtr        = Environment::CommPtr;
            using allocator_type = Allocator;

            explicit WindowObject( CommPtr comm,
                                   const std::size_t size,
                                   const std::size_t localhalosize = 0,
                                   const std::string&  windowobjectname = std::string() );

            WindowObject( const WindowObject& rhs );

            ~WindowObject();

            WindowObject& operator=( const WindowObject& rhs );
            WindowObject& operator=( const value_type& val );

            std::function< void() >     Deletor;

            //! Passive Target ======================================================================
            void            LockShared  ( const int rank ) const ;
            void            LockExclusive  ( const int rank ) const;
            void            Unlock( const int rank ) const;
            //! ====================================================================== Passive Target

            //! Basic Operation =====================================================================
            value_type      Read ( const std::size_t index ) const;
            void            Write( const value_type& value, const std::size_t index );
            void            Read ( T* baseptr,
                                   const std::size_t startindex,
                                   const std::size_t count );
            void            Write( const T* baseptr,
                                   const std::size_t startindex,
                                   const std::size_t count );
            const value_type at( const std::size_t index ) const;
            operator T () const;
            const WindowObject& operator[]( const std::size_t index ) const;
            WindowObject& operator []( const std::size_t index );
            //! ===================================================================== Basic Operation

            //! Getter Property =====================================================================
            pointer         GetBasePtr() const;
            std::size_t     GetGlobalSize() const;
            std::size_t     GetLocalSize() const;
            std::size_t     GetLocalHaloSize() const;
            std::size_t     GetDisplacementUnitSize() const;
            std::string     GetWindowObjName() const;
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

            inline void     Put( const value_type* baseptr,
                                 const std::size_t offsetfrombaseptr,
                                 const std::size_t count,
                                 const int targetRank );
            inline void     Get( value_type* baseptr,
                                 const std::size_t offsetfrombaseptr,
                                 const std::size_t count,
                                 const int targetRank ) const;

            //! アドレスを取得できないようにする。
            void operator &() const{}

            void CreateObject( CommPtr comm,
                               const std::size_t size,
                               const std::size_t localhalosize,
                               const std::string&  windowobjectname = std::string() );

        };
    }
}

#include "./detail/WindowObjectImpl.h"

#endif // WINDOWOBJECT_H

