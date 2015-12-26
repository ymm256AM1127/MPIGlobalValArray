#include "../../include/MPI/Communicator.h"
#include "../../include/Utility/ScopedMutex.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        Communicator::Communicator() : m_i32Tag( 0 ), m_Commnunicator( MPI_COMM_WORLD )
        {
            MPI_Comm_rank( m_Commnunicator, &m_i32MPIRank );
            MPI_Comm_size( m_Commnunicator, &m_i32MPISize );
        }

        Communicator::Communicator( const MPI_Comm comm, const int tag ) :
            m_i32Tag( tag ), m_Commnunicator( comm )
        {
            MPI_Comm_rank( m_Commnunicator, &m_i32MPIRank );
            MPI_Comm_size( m_Commnunicator, &m_i32MPISize );
        }

        Communicator::~Communicator()
        {

        }

        /*!
         * \brief _MYNAMESPACE_::MPI::Communicator::SetCommunicator
         * \param comm
         */
        void Communicator::SetCommunicator(const MPI_Comm &comm)
        {
            m_Commnunicator = comm;
        }

        /*!
         * \brief Communicator::SetTag
         * \param tag
         */
        void Communicator::SetTag(const int tag)
        {
            m_i32Tag = tag;
        }

        /*!
         * \brief _MYNAMESPACE_::MPI::Communicator::WaitAll
         * このメンバ関数が呼ばれる前にすべての非同期インストラクションが
         * 発行されていなければならない。つまりマルチスレッド下で、
         * このメンバ関数を呼ぶことは非推奨
         */
        void Communicator::WaitAll()
        {
            Utility::ScopedMutex<std::mutex> locker( &m_Mutex );
            if( !m_vectMPI_Requests.empty() )
            {
                MPI_Waitall( m_vectMPI_Requests.size(), m_vectMPI_Requests.data(), MPI_STATUSES_IGNORE );
                std::vector< MPI_Request > newRequest;
                m_vectMPI_Requests.swap( newRequest );
            }

            //! コンテナによる非同期P2P通信を行った場合にスレッドの待ち合わせ。
            if( !m_vectThreadPool.empty() )
            {
                for( auto &thread : m_vectThreadPool )
                {
                    thread.join();
                }
                std::vector<std::thread> newthreadpool;
                m_vectThreadPool.swap( newthreadpool );
            }
        }
    }
}


