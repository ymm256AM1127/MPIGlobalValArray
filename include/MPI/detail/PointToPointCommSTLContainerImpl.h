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
                sendsize = dataSend.size();
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
            MPI_Probe( source, i32Tag, comm->GetCommunicator(), &status);

            int sendsize = count;
            //! ステータスからサイズを取得する。（データ型に注意）
            MPI_Get_count(&status, MPIDataType<value_type>(), &sendsize);

            T tempbuffer( sendsize, 0 );

            retival = MPI_Recv( (void*)tempbuffer.data(), sendsize, MPIDataType<value_type>(), source, i32Tag, comm->GetCommunicator(), MPI_STATUS_IGNORE );

            dataRecv.swap( tempbuffer );

            return retival;
        }

        // _Iend_
        template<typename T>
        int     _Isend_( std_container_traits_tag, const T &dataSend, const int dest, const int i32Tag, Communicator* comm, const int count )
        {
            int retival = 0;

            Utility::ScopedMutex<std::mutex> locker( &comm->Mutex() );
            std::function<void()> func = [&dataSend, dest, count, i32Tag, comm]()
            {
                int targetCount = static_cast< int >( dataSend.size() );

                MPI_Request request_size;
                //! 転送サイズを予め通知
                MPI_Isend( (void*)&targetCount, 1, MPIDATATYPE<int>(), dest, i32Tag, comm->GetCommunicator(), &request_size );
                //! マルチスレッドで実行されるので非同期で待つ
                //! ここにMutexを入れたほうがよい？
                MPI_Wait( &request_size, MPI_STATUS_IGNORE );

                MPI_Request request_data;

                MPI_Isend( (void*)dataSend.data(),
                           targetCount,
                           MPIDATATYPE< typename T::value_type >(),
                           dest,
                           i32Tag,
                           comm->GetCommunicator(),
                           &request_data );

                MPI_Wait( &request_data, MPI_STATUS_IGNORE );

//                _Send_<T>( std_container_traits_tag(), dataSend, dest, i32Tag, comm, count );
            };
            comm->ThreadPool().push_back( std::thread( func ) );

            return retival;
        }
        // _Irecv_
        template<typename T>
        int     _Irecv_( std_container_traits_tag,          T &dataRecv, const int source, const int i32Tag, Communicator* comm, const int count )
        {
            int retival = 0;

            Utility::ScopedMutex<std::mutex> locker( &comm->Mutex() );
            std::function<void()> func = [ &dataRecv, source, count, i32Tag, comm]()
            {
                int targetCount = count;
                MPI_Request request_size;

                MPI_Irecv( (void*)&targetCount, 1, MPIDATATYPE<int>(), source, i32Tag, comm->GetCommunicator(), &request_size );

                //! マルチスレッドで実行されるので非同期で待つ
                //! ここにMutexを入れたほうがよい？
                MPI_Wait( &request_size, MPI_STATUS_IGNORE );

                T tempbuffer( targetCount, ZEROVALUE<typename T::value_type >() );

                MPI_Request request_data;

                MPI_Irecv( (void*)tempbuffer.data(),
                           targetCount,
                           MPIDATATYPE< typename T::value_type >(),
                           source,
                           i32Tag,
                           comm->GetCommunicator(),
                           &request_data );

                MPI_Wait( &request_data, MPI_STATUS_IGNORE );

                tempbuffer.swap( dataRecv );

//                _Recv_<T>( std_container_traits_tag(), dataRecv, source, i32Tag, comm, count );
            };
            comm->ThreadPool().push_back( std::thread( func ) );

            return retival;
        }

    }
}

#endif // POINTTOPOINTCOMMSTLCONTAINERIMPL_H

