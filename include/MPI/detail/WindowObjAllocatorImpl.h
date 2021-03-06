﻿#ifndef WINDOWOBJALLOCATORIMPL_H
#define WINDOWOBJALLOCATORIMPL_H

#include "../../MPI/WindowObjAllocator.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        template < class T >
        int WinAllocate<T>::CreateWindowObj( pointer &baseptr,
                                             MPI_Win &win,
                                             size_t  &localCapacityCount,
                                             Environment::CommPtr comm,
                                             const size_t globalsize,
                                             const size_t localhalosize,
                                             const std::string &windowobjectname)

        {
            auto Globalsize       = globalsize;
            auto Localhalosize    = localhalosize;
            int MPISize           = -1;

            MPI_Comm_size( comm->GetCommunicator(), &MPISize );

            auto MPISize64 = static_cast< size_t >(MPISize);

            auto rest = ( ( Globalsize % MPISize64 ) == 0 ) ? 0 : MPISize64 - ( Globalsize % MPISize64 ) ;
            Globalsize += rest;
            size_t LocalSize = Globalsize / MPISize64 + Localhalosize;

#ifdef MSMPI_VER
    #if MSMPI_VER <= 0x100

            auto ret = MPI_Alloc_mem ( LocalSize * sizeof( T ), MPI_INFO_NULL, &baseptr );
            ret      = MPI_Win_create( baseptr, LocalSize * sizeof( T ),
                                       sizeof( T ),
                                       MPI_INFO_NULL,
                                       comm->GetCommunicator(),
                                       &win );
    #endif
#else

            auto ret = MPI_Win_allocate( LocalSize * sizeof( T ),
                                         sizeof( T ),
                                         MPI_INFO_NULL,
                                         comm->GetCommunicator(),
                                         &baseptr,
                                         &win );

#endif
            localCapacityCount = LocalSize;

            if( MPI_SUCCESS != ret )
            {
                //! エラーを書く
            }

            MPI_Win_set_name( win, windowobjectname.c_str() );

            return ret;

        }

        template < class T >
        int WinAllocate<T>::DeleteWindowObj(MPI_Win &win)
        {
            int ret = MPI_SUCCESS;
            //! たいていのMPIの実装ではWindowオブジェクトが開放されれば、
            //! 自分が確保したメモリも開放しているので、ベースポインタを自
            //! 前で開放する必要はない。ただしMSMPIv7までは、MPI3に完全
            //! に準拠していないため、MPI_Win_allocateがまだ未実装であ
            //! り、例外として扱う。
            #ifdef MSMPI_VER
                #if MSMPI_VER <= 0x100
                        T* baseptr = nullptr;
                        int flag   = 0;
                        MPI_Win_get_attr( win, MPI_WIN_BASE, &baseptr, &flag );
                        if( baseptr != nullptr )
                        {
                            MPI_Free_mem( baseptr );
                        }
                #endif
            #endif
            MPI_Win_free( &win );
            win = MPI_WIN_NULL;
            return ret;
        }
    }
}

#endif // WINDOWOBJALLOCATORIMPL_H
