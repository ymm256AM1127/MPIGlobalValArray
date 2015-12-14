#include "../../include/MPI/Environment.h"


#include <iostream>
#include <sstream>
#include <stdexcept>
#include "../../include/MPI/ErrorCode.h"
#include <memory>
#include <vector>

#define _MPI_MULTI_THREAD_ENABLE_


_MYNAMESPACE_::MPI::Environment::Environment()
{
    m_i32MPIRank                  = 0;
    m_i32MPISize                  = 0;
    m_IsLocalNodeMaster           = false;
}

_MYNAMESPACE_::MPI::Environment::~Environment()
{

}

/*!
 * \brief _MYNAMESPACE_::MPI::Environment::Init
 * \param argc
 * \param argv
 */
void _MYNAMESPACE_::MPI::Environment::Init(int argc, char *argv[])
{
#ifndef _MPI_MULTI_THREAD_ENABLE_
    MPI_Init( &argc, &argv );
#else
    int provided = 0;
    MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE, &provided );
    if( provided != MPI_THREAD_MULTIPLE )
    {
        std::stringstream ss;
        ss << "Error Code: " << NOTSUPPORTTHREADMULTIPLE << " => it does not definde MPI Thread Multiple." << std::endl;
        throw std::domain_error( ss.str() );
        Finalize();
        Abort( NOTSUPPORTTHREADMULTIPLE );
    }
#endif
    MPI_Comm_rank( MPI_COMM_WORLD, &m_i32MPIRank );
    MPI_Comm_size( MPI_COMM_WORLD, &m_i32MPISize );

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

void _MYNAMESPACE_::MPI::Environment::Finalize()
{
    MPI_Finalize();
}

void _MYNAMESPACE_::MPI::Environment::Abort(const int error_code)
{
    MPI_Abort( MPI_COMM_WORLD, error_code );
}

bool _MYNAMESPACE_::MPI::Environment::IsRootRank() const
{
    return ( m_i32MPIRank == 0 );
}

int _MYNAMESPACE_::MPI::Environment::GetRootRank() const
{
    return 0;
}

int _MYNAMESPACE_::MPI::Environment::GetMPIRank() const
{
    return m_i32MPIRank;
}

int _MYNAMESPACE_::MPI::Environment::GetMPISize() const
{
    return m_i32MPISize;
}

std::string _MYNAMESPACE_::MPI::Environment::GetHostName() const
{
    return m_strHostName;
}

/*!
 * \brief _MYNAMESPACE_::MPI::Environment::IsLocalNodeMaster
 * \return
 */
bool _MYNAMESPACE_::MPI::Environment::IsLocalNodeMaster() const
{
    return m_IsLocalNodeMaster;
}

bool _MYNAMESPACE_::MPI::Environment::IsRootRank(const MPI_Comm &comm) const
{
    return ( GetMPIRank( comm ) == 0 );
}

/*!
 * \brief _MYNAMESPACE_::MPI::Environment::GetMPIRank
 * \param comm
 * \return
 */
int _MYNAMESPACE_::MPI::Environment::GetMPIRank(const MPI_Comm &comm) const
{
    int i32rank = 0;
    MPI_Comm_rank( comm, &i32rank );
    return i32rank;
}

/*!
 * \brief _MYNAMESPACE_::MPI::Environment::GetMPISize
 * \param comm
 * \return
 */
int _MYNAMESPACE_::MPI::Environment::GetMPISize(const MPI_Comm &comm) const
{
    int i32size;
    MPI_Comm_size( comm, &i32size );
    return i32size;
}

/*!
 * \brief _MYNAMESPACE_::MPI::Environment::CreateGlobalNodeMap
 */
void _MYNAMESPACE_::MPI::Environment::CreateGlobalNodeMap()
{
    std::vector<char> hostnametemp( _MPI_NAME_LENGH_, 0 );
    int  i32Resultlength = 0;
    MPI_Get_processor_name( hostnametemp.data(), &i32Resultlength );

    //! 総ランク数分の文字列バッファを用意し、allgatherで集めてくる。
    std::vector<int> hostnamesizelist( GetMPISize() , 0 );
    std::vector<char> hostnamelist( _MPI_NAME_LENGH_ * GetMPISize() , 0 );
    MPI_Allgather( &i32Resultlength, 1, MPI_INT, hostnamesizelist.data(), 1, MPI_INT, MPI_COMM_WORLD );
    MPI_Allgather( hostnametemp.data(), _MPI_NAME_LENGH_, MPI_CHAR, hostnamelist.data(), _MPI_NAME_LENGH_, MPI_CHAR, MPI_COMM_WORLD );

    for( auto ii = 0; ii < GetMPISize(); ii++ )
    {
        std::string temp( &hostnamelist[ ii * _MPI_NAME_LENGH_ ], hostnamesizelist[ii] );
        temp.shrink_to_fit();
        m_GlobalMPIRankMap[ ii ] = temp;
    }
}

/*!
 * \brief _MYNAMESPACE_::MPI::Environment::CreateLocalNodeMap
 * This member function must be called after CreateGlobalNodeMap member function
 */
void _MYNAMESPACE_::MPI::Environment::CreateLocalNodeMap()
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
 * \brief _MYNAMESPACE_::MPI::Environment::CheckLocalNodeMaster
 */
void _MYNAMESPACE_::MPI::Environment::CheckLocalNodeMaster()
{
    int minnumberofRank = GetMPIRank();
    for( auto iter : m_LocalMPIRankMap )
    {
        if( iter.first < minnumberofRank )
        {
            minnumberofRank = iter.first;
        }
    }

    if( minnumberofRank == GetMPIRank() )
    {
        m_IsLocalNodeMaster = true;
    }
}
