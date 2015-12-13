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
        /*!
         * \brief Implemantation of std_container_traits_tag ============================================
         */
        // _Send_
        template<typename T>
        int Communicator::_Send_( std_container_traits_tag, const T &dataSend, const int dest, const int count )
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
                MPI_Comm_rank( m_Commnunicator, &myrank );
                std::stringstream ss;
                ss << "Error Code: " << P2PCOMMSIZEERROR << " on file: " << __FILE__ << ", Line: " << __LINE__ << ", Rank: " << myrank << std::endl;
                throw std::range_error( ss.str() );
            }

            retival = MPI_Send( (void*)dataSend.data(), sendsize, MPIDataType<value_type>(), dest, m_i32Tag, m_Commnunicator );

            return retival;
        }
        // _Recv_
        template<typename T>
        int     Communicator::_Recv_( std_container_traits_tag, T &dataRecv, const int source, const int count )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            int retival = 0;

            MPI_Status status;
            //! プローブで送られてくるステータスを確認する
            MPI_Probe( source, m_i32Tag, m_Commnunicator, &status);

            int sendsize = count;
            //! ステータスからサイズを取得する。（データ型に注意）
            MPI_Get_count(&status, MPIDataType<value_type>(), &sendsize);

            T tempbuffer( sendsize, 0 );

            retival = MPI_Recv( (void*)tempbuffer.data(), sendsize, MPIDataType<value_type>(), source, m_i32Tag, m_Commnunicator, MPI_STATUS_IGNORE );

            dataRecv.swap( tempbuffer );

            return retival;
        }

        // _Iend_
        template<typename T>
        int     Communicator::_Isend_( std_container_traits_tag, const T &dataSend, const int dest, const int count )
        {
            int retival = 0;

            Utility::ScopedMutex<std::mutex> locker( &m_Mutex );
            std::function<void()> func = [&dataSend, dest, count, this]()
            {
                this->_Send_( std_container_traits_tag(), dataSend, dest, count );
            };
            m_vectThreadPool.push_back(  std::thread( func ) );

            return retival;
        }
        // _Irecv_
        template<typename T>
        int     Communicator::_Irecv_( std_container_traits_tag,          T &dataRecv, const int source, const int count )
        {
            int retival = 0;

            Utility::ScopedMutex<std::mutex> locker( &m_Mutex );
            std::function<void()> func = [ &dataRecv, source, count, this]()
            {
                this->_Recv_( std_container_traits_tag(), dataRecv, source, count );
            };
            m_vectThreadPool.push_back( std::thread( func ) );

            return retival;
        }


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
        int     Communicator::_Gather_( std_container_traits_tag, const T &SendBuffer, typename list_trais<T>::type &RecvBuffer, const int rootRank, const int itemCount )
        {
            using value_type = typename std::enable_if< is_pod_with_complex< typename T::value_type >::value, typename T::value_type >::type;

            int retival = 0;

            int sendsize = 0;

            //! size check
            if( itemCount == 0 )
            {
                sendsize = SendBuffer.size();
            }
            else if( static_cast<int>( SendBuffer.size() ) > itemCount )  //例外を投げておく。MPIは例外が出た時点で止まる。
            {
                int myrank = 0;
                MPI_Comm_rank( m_Commnunicator, &myrank );
                std::stringstream ss;
                ss << "Error Code: " << GATHERSIZEERROR << " on file: " << __FILE__ << ", Line: " << __LINE__ << ", Rank: " << myrank << std::endl;
                throw std::range_error( ss.str() );
            }


            return retival;
        }

        // Scatter
        template<typename T>
        int     Communicator::_Scatter_( std_container_traits_tag, const typename list_trais<T>::type &SendBuffer, T &RecvBuffer, const int rootRank, const int itemCount )
        {
            return 0;
        }

        // AllGather
        template<typename T>
        int     Communicator::_AllGather_( std_container_traits_tag,
                                           const T &SendBuffer, typename list_trais<T>::type &RecvBuffer,
                                           const int itemCount )
        {
            return 0;
        }

        // Alltoall
        template<typename T>
        int     Communicator::_Alltoall_( std_container_traits_tag,
                                          const typename list_trais<T>::type &SendBuffer,
                                                typename list_trais<T>::type &RecvBuffer,
                                          const int itemCount )
        {
            return 0;
        }

        // Reduce
        template<typename T >
        int     Communicator::_Reduce_( std_container_traits_tag, MPI_Op Op,
                                        const typename reducible_type<T>::type &SendBuffer,
                                        typename reducible_type<T>::type &RecvBuffer,
                                        const int rootRank, const int itemCount )
        {
            return 0;
        }

        // AllReduce
        template<typename T >
        int     Communicator::_Allreduce_( std_container_traits_tag, MPI_Op Op,
                                           const typename reducible_type<T>::type &SendBuffer,
                                           typename reducible_type<T>::type &RecvBuffer, const int itemCount )
        {
            return 0;
        }

    }

}


#endif // COMMUNICATORSTLCONTAINERTRAITIMPL_H

