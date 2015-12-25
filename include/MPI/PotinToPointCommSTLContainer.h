#ifndef POINTTOPOINCOMMSTLCONTAINER_H
#define POINTTOPOINCOMMSTLCONTAINER_H

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
            //! STLコンテナのプロトタイプ
            template< class T > inline int _Send_       ( std_container_traits_tag, const T &dataSend, const int dest, const int i32Tag, Communicator* comm, const int count = 0 );
            template< class T > inline int _Recv_       ( std_container_traits_tag, T &dataRecv, const int source, const int i32Tag, Communicator* comm, const int count );
            template< class T > inline int _Isend_      ( std_container_traits_tag, const T &dataSend, const int dest, const int i32Tag, Communicator* comm, const int count = 0 );
            template< class T > inline int _Irecv_      ( std_container_traits_tag, T &dataRecv, const int source, const int i32Tag, Communicator* comm, const int count );
    }
}
#endif // POINTTOPOINCOMMSTLCONTAINER_H

#include "detail/PointToPointCommSTLContainerImpl.h"

