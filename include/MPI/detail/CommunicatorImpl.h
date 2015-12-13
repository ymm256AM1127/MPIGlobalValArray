#ifndef COMMUNICATORIMPL_H
#define COMMUNICATORIMPL_H

#include "../Communicator.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
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
            return _Send_< T >( typename comm_trais<T>::tag(), dataSend, dest, count );
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
            return _Recv_< T >( typename comm_trais<T>::tag(), dataRecv, source, count );
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
            return _Isend_< T >( typename comm_trais<T>::tag(),  dataSend, dest, count);
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
            return _Irecv_( typename comm_trais<T>::tag(), dataRecv, source, count );
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
            return _Bcast_< T >( typename comm_trais<T>::tag(), dataBufer, srcRank, itemCount );
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
        int Communicator::Gather( const T &SendBuffer, typename list_trais<T>::type &RecvBuffer, const int rootRank, const int itemCount )
        {
            return _Gather_< T >( typename comm_trais<T>::tag(), SendBuffer, RecvBuffer, rootRank, itemCount );
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
        int Communicator::Scatter( const typename list_trais<T>::type &SendBuffer, T &RecvBuffer, const int rootRank, const int itemCount )
        {
            return _Scatter_< T >( typename comm_trais<T>::tag(), SendBuffer, RecvBuffer, rootRank, itemCount );
        }

        template< class T >
        /*!
         * \brief AllGather
         * \param SendBuffer
         * \param RecvBuffer
         * \param itemCount
         * \return
         */
        int Communicator::AllGather( const T &SendBuffer, typename list_trais<T>::type &RecvBuffer, const int itemCount )
        {
            return _AllGather_< T >( typename comm_trais<T>::tag(), SendBuffer, RecvBuffer, itemCount );
        }

        template< class T >
        /*!
         * \brief Alltoall
         * \param SendBuffer
         * \param RecvBuffer
         * \param itemCount
         * \return
         */
        int Communicator::Alltoall( const typename list_trais<T>::type &SendBuffer,
                                    typename list_trais<T>::type &RecvBuffer, const int itemCount )
        {
            return _Alltoall_< T >( typename comm_trais<T>::tag(), SendBuffer, RecvBuffer, itemCount );
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
                             SendBuffer, RecvBuffer, rootRank, itemCount );
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
            return _Allreduce_<T>( typename comm_trais<T>::tag(), MPIOpType<T,OP>(), SendBuffer, RecvBuffer, itemCount );
        }
    }
}
#endif // COMMUNICATORIMPL_H

#include "CommunicatorPodTraitImpl.h"
#include "CommunicatorSTLContainerTraitImpl.h"

