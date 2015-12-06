#ifndef MPIWAPPERUTILS_H
#define MPIWAPPERUTILS_H

#include <mpi.h>
#include <unordered_map>

//! MPI Windowのメモリ管理用Hashmap
extern std::unordered_map< void*, MPI_Win > MPIWinAllocMap;

/*!
 * \brief MPIFinalize \n
 * MPIはプログラム終了時にMPI_Finalize()を呼ぶ必要があるが、\n
 * MPI_Finalize()が呼ばれた後にMPI Window関連のメモリを\n
 * 解放するとエラーが出るため、ラッパーを用意する。
 */
void        MPIFinalize();

/*!
 * \brief MPISize
 * \return Total number of MPI Ranks (int type)
 */
int         MPISize();
/*!
 * \brief MPIRank
 * \return my rank number (int type)
 */
int         MPIRank();

#endif // MPIWAPPERUTILS_H
