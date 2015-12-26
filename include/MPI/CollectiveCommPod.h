#ifndef COLLECTIVECOMMPOD_H
#define COLLECTIVECOMMPOD_H

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
        //! POD(Plain Old Data)のプロトタイプ
        template< class T > inline int _Bcast_      ( pod_traits_tag, T &dataBuffer, const int srcRank, Communicator* comm, const int itemCount = 0 );

        template< class T > inline int _Gather_     ( pod_traits_tag,
                                                      const T &SendBuffer,
                                                      T &RecvBuffer,
                                                      const int rootRank,
                                                      Communicator* comm,
                                                      const int itemCount = 0 );

        template< class T > inline int _Scatter_    ( pod_traits_tag,
                                                      const T &SendBuffer,
                                                      T &RecvBuffer,
                                                      const int rootRank,
                                                      Communicator* comm,
                                                      const int itemCount );

        template< class T > inline int _AllGather_  ( pod_traits_tag,
                                                      const T &SendBuffer,
                                                      T &RecvBuffer,
                                                      const int itemCount = 0 );

        template< class T > inline int _Alltoall_   ( pod_traits_tag,
                                                      const T &SendBuffer,
                                                      T &RecvBuffer,
                                                      Communicator* comm,
                                                      const int itemCount = 0 );

        template< class T > inline int _Reduce_     ( pod_traits_tag, MPI_Op Op,
                                                      const typename reducible_type<T>::type &SendBuffer,
                                                      typename reducible_type<T>::type &RecvBuffer,
                                                      const int rootRank,
                                                      Communicator* comm,
                                                      const int itemCount = 0 );

        template< class T > inline int _Allreduce_  ( pod_traits_tag, MPI_Op Op,
                                                      const typename reducible_type<T>::type &SendBuffer,
                                                      typename reducible_type<T>::type &RecvBuffer,
                                                      Communicator* comm,
                                                      const int itemCount = 0 );
    }
}
#endif // COLLECTIVECOMMPOD_H

#include "detail/CollectiveCommPodImpl.h"

