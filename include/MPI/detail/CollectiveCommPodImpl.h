#ifndef COLLECTIVECOMMPODIMPL_H
#define COLLECTIVECOMMPODIMPL_H

#include "../CollectiveCommPod.h"
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
        // Bcast
        template<typename T>
        int     _Bcast_(pod_traits_tag, T &dataBuffer, const int srcRank, Communicator* comm, const int itemCount)
        {
            return MPI_Bcast( (void*)&dataBuffer, itemCount, MPIDataType<T>(), srcRank, comm->GetCommunicator() );
        }

        // Gather
        template<typename T>
        int     _Gather_( pod_traits_tag,
                          const T &SendBuffer,
                          T &RecvBuffer,
                          const int rootRank,
                          Communicator* comm,
                          const int itemCount )
        {
            return MPI_Gather( (void*)&SendBuffer, itemCount, MPIDataType<T>(),
                               (void*)&RecvBuffer, itemCount, MPIDataType<T>(),
                               rootRank, comm->GetCommunicator() );
        }

        // Scatter
        template<typename T>
        int     _Scatter_( pod_traits_tag,
                           const T &SendBuffer,
                           T &RecvBuffer,
                           const int rootRank,
                           Communicator* comm,
                           const int itemCount )
        {
            return MPI_Scatter( (void*)&SendBuffer, itemCount, MPIDataType<T>(),
                                (void*)&RecvBuffer, itemCount, MPIDataType<T>(),
                                rootRank, comm->GetCommunicator() );
        }

        // AllGather
        template<typename T>
        int     _AllGather_( pod_traits_tag,
                             const T &SendBuffer,
                             T &RecvBuffer,
                             Communicator* comm,
                             const int itemCount )
        {
            return MPI_Allgather( (void*)&SendBuffer, itemCount, MPIDataType<T>(),
                                  (void*)&RecvBuffer, itemCount, MPIDataType<T>(), comm->GetCommunicator() );
        }

        // Alltoall
        template<typename T>
        int     _Alltoall_( pod_traits_tag,
                            const T &SendBuffer,
                            T &RecvBuffer,
                            Communicator* comm,
                            const int itemCount )
        {
            return MPI_Alltoall( (void*)&SendBuffer, itemCount, MPIDataType<T>(),
                                 (void*)&RecvBuffer, itemCount, MPIDataType<T>(), comm->GetCommunicator() );
        }

        // Reduce
        template<typename T >
        int     _Reduce_( pod_traits_tag,
                          MPI_Op Op,
                          const typename reducible_type<T>::type &SendBuffer,
                          typename reducible_type<T>::type &RecvBuffer,
                          const int rootRank,
                          Communicator* comm,
                          const int itemCount )
        {
            return MPI_Reduce( (void*)&SendBuffer,(void*)&RecvBuffer, itemCount, MPIDataType<T>(), Op, rootRank, comm->GetCommunicator() );
        }

        // AllReduce
        template<typename T >
        int     _Allreduce_( pod_traits_tag,
                             MPI_Op Op,
                             const typename reducible_type<T>::type &SendBuffer,
                             typename reducible_type<T>::type &RecvBuffer,
                             Communicator* comm,
                             const int itemCount )
        {
            return MPI_Allreduce( (void*)&SendBuffer,(void*)&RecvBuffer, itemCount, MPIDataType<T>(), Op, comm->GetCommunicator() );
        }


    }
}

#endif // COLLECTIVECOMMPODIMPL_H

