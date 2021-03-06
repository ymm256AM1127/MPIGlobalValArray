﻿#ifndef WINDOWOBJALLOCATOR_H
#define WINDOWOBJALLOCATOR_H

#include <type_traits>
#include <mpi.h>
#include "Environment.h"
#include "../MPL/meta_function.h"

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
            using CommPtr        = Environment::CommPtr;
            using size_t         = std::size_t;
            using value_type     = T;
            using pointer        = value_type*;

            int CreateWindowObj( pointer &baseptr,       // out
                                 MPI_Win &win,           // out
                                 size_t  &localCapacityCount, // out
                                 CommPtr* comm,
                                 const size_t globalsize,
                                 const size_t localhalosize,
                                 const std::string&  windowobjectname )
            {
                return static_cast<U &>(this)->CreateWindowObj( baseptr,
                                                                win,
                                                                localCapacityCount,
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
                                 size_t  &localCapacityCount,
                                 Environment::CommPtr comm,
                                 const size_t globalsize,
                                 const size_t localhalosize,
                                 const std::string&  windowobjectname );

            int DeleteWindowObj( MPI_Win &win );
        };
    }
}

#endif // WINDOWOBJALLOCATOR_H

#include "./detail/WindowObjAllocatorImpl.h"
