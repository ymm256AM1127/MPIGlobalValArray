#include "MPIWapperUtils.h"
#include <iostream>

std::unordered_map< void*, MPI_Win > MPIWinAllocMap;

//! MPIが実行されているかどうかのフラグ
//! ラッパーであるMPIInit()とMPIFinalize()で
//! 書き換える。デフォルトはfalse
//!
//! 状態遷移
//! MPIInit():true
//! MPIFinalize():false
bool MPIRunningFlag = false;

/*!
 * \brief MPIInit
 * \param argc
 * \param argv
 */
void MPIInit( int argc, char** &argv)
{
    MPI_Init( &argc, &argv );
    MPIRunningFlag = true;
}

/*!
 * \brief MPIFinalize
 */
void MPIFinalize()
{
    for( auto iter : MPIWinAllocMap )
    {
        MPI_Win_free( &iter.second );
    }
    MPI_Barrier( MPI_COMM_WORLD );
    MPIWinAllocMap.clear();
    MPI_Finalize();

    MPIRunningFlag = false;
}

/*!
 * \brief MPISize
 * \return
 */
int MPISize()
{
    int size = 0;
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    return size;
}

/*!
 * \brief MPIRank
 * \return
 */
int MPIRank()
{
    int myrank = 0;
    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
    return myrank;
}


