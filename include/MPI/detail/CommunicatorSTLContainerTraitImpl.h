#ifndef COMMUNICATORSTLCONTAINERTRAITIMPL_H
#define COMMUNICATORSTLCONTAINERTRAITIMPL_H

#include "../ErrorCode.h"
#include "CommunicatorImpl.h"
#include <exception>
#include <sstream>
#include "../DataTypeDefine.h"
#include "../PreDefinedDataType.h"
#include "../PreDefinedOperationType.h"
#include "../../Utility/ScopedMutex.h"
#include <functional>
#include <iostream>

namespace _MYNAMESPACE_
{
    namespace MPI
    {
//        /*!
//         * \brief Implemantation of std_container_traits_tag ============================================
//         */
//        // _Send_
//        template<typename T>
//        int Communicator::_Send_( std_container_traits_tag, const T &dataSend, const int dest, const int count )
//        {
//            //! ここでコンテナがPOD型であるかどうかを確認する。
//            //! 複雑な型の場合はenable_ifでSFINAEが働くようにする。
//            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

//            int retival = 0;

//            int sendsize = 0;

//            //! size check
//            if( count == 0 )
//            {
//                sendsize = dataSend.size();
//            }
//            else if( static_cast<int>( dataSend.size() ) > count )  //例外を投げておく。MPIは例外が出た時点で止まる。
//            {
//                int myrank = 0;
//                MPI_Comm_rank( m_Commnunicator, &myrank );
//                std::stringstream ss;
//                ss << "Error Code: " << P2PCOMMSIZEERROR << " on file: " << __FILE__ << ", Line: " << __LINE__ << ", Rank: " << myrank << std::endl;
//                throw std::range_error( ss.str() );
//            }

//            retival = MPI_Send( (void*)dataSend.data(), sendsize, MPIDataType<value_type>(), dest, m_i32Tag, m_Commnunicator );

//            return retival;
//        }
//        // _Recv_
//        template<typename T>
//        int     Communicator::_Recv_( std_container_traits_tag, T &dataRecv, const int source, const int count )
//        {
//            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

//            int retival = 0;

//            MPI_Status status;
//            //! プローブで送られてくるステータスを確認する
//            MPI_Probe( source, m_i32Tag, m_Commnunicator, &status);

//            int sendsize = count;
//            //! ステータスからサイズを取得する。（データ型に注意）
//            MPI_Get_count(&status, MPIDataType<value_type>(), &sendsize);

//            T tempbuffer( sendsize, 0 );

//            retival = MPI_Recv( (void*)tempbuffer.data(), sendsize, MPIDataType<value_type>(), source, m_i32Tag, m_Commnunicator, MPI_STATUS_IGNORE );

//            dataRecv.swap( tempbuffer );

//            return retival;
//        }

//        // _Iend_
//        template<typename T>
//        int     Communicator::_Isend_( std_container_traits_tag, const T &dataSend, const int dest, const int count )
//        {
//            int retival = 0;

//            Utility::ScopedMutex<std::mutex> locker( &m_Mutex );
//            std::function<void()> func = [&dataSend, dest, count, this]()
//            {
//                this->_Send_( std_container_traits_tag(), dataSend, dest, count );
//            };
//            m_vectThreadPool.push_back(  std::thread( func ) );

//            return retival;
//        }
//        // _Irecv_
//        template<typename T>
//        int     Communicator::_Irecv_( std_container_traits_tag,          T &dataRecv, const int source, const int count )
//        {
//            int retival = 0;

//            Utility::ScopedMutex<std::mutex> locker( &m_Mutex );
//            std::function<void()> func = [ &dataRecv, source, count, this]()
//            {
//                this->_Recv_( std_container_traits_tag(), dataRecv, source, count );
//            };
//            m_vectThreadPool.push_back( std::thread( func ) );

//            return retival;
//        }


        // Bcast
        template<typename T>
        int     Communicator::_Bcast_(std_container_traits_tag, T &dataBuffer, const int srcRank, const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            int retival = 0;

            int sendsize = 0;

            //! size check
            if( itemCount == 0 )
            {
                sendsize = dataBuffer.size();
            }
            else if( static_cast<int>( dataBuffer.size() ) > itemCount )  //例外を投げておく。MPIは例外が出た時点で止まる。
            {
                int myrank = 0;
                MPI_Comm_rank( m_Commnunicator, &myrank );
                std::stringstream ss;
                ss << "Error Code: " << BROADCASTSIZEERROR << " on file: " << __FILE__ << ", Line: " << __LINE__ << ", Rank: " << myrank << std::endl;
                throw std::range_error( ss.str() );
            }

            MPI_Bcast( (void*)&sendsize, 1, MPIDataType<value_type>(), srcRank, m_Commnunicator );

            int myrank = 0;
            MPI_Comm_rank( m_Commnunicator, &myrank );

            if( myrank != srcRank )
            {
                T temp( sendsize, 0 );
                dataBuffer.swap( temp );
            }

            MPI_Bcast( (void*)dataBuffer.data(), sendsize, MPIDataType<value_type>(), srcRank, m_Commnunicator );

            return retival;
        }

        // Gather
        template<typename T>
        /*!
         * \brief Communicator::_Gather_
         * アイテムカウントは無効となる。
         * \param SendBuffer
         * \param RecvBuffer
         * \param rootRank
         * \param itemCount
         * \return
         */
        int     Communicator::_Gather_( std_container_traits_tag, const T &SendBuffer,
                                        T &RecvBuffer,
                                        const int rootRank,
                                        const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            int retival = 0;

            int SendSize    = static_cast<int>( SendBuffer.size() );

            //! 送信バッファは指定されたコンテナのサイズの最小とし、そのサイズの総ランク分の
            //! バッファを受信バッファとする。その理由として、最大値を必要とすると少ない方の
            //! コンテナのサイズを拡張する必要があり、パフォーマンスの面で良くないため。
            int MinSendSize = 0;
            this->Allreduce<MIN,int>( SendSize, MinSendSize, 1 );

            int MPISize = 0;
            MPI_Comm_size( m_Commnunicator, &MPISize );

            //! 受信バッファサイズの計算
            int RecvSize = MinSendSize * MPISize;

            T recvbuffer( RecvSize, 0 );


            retival =  MPI_Gather( (void*)SendBuffer.data(), MinSendSize, MPIDataType<value_type>(),
                                   (void*)recvbuffer.data(), MinSendSize, MPIDataType<value_type>(),
                                   rootRank, m_Commnunicator );

            RecvBuffer.swap( recvbuffer );

            return retival;
        }

        // Scatter
        template<typename T>
        /*!
         * \brief Communicator::_Scatter_
         * \param SendBuffer
         * \param RecvBuffer
         * \param rootRank
         * \param itemCount
         * \return
         */
        int     Communicator::_Scatter_( std_container_traits_tag, const T &SendBuffer, T &RecvBuffer, const int rootRank, const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;
            int retival  = 0;
            int myrank   = 0;
            int mpisize  = 1;
            int sendsize = 0;
            MPI_Comm_rank( m_Commnunicator, &myrank  );
            MPI_Comm_size( m_Commnunicator, &mpisize );

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

            this->Bcast<int>( sendsize, rootRank, 1 );

            T temp( sendsize, 0 );

            retival = MPI_Scatter( (void*)SendBuffer.data(), sendsize, MPIDATATYPE<value_type>(), (void*)temp.data(), sendsize, MPIDATATYPE<value_type>(), rootRank, m_Commnunicator  );

            RecvBuffer.swap( temp );

            return retival;
        }

        // AllGather
        template<typename T>
        int     Communicator::_AllGather_( std_container_traits_tag,
                                           const T &SendBuffer,
                                           T &RecvBuffer,
                                           const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            int retival = 0;

            int SendSize    = static_cast<int>( SendBuffer.size() );

            //! 送信バッファは指定されたコンテナのサイズの最小とし、そのサイズの総ランク分の
            //! バッファを受信バッファとする。その理由として、最大値を必要とすると少ない方の
            //! コンテナのサイズを拡張する必要があり、パフォーマンスの面で良くないため。
            int MinSendSize = 0;
            this->Allreduce< MIN, int>( SendSize, MinSendSize, 1 );

            int MPISize = 0;
            MPI_Comm_size( m_Commnunicator, &MPISize );

            //! 受信バッファサイズの計算
            int RecvSize = MinSendSize * MPISize;

            T recvbuffer( RecvSize , 0 );

            retival =  MPI_Allgather( (void*)SendBuffer.data(), MinSendSize, MPIDataType<value_type>(),
                                      (void*)recvbuffer.data(), MinSendSize, MPIDataType<value_type>(),
                                      m_Commnunicator );

            RecvBuffer.swap( recvbuffer );

            return retival;
        }

        // Alltoall
        template<typename T>
        int     Communicator::_Alltoall_( std_container_traits_tag,
                                          const T &SendBuffer,
                                                T &RecvBuffer,
                                          const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;
            int retival  = 0;

            int SendSize    = static_cast<int>( SendBuffer.size() );

            //! 送信バッファは指定されたコンテナのサイズの最小とし、そのサイズの総ランク分の
            //! バッファを受信バッファとする。その理由として、最大値を必要とすると少ない方の
            //! コンテナのサイズを拡張する必要があり、パフォーマンスの面で良くないため。
            int MinSendSize = 0;
            this->Allreduce< MIN, int>( SendSize, MinSendSize, 1 );

            int MPISize = 0;
            MPI_Comm_size( m_Commnunicator, &MPISize );

            if( SendSize > 0 && ( ( SendSize % MPISize ) == 0 ) )
            {
                SendSize = SendSize / MPISize;
            }
            else
            {
                int myrank   = 0;
                MPI_Comm_rank( m_Commnunicator, &myrank  );
                std::stringstream ss;
                ss << "Error Code: " << ALLTOALLSIZEERROR << " on file: " << __FILE__ << ", Line: " << __LINE__ << ", Rank: " << myrank << std::endl;
                throw std::range_error( ss.str() );
            }

            //! 受信バッファサイズの計算
            int RecvSize = MinSendSize;

            T recvbuffer( RecvSize , 0 );

            retival = MPI_Alltoall( (void*)SendBuffer.data(), SendSize, MPIDATATYPE<value_type>(), (void*)recvbuffer.data(), SendSize, MPIDATATYPE<value_type>() , m_Commnunicator  );

            RecvBuffer.swap( recvbuffer );

            return retival;
        }

    }

}


#endif // COMMUNICATORSTLCONTAINERTRAITIMPL_H

