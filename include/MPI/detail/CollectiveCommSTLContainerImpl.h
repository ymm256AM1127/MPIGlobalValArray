#ifndef COLLECTIVECOMMSTLCONTAINERIMPL_H
#define COLLECTIVECOMMSTLCONTAINERIMPL_H

#include "../CollectiveCommSTLContainer.h"
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
        // Bcast
        template<typename T>
        int     _Bcast_(std_container_traits_tag, T &dataBuffer, const int srcRank, Communicator *comm, const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            UNUSED_VARIABLE( itemCount );

            int retival  = 0;

            int sendsize = 0;

            //! size check
            if( itemCount == 0 )
            {
                sendsize = dataBuffer.size();
            }
            else if( static_cast<int>( dataBuffer.size() ) > itemCount )  //例外を投げておく。MPIは例外が出た時点で止まる。
            {
                int myrank = 0;
                MPI_Comm_rank( comm->GetCommunicator(), &myrank );
                std::stringstream ss;
                ss << "Error Code: " << BROADCASTSIZEERROR << " on file: " << __FILE__ << ", Line: " << __LINE__ << ", Rank: " << myrank << std::endl;
                throw std::range_error( ss.str() );
            }

            MPI_Bcast( (void*)&sendsize, 1, MPIDataType<value_type>(), srcRank, comm->GetCommunicator() );

            int myrank = 0;
            MPI_Comm_rank( comm->GetCommunicator(), &myrank );

            if( myrank != srcRank )
            {
                T temp( sendsize, 0 );
                dataBuffer.swap( temp );
            }

            MPI_Bcast( (void*)dataBuffer.data(), sendsize, MPIDataType<value_type>(), srcRank, comm->GetCommunicator() );

            return retival;
        }

        // Gather
        template<typename T>
        /*!
         * \brief _Gather_
         * アイテムカウントは無効となる。
         * \param SendBuffer
         * \param RecvBuffer
         * \param rootRank
         * \param itemCount
         * \return
         */
        int     _Gather_( std_container_traits_tag,
                          const T &SendBuffer,
                          T &RecvBuffer,
                          const int rootRank,
                          Communicator* comm,
                          const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            UNUSED_VARIABLE( itemCount );

            int retival  = 0;

            int SendSize    = static_cast<int>( SendBuffer.size() );

            //! 送信バッファは指定されたコンテナのサイズの最小とし、そのサイズの総ランク分の
            //! バッファを受信バッファとする。その理由として、最大値を必要とすると少ない方の
            //! コンテナのサイズを拡張する必要があり、パフォーマンスの面で良くないため。
            int MinSendSize = 0;
            MPI_Allreduce( &SendSize, &MinSendSize, 1, MPI_INT, MPI_MIN, comm->GetCommunicator() );

            int MPISize = 0;
            MPI_Comm_size( comm->GetCommunicator(), &MPISize );

            //! 受信バッファサイズの計算
            int RecvSize = MinSendSize * MPISize;

            T recvbuffer( RecvSize, 0 );


            retival =  MPI_Gather( (void*)SendBuffer.data(), MinSendSize, MPIDataType<value_type>(),
                                   (void*)recvbuffer.data(), MinSendSize, MPIDataType<value_type>(),
                                   rootRank, comm->GetCommunicator() );

            RecvBuffer.swap( recvbuffer );

            return retival;
        }

        // Scatter
        template<typename T>
        /*!
         * \brief _Scatter_
         * \param SendBuffer
         * \param RecvBuffer
         * \param rootRank
         * \param itemCount
         * \return
         */
        int     _Scatter_( std_container_traits_tag,
                           const T &SendBuffer,
                           T &RecvBuffer,
                           const int rootRank,
                           Communicator* comm,
                           const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;
            UNUSED_VARIABLE( itemCount );

            int retival  = 0;
            int myrank   = 0;
            int mpisize  = 1;
            int sendsize = 0;
            MPI_Comm_rank( comm->GetCommunicator(), &myrank  );
            MPI_Comm_size( comm->GetCommunicator(), &mpisize );

            if( myrank == rootRank )
            {
                sendsize = SendBuffer.size();

                if( sendsize > 0 && ( ( sendsize % mpisize ) == 0 ) )
                {
                    sendsize = sendsize / mpisize;
                }
                else
                {
                    std::stringstream ss;
                    ss << "Error Code: " << SCCATERSIZEERROR << " on file: " << __FILE__ << ", Line: " << __LINE__ << ", Rank: " << myrank << std::endl;
                    throw std::range_error( ss.str() );
                }
            }

            MPI_Bcast( &sendsize, 1, MPI_INT, rootRank, comm->GetCommunicator() );

            T temp( sendsize, 0 );

            retival = MPI_Scatter( (void*)SendBuffer.data(), sendsize, MPIDATATYPE<value_type>(), (void*)temp.data(), sendsize, MPIDATATYPE<value_type>(), rootRank, comm->GetCommunicator()  );

            RecvBuffer.swap( temp );

            return retival;
        }

        // AllGather
        template<typename T>
        int     _AllGather_( std_container_traits_tag,
                             const T &SendBuffer,
                             T &RecvBuffer,
                             Communicator* comm,
                             const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            UNUSED_VARIABLE( itemCount );

            int retival  = 0;

            int SendSize    = static_cast<int>( SendBuffer.size() );

            //! 送信バッファは指定されたコンテナのサイズの最小とし、そのサイズの総ランク分の
            //! バッファを受信バッファとする。その理由として、最大値を必要とすると少ない方の
            //! コンテナのサイズを拡張する必要があり、パフォーマンスの面で良くないため。
            int MinSendSize = 0;
            MPI_Allreduce( &SendSize, &MinSendSize, 1, MPI_INT, MPI_MIN, comm->GetCommunicator() );
            int MPISize = 0;
            MPI_Comm_size( comm->GetCommunicator(), &MPISize );

            //! 受信バッファサイズの計算
            int RecvSize = MinSendSize * MPISize;

            T recvbuffer( RecvSize , 0 );

            retival =  MPI_Allgather( (void*)SendBuffer.data(), MinSendSize, MPIDataType<value_type>(),
                                      (void*)recvbuffer.data(), MinSendSize, MPIDataType<value_type>(),
                                      comm->GetCommunicator() );

            RecvBuffer.swap( recvbuffer );

            return retival;
        }

        // Alltoall
        template<typename T>
        int     _Alltoall_( std_container_traits_tag,
                            const T &SendBuffer,
                            T &RecvBuffer,
                            Communicator* comm,
                            const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            UNUSED_VARIABLE( itemCount );

            int retival  = 0;

            int SendSize    = static_cast<int>( SendBuffer.size() );

            //! 送信バッファは指定されたコンテナのサイズの最小とし、そのサイズの総ランク分の
            //! バッファを受信バッファとする。その理由として、最大値を必要とすると少ない方の
            //! コンテナのサイズを拡張する必要があり、パフォーマンスの面で良くないため。
            int MinSendSize = 0;
            MPI_Allreduce( &SendSize, &MinSendSize, 1, MPI_INT, MPI_MIN, comm->GetCommunicator() );

            int MPISize = 0;
            MPI_Comm_size( comm->GetCommunicator(), &MPISize );

            if( SendSize > 0 && ( ( SendSize % MPISize ) == 0 ) )
            {
                SendSize = SendSize / MPISize;
            }
            else
            {
                int myrank   = 0;
                MPI_Comm_rank( comm->GetCommunicator(), &myrank  );
                std::stringstream ss;
                ss << "Error Code: " << ALLTOALLSIZEERROR << " on file: " << __FILE__ << ", Line: " << __LINE__ << ", Rank: " << myrank << std::endl;
                throw std::range_error( ss.str() );
            }

            //! 受信バッファサイズの計算
            int RecvSize = MinSendSize;

            T recvbuffer( RecvSize , 0 );

            retival = MPI_Alltoall( (void*)SendBuffer.data(), SendSize, MPIDATATYPE<value_type>(), (void*)recvbuffer.data(), SendSize, MPIDATATYPE<value_type>() , comm->GetCommunicator()  );

            RecvBuffer.swap( recvbuffer );

            return retival;
        }



    }
}

#endif // COLLECTIVECOMMSTLCONTAINERIMPL_H

