#ifndef MPIWAPPERUTILS_H
#define MPIWAPPERUTILS_H

#include <mpi.h>
#include <unordered_map>

//! MPI Window�̃������Ǘ��pHashmap
extern std::unordered_map< void*, MPI_Win > MPIWinAllocMap;

/*!
 * \brief MPIFinalize \n
 * MPI�̓v���O�����I������MPI_Finalize()���ĂԕK�v�����邪�A\n
 * MPI_Finalize()���Ă΂ꂽ���MPI Window�֘A�̃�������\n
 * �������ƃG���[���o�邽�߁A���b�p�[��p�ӂ���B
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
