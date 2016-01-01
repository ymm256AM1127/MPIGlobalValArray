#include "../../include/MPI/Communicator.h"
#include "../../include/Utility/ScopedMutex.h"
#include <QDebug>

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        Communicator::Communicator( const MPI_Comm comm ) :
            m_i32Tag( 0 ), m_Commnunicator( comm )
        {
            MPI_Comm_rank( m_Commnunicator, &m_i32MPIRank );
            MPI_Comm_size( m_Commnunicator, &m_i32MPISize );
        }

        Communicator::~Communicator()
        {
            //! 登録されているWindowを削除する。
            //! MPIの実装の中には、MPI APIで確保したメモリを自分で開放しないとFinalizeが
            //! 呼ばれた時に、エラーや警告をだすため。
            //! また、複数のコミュニケータを定義した場合におけるもちろんメモリリーク対策もあ
            //! り。若干コストがかかるが、MPIにおいてコミュニケータを高速に再定義することは、
            //! 想定されていないので、何度も呼び出すことには向いていない。
            if( !m_WindowObjSet.empty() )
            {
                for( auto iter : m_WindowObjSet )
                {
                    ((*iter))();
                }
                //! swapを用いてデストラクタによりマップを削除。
                WinObsSetType ForClearSet;
                m_WindowObjSet.swap( ForClearSet );
            }
        }

        /*!
         * \brief Communicator::EraseWinObjSet
         * \param func
         * \return
         */
        int Communicator::EraseWinObjSet(const std::function<void ()>* func)
        {
            int ret = MPI_SUCCESS;
            auto iter = m_WindowObjSet.find( const_cast<std::function<void ()>*>( func )  );
            if( iter != m_WindowObjSet.end() )
            {
                //! 登録した関数を実行(ラムダ式の関数をポインタで収納しているのでややわかりづらい。)
                (*(*iter))();
                m_WindowObjSet.erase( iter );
            }
            else
            {
                ret = CANNOTERASEWINFEOMCOMM;
            }
            return ret;
        }

        /*!
         * \brief Communicator::RegisterWinObjSet
         * \param func
         * \return
         */
        int Communicator::RegisterWinObjSet( const std::function<void ()>* func)
        {
            int ret = MPI_SUCCESS;
            auto iter = m_WindowObjSet.find( const_cast<std::function<void ()>*>( func ) );
            if( iter == m_WindowObjSet.end() )
            {
                m_WindowObjSet.insert( const_cast<std::function<void ()>*>( func )  );
            }
            else
            {
                ret = CANNOTREGISTWINTOCOMM;
            }
            return ret;
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


