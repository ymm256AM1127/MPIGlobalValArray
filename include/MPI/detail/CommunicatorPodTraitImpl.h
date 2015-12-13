#ifndef COMMUNICATORPODTRAITIMPL_H
#define COMMUNICATORPODTRAITIMPL_H

#include "CommunicatorImpl.h"
#include "../../SharedLibraryDefine.h"
#include "../DataTypeDefine.h"
#include "../PreDefinedDataType.h"
#include "../PreDefinedOperationType.h"
#include "../../Utility/ScopedMutex.h"

#include <iostream>

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        /*!
         * \brief Implemantation of pod_traits_tag =====================================================
         */
        // _Send_
        template< class T >
        int     Communicator::_Send_( pod_traits_tag, const T &dataSend, const int dest, const int count )
        {
            int retival = 0;

            retival = MPI_Send( (void*)&dataSend, count, MPIDataType<T>(), dest, m_i32Tag, m_Commnunicator );

            return retival;
        }
        // _Recv_
        template<typename T>
        int     Communicator::_Recv_( pod_traits_tag,     T &dataRecv, const int source, const int count )
        {
            int retival = 0;

            retival = MPI_Recv( (void*)&dataRecv, count, MPIDataType<T>(), source, m_i32Tag, m_Commnunicator, MPI_STATUS_IGNORE );

            return retival;
        }

        // _Iend_
        template<typename T>
        int     Communicator::_Isend_( pod_traits_tag, const T &dataSend, const int dest, const int count )
        {
            int retival = 0;
            Utility::ScopedMutex<std::mutex> locker( &m_Mutex );
            m_vectMPI_Requests.push_back( MPI_Request() );
            int i32LastIndex = m_vectMPI_Requests.size() - 1;

            retival = MPI_Isend( (void*)&dataSend, count, MPIDataType<T>(), dest,
                                 m_i32Tag, m_Commnunicator, &m_vectMPI_Requests[i32LastIndex] );

            return retival;
        }
        // _Irecv_
        template<typename T>
        int     Communicator::_Irecv_( pod_traits_tag,          T &dataRecv, const int source, const int count )
        {
            int retival = 0;
            Utility::ScopedMutex<std::mutex> locker( &m_Mutex );
            m_vectMPI_Requests.push_back( MPI_Request() );
            int i32LastIndex = m_vectMPI_Requests.size() - 1;

            retival = MPI_Irecv( (void*)&dataRecv, count, MPIDataType<T>(), source,
                                 m_i32Tag, m_Commnunicator, &m_vectMPI_Requests[i32LastIndex] );

            return retival;
        }

        // Bcast
        template<typename T>
        int     Communicator::_Bcast_(pod_traits_tag, T &dataBuffer, const int srcRank, const int itemCount)
        {
            return MPI_Bcast( (void*)&dataBuffer, itemCount, MPIDataType<T>(), srcRank, m_Commnunicator );
        }

        // Gather
        template<typename T>
        int     Communicator::_Gather_( pod_traits_tag,
                                        const T &SendBuffer, typename list_trais<T>::type &RecvBuffer,
                                        const int rootRank, const int itemCount )
        {
            return MPI_Gather( (void*)&SendBuffer, itemCount, MPIDataType<T>(),
                               (void*)&RecvBuffer, itemCount, MPIDataType<T>(),
                               rootRank, m_Commnunicator );
        }

        // Scatter
        template<typename T>
        int     Communicator::_Scatter_( pod_traits_tag,
                                         const typename list_trais<T>::type &SendBuffer, T &RecvBuffer,
                                         const int rootRank, const int itemCount )
        {
            return MPI_Scatter( (void*)&SendBuffer, itemCount, MPIDataType<T>(),
                                (void*)&RecvBuffer, itemCount, MPIDataType<T>(),
                                rootRank, m_Commnunicator );
        }

        // AllGather
        template<typename T>
        int     Communicator::_AllGather_( pod_traits_tag,
                                           const T &SendBuffer, typename list_trais<T>::type &RecvBuffer,
                                           const int itemCount )
        {
            return MPI_Allgather( (void*)&SendBuffer, itemCount, MPIDataType<T>(),
                                  (void*)&RecvBuffer, itemCount, MPIDataType<T>(), m_Commnunicator );
        }

        // Alltoall
        template<typename T>
        int     Communicator::_Alltoall_( pod_traits_tag,
                                          const typename list_trais<T>::type &SendBuffer,
                                                typename list_trais<T>::type &RecvBuffer,
                                          const int itemCount )
        {
            return MPI_Alltoall( (void*)&SendBuffer, itemCount, MPIDataType<T>(),
                                 (void*)&RecvBuffer, itemCount, MPIDataType<T>(), m_Commnunicator );
        }

        // Reduce
        template<typename T >
        int     Communicator::_Reduce_( pod_traits_tag, MPI_Op Op,
                                        const typename reducible_type<T>::type &SendBuffer,
                                        typename reducible_type<T>::type &RecvBuffer,
                                        const int rootRank, const int itemCount )
        {
            return MPI_Reduce( (void*)&SendBuffer,(void*)&RecvBuffer, itemCount, MPIDataType<T>(), Op, rootRank, m_Commnunicator );
        }

        // AllReduce
        template<typename T >
        int     Communicator::_Allreduce_( pod_traits_tag, MPI_Op Op,
                                           const typename reducible_type<T>::type &SendBuffer,
                                           typename reducible_type<T>::type &RecvBuffer,
                                           const int itemCount )
        {
            return MPI_Allreduce( (void*)&SendBuffer,(void*)&RecvBuffer, itemCount, MPIDataType<T>(), Op, m_Commnunicator );
        }

    }
}

#endif // COMMUNICATORPODTRAITIMPL_H

