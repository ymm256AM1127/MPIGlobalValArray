#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <numeric>
#include "MPISharedVector.h"
#include <QElapsedTimer>
#include <memory.h>

#include "include/MPI/Environment.h"
#include "include/MPI/Communicator.h"

const int ExecCount = 100;

int main(int argc, char *argv[])
{
    //! MPIの初期化
    MPIEnvPtr->Init( argc, argv );

    std::vector<double> bufferIn( 1 << 22 );
    std::vector<double> bufferOut( 1 << 22   );

    int myrank  = MPIEnvPtr->GetMPIRank();
    int mpisize = MPIEnvPtr->GetMPISize();
    int src     = MPIEnvPtr->GetMPIRank() - 1;
    int dst     = MPIEnvPtr->GetMPIRank() + 1;

    if( src < 0 )
    {
        src = mpisize - 1;
    }

    if( dst >= mpisize )
    {
        dst = 0;
    }

    qint64 elapsed = 0;
    MPICommPtr->Barrier();

    for( auto ii = 0; ii < ExecCount; ii++ )
    {
        QElapsedTimer timer;
        timer.start();

        MPICommPtr->Isend<double>( bufferOut[0], dst, bufferIn.size() );
        MPICommPtr->Irecv<double>( bufferIn[0], src, bufferOut.size() );
        MPICommPtr->WaitAll();

        elapsed += timer.elapsed();

        MPICommPtr->Barrier();
    }

    MPICommPtr->Barrier();

    qint64 totalelapsed1 = 0;
    MPICommPtr->Allreduce<_MYNAMESPACE_::MPI::SUM, qint64>( elapsed, totalelapsed1, 1);

    //! ラップしたMPI_Finalize()の呼び出し。
    MPIEnvPtr->Finalize();

    if( myrank == 0 )
    {
        std::cout << MPIEnvPtr->GetHostName() << " Time " << totalelapsed1 / mpisize << std::endl;
    }

    return 0;
}

