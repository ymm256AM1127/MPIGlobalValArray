#ifndef COMMUNICATORIMPL_H
#define COMMUNICATORIMPL_H

#include "../Communicator.h"
#include "../PotinToPointCommPod.h"
#include "../PotinToPointCommSTLContainer.h"
#include "../CollectiveCommPod.h"
#include "../CollectiveCommSTLContainer.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        /*!
         * \brief MPI::Communicator::Barrier
         */
        void Communicator::Barrier() const
        {
            MPI_Barrier( m_Commnunicator );
        }

        bool Communicator::IsRootRank() const
        {
            return ( m_i32MPIRank == 0 );
        }

        int Communicator::GetRootRank() const
        {
            return 0;
        }

        int Communicator::GetMPIRank() const
        {
            return m_i32MPIRank;
        }

        int Communicator::GetMPISize() const
        {
            return m_i32MPISize;
        }

        int Communicator::GetTag() const
        {
            return m_i32Tag;
        }

        MPI_Comm Communicator::GetCommunicator()
        {
            return m_Commnunicator;
        }

        std::vector<MPI_Request> &Communicator::MPI_Requests()
        {
            return m_vectMPI_Requests;
        }

        std::mutex &Communicator::Mutex()
        {
            return m_Mutex;
        }

        std::vector<std::thread> &Communicator::ThreadPool()
        {
            return m_vectThreadPool;
        }
        template< class T >
        /*!
         * \brief Send
         * \param dataSend
         * \param dest
         * \param count
         * \return
         */
        int Communicator::Send( const T &dataSend, const int dest, const int count )
        {
            return _Send_< T >( typename comm_trais<T>::tag(), dataSend, dest, m_i32Tag, this, count );
        }

        template< class T >
        /*!
         * \brief Recv
         * \param dataRecv
         * \param source
         * \param count
         * \return
         */
        int Communicator::Recv(  T &dataRecv, const int source, const int count )
        {
            return _Recv_< T >( typename comm_trais<T>::tag(), dataRecv, source, m_i32Tag, this, count );
        }

        template< class T >
        /*!
         * \brief Isend
         * \param dataSend
         * \param dest
         * \param count
         * \return
         */
        int Communicator::Isend( const T &dataSend, const int dest, const int count )
        {
            return _Isend_< T >( typename comm_trais<T>::tag(),  dataSend, dest, m_i32Tag, this, count);
        }

        template< class T >
        /*!
         * \brief Irecv
         * \param dataRecv
         * \param source
         * \param count
         * \return
         */
        int Communicator::Irecv(  T &dataRecv, const int source, const int count )
        {
            return _Irecv_( typename comm_trais<T>::tag(), dataRecv, source, m_i32Tag, this, count );
        }


        template< class T >
        /*!
         * \brief Bcast
         * \param dataBufer
         * \param srcRank
         * \param itemCount
         * \return
         */
        int Communicator::Bcast( T &dataBufer, const int srcRank, const int itemCount )
        {
            return _Bcast_< T >( typename comm_trais<T>::tag(), dataBufer, srcRank, this, itemCount );
        }

        template< class T >
        /*!
         * \brief Gather
         * \param SendBuffer
         * \param RecvBuffer
         * \param rootRank
         * \param itemCount
         * \return
         */
        int Communicator::Gather( const T &SendBuffer, T &RecvBuffer, const int rootRank, const int itemCount )
        {
            return _Gather_< T >( typename comm_trais<T>::tag(), SendBuffer, RecvBuffer, rootRank, this, itemCount );
        }


        template< class T >
        /*!
         * \brief Scatter
         * \param SendBuffer
         * \param RecvBuffer
         * \param rootRank
         * \param itemCount
         * \return
         */
        int Communicator::Scatter( const T &SendBuffer, T &RecvBuffer, const int rootRank, const int itemCount )
        {
            return _Scatter_< T >( typename comm_trais<T>::tag(), SendBuffer, RecvBuffer, rootRank, this, itemCount );
        }

        template< class T >
        /*!
         * \brief AllGather
         * \param SendBuffer
         * \param RecvBuffer
         * \param itemCount
         * \return
         */
        int Communicator::AllGather( const T &SendBuffer, T &RecvBuffer, const int itemCount )
        {
            return _AllGather_< T >( typename comm_trais<T>::tag(), SendBuffer, RecvBuffer, this, itemCount );
        }

        template< class T >
        /*!
         * \brief Alltoall
         * \param SendBuffer
         * \param RecvBuffer
         * \param itemCount
         * \return
         */
        int Communicator::Alltoall( const T &SendBuffer,
                                    T &RecvBuffer, const int itemCount )
        {
            return _Alltoall_< T >( typename comm_trais<T>::tag(), SendBuffer, RecvBuffer, this, itemCount );
        }

        template<int OP,  class T  >
        /*!
         * \brief Reduce
         * \param SendBuffer
         * \param RecvBuffer
         * \param rootRank
         * \param itemCount
         * \return
         */
        int Communicator::Reduce( const typename reducible_type<T>::type &SendBuffer,
                                  typename reducible_type<T>::type &RecvBuffer, const int rootRank, const int itemCount )
        {
            return _Reduce_< T >( typename comm_trais<T>::tag(), MPIOpType< T, OP >(),
                             SendBuffer, RecvBuffer, rootRank, this, itemCount );
        }

        template<int OP,  class T  >
        /*!
         * \brief Allreduce
         * \param SendBuffer
         * \param RecvBuffer
         * \param itemCount
         * \return
         */
        int Communicator::Allreduce( const typename reducible_type<T>::type &SendBuffer,
                                     typename reducible_type<T>::type &RecvBuffer, const int itemCount )
        {
            return _Allreduce_<T>( typename comm_trais<T>::tag(), MPIOpType<T,OP>(), SendBuffer, RecvBuffer, this, itemCount );
        }
    }
}
#endif // COMMUNICATORIMPL_H

//#include "CommunicatorPodTraitImpl.h"
//#include "CommunicatorSTLContainerTraitImpl.h"

