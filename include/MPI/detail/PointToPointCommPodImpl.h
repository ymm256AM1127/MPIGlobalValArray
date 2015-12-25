#ifndef POINTTOPOINTCOMMPODIMPL_H
#define POINTTOPOINTCOMMPODIMPL_H

#include "../PotinToPointCommPod.h"
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
        int     _Send_( pod_traits_tag, const T &dataSend, const int dest, const int i32Tag, Communicator* comm, const int count )
        {
            int retival = 0;

            retival = MPI_Send( (void*)&dataSend, count, MPIDataType<T>(), dest, i32Tag, comm->GetCommunicator() );

            return retival;
        }
        // _Recv_
        template<typename T>
        int     _Recv_( pod_traits_tag,     T &dataRecv, const int source, const int i32Tag, Communicator* comm, const int count )
        {
            int retival = 0;

            retival = MPI_Recv( (void*)&dataRecv, count, MPIDataType<T>(), source, i32Tag, comm->GetCommunicator(), MPI_STATUS_IGNORE );

            return retival;
        }

        // _Iend_
        template<typename T>
        int     _Isend_( pod_traits_tag, const T &dataSend, const int dest, const int i32Tag, Communicator* comm, const int count )
        {
            int retival = 0;
            Utility::ScopedMutex<std::mutex> locker( &comm->Mutex() );
            comm->MPI_Requests().push_back( MPI_Request() );
            int i32LastIndex = comm->MPI_Requests().size() - 1;

            retival = MPI_Isend( (void*)&dataSend, count, MPIDataType<T>(), dest,
                                 i32Tag, comm->GetCommunicator(), &comm->MPI_Requests()[i32LastIndex] );

            return retival;
        }

        // _Irecv_
        template<typename T>
        int     _Irecv_( pod_traits_tag,          T &dataRecv, const int source, const int i32Tag, Communicator* comm, const int count )
        {
            int retival = 0;
            Utility::ScopedMutex<std::mutex> locker( &comm->Mutex() );
            comm->MPI_Requests().push_back( MPI_Request() );
            int i32LastIndex = comm->MPI_Requests().size() - 1;

            retival = MPI_Irecv( (void*)&dataRecv, count, MPIDataType<T>(), source,
                                 i32Tag, comm->GetCommunicator(), &comm->MPI_Requests()[i32LastIndex] );

            return retival;
        }
    }
}

#endif // POINTTOPOINTCOMMPODIMPL_H

