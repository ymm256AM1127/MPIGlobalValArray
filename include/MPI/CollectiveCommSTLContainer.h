#ifndef COLLECTIVECOMMCONTAINER_H
#define COLLECTIVECOMMCONTAINER_H

#include "../SharedLibraryDefine.h"
#include "Communicator.h"

namespace _MYNAMESPACE_
{
    using namespace MPL;
    namespace  MPI
    {
        /*!
         * \brief ここにデータ形式の異なるトレイツのプロトタイプを行う。 ====================================================
         */
        //! STLコンテナ型のプロトタイプ
        template< class T > inline int _Bcast_      ( std_container_traits_tag, T &dataBuffer, const int srcRank, Communicator* comm, const int itemCount = 0 );

        template< class T > inline int _Gather_     ( std_container_traits_tag,
                                                      const T &SendBuffer,
                                                      T &RecvBuffer,
                                                      const int rootRank,
                                                      Communicator* comm,
                                                      const int itemCount = 0 );

        template< class T > inline int _Scatter_    ( std_container_traits_tag,
                                                      const T &SendBuffer,
                                                      T &RecvBuffer,
                                                      const int rootRank,
                                                      Communicator* comm,
                                                      const int itemCount );

        template< class T > inline int _AllGather_  ( std_container_traits_tag,
                                                      const T &SendBuffer,
                                                      T &RecvBuffer,
                                                      const int itemCount = 0 );

        template< class T > inline int _Alltoall_   ( std_container_traits_tag,
                                                      const T &SendBuffer,
                                                      T &RecvBuffer,
                                                      Communicator* comm,
                                                      const int itemCount = 0 );

        template< class T > inline int _Reduce_     ( std_container_traits_tag, MPI_Op Op,
                                                      const typename reducible_type<T>::type &SendBuffer,
                                                      typename reducible_type<T>::type &RecvBuffer,
                                                      const int rootRank,
                                                      Communicator* comm,
                                                      const int itemCount = 0 );

        template< class T > inline int _Allreduce_  ( std_container_traits_tag, MPI_Op Op,
                                                      const typename reducible_type<T>::type &SendBuffer,
                                                      typename reducible_type<T>::type &RecvBuffer,
                                                      Communicator* comm,
                                                      const int itemCount = 0 );
    }
}
#endif // COLLECTIVECOMMCONTAINER_H

#include "detail/CollectiveCommSTLContainerImpl.h"

