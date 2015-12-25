#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "../SharedLibraryDefine.h"
#include "../Utility/SingletonHolder.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <mpi.h>
#include "CommunicatorTraits.h"
#include <mutex>
#include <thread>

namespace _MYNAMESPACE_
{
    using namespace MPL;
    namespace  MPI
    {
        class  SHAREDLIBRARYDEFINE_EXPORT Communicator
        {
        public:
            Communicator();
            ~Communicator();

            void                        Barrier() const;
            void                        SetCommunicator( const MPI_Comm& comm );
            void                        SetTag( const int tag );
            void                        WaitAll();

            int                         GetTag() const;
            MPI_Comm                    GetCommunicator();
            std::vector<MPI_Request>&   MPI_Requests();
            std::mutex&                 Mutex();
            std::vector<std::thread>&   ThreadPool();

            /*!
             * \brief point to point communication =====================================================================
             */
            template< class T > int Send     ( const T &dataSend, const int dest,    const int count = 0 );
            template< class T > int Recv     ( T &dataRecv,       const int source,  const int count = 0 );
            template< class T > int Isend    ( const T &dataSend, const int dest,    const int count = 0 );
            template< class T > int Irecv    ( T &dataRecv,       const int source,  const int count = 0 );

            /*!
             * \brief corrective communication =========================================================================
             */
            template< class T > int Bcast    ( T &dataBufer,      const int srcRank, const int itemCount = 0 );
            template< class T > int Gather   ( const T &SendBuffer, T &RecvBuffer,
                                               const int rootRank, const int itemCount = 0 );
            template< class T > int Scatter  ( const T &SendBuffer, T &RecvBuffer,
                                               const int rootRank, const int itemCount = 0 );
            template< class T > int AllGather( const T &SendBuffer,
                                               T &RecvBuffer, const int itemCount = 0 );
            template< class T > int Alltoall ( const T &SendBuffer,
                                               T &RecvBuffer, const int itemCount = 0 );

            template< int OP, class T > int Reduce   ( const typename reducible_type<T>::type &SendBuffer,
                                                       typename reducible_type<T>::type &RecvBuffer,
                                                       const int rootRank, const int itemCount = 0 );

            template< int OP, class T > int Allreduce( const typename reducible_type<T>::type &SendBuffer,
                                                       typename reducible_type<T>::type &RecvBuffer,
                                                       const int itemCount = 0 );

        private:
            int                         m_i32Tag;
            MPI_Comm                    m_Commnunicator;
            std::vector<MPI_Request>    m_vectMPI_Requests;
            std::mutex                  m_Mutex;
            std::vector<std::thread>    m_vectThreadPool;

            /*!
             * \brief ここにデータ形式の異なるトレイツのプロトタイプを行う。 ====================================================
             */

            //! POD(Plain Old Data)のプロトタイプ
//            template< class T > inline int _Send_       ( pod_traits_tag, const T &dataSend, const int dest, const int count = 0 );
//            template< class T > inline int _Recv_       ( pod_traits_tag, T &dataRecv, const int source, const int count );
//            template< class T > inline int _Isend_      ( pod_traits_tag, const T &dataSend, const int dest, const int count = 0 );
//            template< class T > inline int _Irecv_      ( pod_traits_tag, T &dataRecv, const int source, const int count );

            template< class T > inline int _Bcast_      ( pod_traits_tag, T &dataBuffer, const int srcRank, const int itemCount = 0 );

            template< class T > inline int _Gather_     ( pod_traits_tag, const T &SendBuffer,
                                                          T &RecvBuffer,
                                                          const int rootRank, const int itemCount = 0 );
            template< class T > inline int _Scatter_    ( pod_traits_tag, const T &SendBuffer,
                                                          T &RecvBuffer, const int rootRank, const int itemCount );

            template< class T > inline int _AllGather_  ( pod_traits_tag, const T &SendBuffer,
                                                          T &RecvBuffer,
                                                          const int itemCount = 0 );
            template< class T > inline int _Alltoall_   ( pod_traits_tag, const T &SendBuffer,
                                                          T &RecvBuffer, const int itemCount = 0 );

            template< class T > inline int _Reduce_     ( pod_traits_tag, MPI_Op Op,
                                                          const typename reducible_type<T>::type &SendBuffer,
                                                          typename reducible_type<T>::type &RecvBuffer,
                                                          const int rootRank, const int itemCount = 0 );
            template< class T > inline int _Allreduce_  ( pod_traits_tag, MPI_Op Op,
                                                          const typename reducible_type<T>::type &SendBuffer,
                                                          typename reducible_type<T>::type &RecvBuffer, const int itemCount = 0 );
            //! STLコンテナのプロトタイプ
//            template< class T > inline int _Send_       ( std_container_traits_tag, const T &dataSend, const int dest, const int count = 0 );
//            template< class T > inline int _Recv_       ( std_container_traits_tag, T &dataRecv, const int source, const int count );
//            template< class T > inline int _Isend_      ( std_container_traits_tag, const T &dataSend, const int dest, const int count = 0 );
//            template< class T > inline int _Irecv_      ( std_container_traits_tag, T &dataRecv, const int source, const int count );

            template< class T > inline int _Bcast_      ( std_container_traits_tag, T &dataBuffer, const int srcRank, const int itemCount = 0 );

            template< class T > inline int _Gather_     ( std_container_traits_tag, const T &SendBuffer,
                                                          T &RecvBuffer,
                                                          const int rootRank, const int itemCount = 0 );
            template< class T > inline int _Scatter_    ( std_container_traits_tag, const T &SendBuffer,
                                                          T &RecvBuffer, const int rootRank, const int itemCount = 0 );

            template< class T > inline int _AllGather_  ( std_container_traits_tag, const T &SendBuffer,
                                                          T &RecvBuffer,
                                                          const int itemCount = 0 );
            template< class T > inline int _Alltoall_   ( std_container_traits_tag, const T &SendBuffer,
                                                          T &RecvBuffer, const int itemCount = 0 );


        };

        typedef SingletonHolder< Communicator >  CommunicatorSingleton;
    }
}
#endif // COMMUNICATOR_H

#include "detail/CommunicatorImpl.h"

#define MPICommPtr _MYNAMESPACE_::MPI::CommunicatorSingleton::GetInstance()

