#ifndef WINDOWOBJALLOCATOR_H
#define WINDOWOBJALLOCATOR_H

#include <type_traits>
#include <mpi.h>
#include "Environment.h"
#include "../MPL/meta_function.h"
#include <QDebug>

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        /*!
         * \brief _Allocator \n
         * Allocatorプロトタイプ
         */
        template < class T > class _WinAllocator;

        /*!
         * \brief WinInterface \n
         * WinInterfaceプロトタイプ
         */
        template < class T, class U > class WinAllocateInterface;

        /*!
         * \brief The WinInterface class \n
         * ここにWinInterfaceの機能を追加していく。 静的なポリモーフィズムなので、追加したら\n
         * このクラスを継承して、インスタンスを生成するすべてのクラスに機能を追加しなければならない。
         */
        template < class T , class U >
        class WinAllocateInterface
        {
        public:
            using CommPtr         = Environment::CommPtr;
            using size_t          = std::size_t;
            using value_type     = T;
            using pointer        = value_type*;
//            using const_pointer   = const value_type*;
//            using reference       = value_type&;
//            using const_reference = const value_type&;

            int CreateWindowObj( pointer &baseptr,  // out
                                 MPI_Win &win,     // out
                                 CommPtr* comm,
                                 const size_t globalsize,
                                 const size_t localhalosize,
                                 const std::string&  windowobjectname )
            {
                return static_cast<U &>(this)->CreateWindowObj( baseptr,
                                                                win,
                                                                comm,
                                                                globalsize,
                                                                localhalosize,
                                                                windowobjectname );
            }

            int DeleteWindowObj( MPI_Win win )
            {
                return static_cast<U &>(this)->DeleteWindowObj( win );
            }
        };

        template < class T >
        class WinAllocate : WinAllocateInterface< T, WinAllocate<T> >
        {
        public:
            using value_type = T;
            using pointer    = value_type*;

            int CreateWindowObj( pointer &baseptr,
                                 MPI_Win &win,
                                 Environment::CommPtr comm,
                                 const size_t globalsize,
                                 const size_t localhalosize,
                                 const std::string&  windowobjectname )
            {
                auto Globalsize       = globalsize;
                auto Localhalosize    = localhalosize;
                int MPISize             = -1;

                MPI_Comm_size( comm->GetCommunicator(), &MPISize );

                auto MPISize64 = static_cast< size_t >(MPISize);

                auto rest = ( ( Globalsize % MPISize64 ) == 0 ) ? 0 : MPISize64 - ( Globalsize % MPISize64 ) ;
                Globalsize += rest;
                size_t LocalSize = Globalsize / MPISize64 + Localhalosize;

                auto ret = MPI_Win_allocate( LocalSize * sizeof( T ),
                                             sizeof( T ),
                                             MPI_INFO_NULL,
                                             comm->GetCommunicator(),
                                             &baseptr,
                                             &win );

                if( MPI_SUCCESS != ret )
                {
                    //! エラーを書く
                }

                MPI_Win_set_name( win, windowobjectname.c_str() );

                return ret;

            }

            int DeleteWindowObj( MPI_Win &win )
            {
                int flag = 0;
                int *flaver = nullptr;
                int ret = MPI_SUCCESS;
                MPI_Win_get_attr( win, MPI_WIN_CREATE_FLAVOR, &flaver, &flag );
                if( *flaver == MPI_WIN_FLAVOR_ALLOCATE )
                {
                    value_type* baseptr = nullptr;
                    MPI_Win_get_attr( win, MPI_WIN_BASE, &baseptr, &flag );
//                    ret = MPI_Free_mem( baseptr );
                }
                MPI_Win_free( &win );
                win = MPI_WIN_NULL;
                return ret;
            }
        };
    }
}

#endif // WINDOWOBJALLOCATOR_H
