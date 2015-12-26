#ifndef POINTTOPOINCOMMPOD_H
#define POINTTOPOINCOMMPOD_H

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
            template< class T > inline int _Send_       ( pod_traits_tag, const T &dataSend, const int dest, const int i32Tag, Communicator* comm, const int count = 0 );
            template< class T > inline int _Recv_       ( pod_traits_tag, T &dataRecv, const int source, const int i32Tag, Communicator* comm, const int count );
            template< class T > inline int _Isend_      ( pod_traits_tag, const T &dataSend, const int dest, const int i32Tag, Communicator* comm, const int count = 0 );
            template< class T > inline int _Irecv_      ( pod_traits_tag, T &dataRecv, const int source, const int i32Tag, Communicator* comm, const int count );
    }
}
#endif // POINTTOPOINCOMMPOD_H

#include "detail/PointToPointCommPodImpl.h"

