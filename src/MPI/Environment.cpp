#include "../../include/MPI/Environment.h"


#include <iostream>
#include <sstream>
#include <stdexcept>
#include "../../include/MPI/ErrorCode.h"
#include <memory>
#include <vector>
#include "../../include/MPI/Communicator.h"
#include "../../include/MPI/DataTypes.h"
#include <QDebug>

#define _MPI_MULTI_THREAD_ENABLE_

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        Environment::Environment()
        {
            m_IsLocalNodeMaster           = false;
        }

        Environment::~Environment()
        {
            //! 登録したCommunicatorのメモリ確保する。
            if( !m_CommAllocSet.empty() )
            {
                CommAllocSet temp;
                for( auto iter : m_CommAllocSet )
                {
                    iter.reset();
                }
                temp.swap( m_CommAllocSet );
            }
            this->Finalize();
        }

        /*!
         * \brief Environment::Init
         * \param argc
         * \param argv
         */
        void Environment::Init(int argc, char *argv[])
        {
        #ifndef _MPI_MULTI_THREAD_ENABLE_
            MPI_Init( &argc, &argv );
        #else
            int provided = 0;
            MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE, &provided );
            if( provided != MPI_THREAD_MULTIPLE && provided != MPI_THREAD_SERIALIZED )
            {
                std::stringstream ss;
                ss << "Error Code: " << NOTSUPPORTTHREADMULTIPLE << " => it does not definde MPI Thread Multiple or MPI Thread Serialized." << std::endl;
                throw std::domain_error( ss.str() );
                Finalize();
                Abort( NOTSUPPORTTHREADMULTIPLE );
            }
        #endif

            //! カスタムDataTypeを登録する。
            CustomDataTypeRegister();
            //! カスタムオペレーションを登録する。
            CustomOperationTypeRegister();

            int i32MPIRank = -1;
            int i32MPISize = -1;
            MPI_Comm_rank( MPI_COMM_WORLD, &i32MPIRank );
            MPI_Comm_size( MPI_COMM_WORLD, &i32MPISize );

            //! ホスト名を取得
            std::vector<char> hostnametemp( _MPI_NAME_LENGH_, 0 );
            int  i32Resultlength = 0;
            MPI_Get_processor_name( hostnametemp.data(), &i32Resultlength );
            m_strHostName = std::string( hostnametemp.data() , i32Resultlength );
            m_strHostName.shrink_to_fit();

            //! ノードマップ（ランクマップ）を作成する
            CreateGlobalNodeMap();
            CreateLocalNodeMap();
            //! フラットMPIを行った場合の物理ノードのチェック
            //! ただし、ホスト名で識別しているので、厳密なチェックではない。
            CheckLocalNodeMaster();

            MPI_Barrier( MPI_COMM_WORLD );

        }

        void Environment::Finalize()
        {
            MPI_Finalize();
        }

        void Environment::Abort(const int error_code)
        {
            MPI_Abort( MPI_COMM_WORLD, error_code );
        }


        std::string Environment::GetHostName() const
        {
            return m_strHostName;
        }

        /*!
         * \brief Environment::IsLocalNodeMaster
         * \return
         */
        bool Environment::IsLocalNodeMaster() const
        {
            return m_IsLocalNodeMaster;
        }

        /*!
         * \brief Environment::CreateCommunicator
         * \param comm
         * \return
         */
        std::shared_ptr<Communicator> Environment::CreateCommunicator( const MPI_Comm comm )
        {
            std::shared_ptr<Communicator> temp( new Communicator( comm ) );
            auto error = m_CommAllocSet.insert( temp );
            if( !error.second )
            {
                throw std::domain_error( " Cannnot Create Communicator." );
            }
            return temp;
        }

        /*!
         * \brief Environment::RemoveCommunicator
         * \param comm
         */
        void Environment::RemoveCommunicator( CommPtr comm )
        {
            auto iter = m_CommAllocSet.find( comm );
            if( iter != m_CommAllocSet.end() )
            {
                m_CommAllocSet.erase( iter );
            }
        }

        /*!
         * \brief Environment::CreateGlobalNodeMap
         */
        void Environment::CreateGlobalNodeMap()
        {
            std::vector<char> hostnametemp( _MPI_NAME_LENGH_, 0 );
            int  i32Resultlength = 0;
            MPI_Get_processor_name( hostnametemp.data(), &i32Resultlength );

            int i32MPISize = -1;
            MPI_Comm_size( MPI_COMM_WORLD, &i32MPISize );

            //! 総ランク数分の文字列バッファを用意し、allgatherで集めてくる。
            std::vector<int> hostnamesizelist( i32MPISize , 0 );
            std::vector<char> hostnamelist( _MPI_NAME_LENGH_ * i32MPISize , 0 );
            MPI_Allgather( &i32Resultlength, 1, MPI_INT, hostnamesizelist.data(), 1, MPI_INT, MPI_COMM_WORLD );
            MPI_Allgather( hostnametemp.data(), _MPI_NAME_LENGH_, MPI_CHAR, hostnamelist.data(), _MPI_NAME_LENGH_, MPI_CHAR, MPI_COMM_WORLD );

            for( auto ii = 0; ii < i32MPISize; ii++ )
            {
                std::string temp( &hostnamelist[ ii * _MPI_NAME_LENGH_ ], hostnamesizelist[ii] );
                temp.shrink_to_fit();
                m_GlobalMPIRankMap[ ii ] = temp;
            }
        }

        /*!
         * \brief Environment::CreateLocalNodeMap
         * This member function must be called after CreateGlobalNodeMap member function
         */
        void Environment::CreateLocalNodeMap()
        {
            for( auto iter : m_GlobalMPIRankMap )
            {
                if( iter.second == GetHostName() )
                {
                    auto ret = m_LocalMPIRankMap.insert( iter );
                    if( !ret.second )
                    {
                        std::stringstream ss;
                        ss << "Error Code: " << CANNOTINSERTVALTOMAP << " on file: " << __FILE__ << ", Line: " << __LINE__ << std::endl;
                        throw std::domain_error( ss.str() );
                    }
                }
            }
        }

        /*!
         * \brief Environment::CheckLocalNodeMaster
         */
        void Environment::CheckLocalNodeMaster()
        {
            int i32MPIRank = -1;
            MPI_Comm_size( MPI_COMM_WORLD, &i32MPIRank );
            int minnumberofRank = i32MPIRank;
            for( auto iter : m_LocalMPIRankMap )
            {
                if( iter.first < minnumberofRank )
                {
                    minnumberofRank = iter.first;
                }
            }

            if( minnumberofRank == i32MPIRank )
            {
                m_IsLocalNodeMaster = true;
            }
        }
    }
}
