#ifndef POINTTOPOINTCOMMSTLCONTAINERIMPL_H
#define POINTTOPOINTCOMMSTLCONTAINERIMPL_H

#include "../PotinToPointCommSTLContainer.h"
#include "../Communicator.h"
#include "../../SharedLibraryDefine.h"
#include "../DataTypeDefine.h"
#include "../PreDefinedDataType.h"
#include "../PreDefinedOperationType.h"
#include "../../Utility/ScopedMutex.h"
#include "../ErrorCode.h"

#include <iostream>

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        /*!
         * \brief Implemantation of std_container_traits_tag ============================================
         */
        // _Send_
        template<typename T>
        int _Send_( std_container_traits_tag, const T &dataSend, const int dest, const int i32Tag, Communicator* comm, const int count )
        {
            //! ここでコンテナがPOD型であるかどうかを確認する。
            //! 複雑な型の場合はenable_ifでSFINAEが働くようにする。
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            int retival = 0;

            int sendsize = 0;

            //! size check
            if( count == 0 )
            {
                sendsize = static_cast<int>( dataSend.size() );
            }
            else if( static_cast<int>( dataSend.size() ) > count )  //例外を投げておく。MPIは例外が出た時点で止まる。
            {
                int myrank = 0;
                MPI_Comm_rank( comm->GetCommunicator(), &myrank );
                std::stringstream ss;
                ss << "Error Code: " << P2PCOMMSIZEERROR << " on file: " << __FILE__ << ", Line: " << __LINE__ << ", Rank: " << myrank << std::endl;
                throw std::range_error( ss.str() );
            }

            retival = MPI_Send( (void*)dataSend.data(), sendsize, MPIDataType<value_type>(), dest, i32Tag, comm->GetCommunicator() );

            return retival;
        }
        // _Recv_
        template<typename T>
        int     _Recv_( std_container_traits_tag, T &dataRecv, const int source, const int i32Tag, Communicator* comm, const int count )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            int retival = 0;

            MPI_Status status;
            //! プローブで送られてくるステータスを確認する
            MPI_Probe( source, i32Tag, comm->GetCommunicator(), &status );

            int sendsize = count;
            //! ステータスからサイズを取得する。（データ型に注意）
            MPI_Get_count(&status, MPIDataType<value_type>(), &sendsize );

            T tempbuffer( sendsize, 0 );

            retival = MPI_Recv( (void*)tempbuffer.data(), sendsize, MPIDataType<value_type>(), source, i32Tag, comm->GetCommunicator(), MPI_STATUS_IGNORE );

            dataRecv.swap( tempbuffer );

            return retival;
        }

        // _Iend_
        template<typename T>
        ImmediateRetType _Isend_( std_container_traits_tag, const T &dataSend, const int dest, const int i32Tag, Communicator* comm, const int count )
        {
            ImmediateRetType retival;
            int targetCount = static_cast< int >( dataSend.size() );

            retival.ReturnValue = MPI_Isend( (void*)dataSend.data(), targetCount, MPIDATATYPE< typename T::value_type >(), dest, i32Tag, comm->GetCommunicator(), &retival.request );

            return retival;
        }

        // _Irecv_
        template<typename T>
        ImmediateRetType  _Irecv_( std_container_traits_tag,  T &dataRecv, const int source, const int i32Tag, Communicator* comm, const int count )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;
            ImmediateRetType retival;
            int flag = 0;

            //! ビジーループで待つ.
            while( flag == 0 )
            {
                MPI_Iprobe( source, i32Tag, comm->GetCommunicator(), &flag, &retival.status );
            }

            int sendsize = 0;

            //! ステータスからサイズを取得する。（データ型に注意）
            MPI_Get_count( &retival.status, MPIDataType<value_type>(), &sendsize );

            T tempbuffer( sendsize, ZEROVALUE<typename T::value_type >() );
            dataRecv.swap( tempbuffer );

            retival.ReturnValue = MPI_Irecv( (void*)dataRecv.data(), sendsize, MPIDATATYPE< typename T::value_type >(), source, i32Tag, comm->GetCommunicator(), &retival.request );

            return retival;
        }

    }
}

#endif // POINTTOPOINTCOMMSTLCONTAINERIMPL_H

