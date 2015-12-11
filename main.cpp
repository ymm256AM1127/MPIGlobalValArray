#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <numeric>
#include "MPISharedVector.h"
#include <QElapsedTimer>
#include <memory.h>

#include "include/MPI/Environment.h"

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

    MPI_Request request[2];
    qint64 elapsed = 0;
    MPI_Barrier( MPI_COMM_WORLD );

    for( auto ii = 0; ii < ExecCount; ii++ )
    {
        QElapsedTimer timer;
        timer.start();

        MPI_Isend( bufferOut.data(), bufferIn.size(),  MPI_DOUBLE, dst, 0, MPI_COMM_WORLD, &request[0] );
        MPI_Irecv( bufferIn.data() , bufferOut.size(), MPI_DOUBLE, src, 0, MPI_COMM_WORLD, &request[1] );

        MPI_Waitall( sizeof( request ) / sizeof( MPI_Request), request, MPI_STATUSES_IGNORE );

        elapsed += timer.elapsed();

        MPI_Barrier( MPI_COMM_WORLD );
    }

    MPI_Barrier( MPI_COMM_WORLD );

    qint64 totalelapsed1 = 0;
    MPI_Allreduce( &elapsed, &totalelapsed1, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD );



//    elapsed = 0;
//    MPISharedVector<double> vecIn( (1 << 22) * MPISize() );
//    MPISharedVector<double> vecOut( (1 << 22) * MPISize() );

//    for( auto ii = 0; ii < ExecCount; ii++ )
//    {
//        QElapsedTimer timer;
//        timer.start();

//        //! ここにSHMのコードを書く。

//        elapsed += timer.elapsed();

//        MPI_Barrier( MPI_COMM_WORLD );
//    }


    //! ラップしたMPI_Finalize()の呼び出し。
    MPIEnvPtr->Finalize();

    if( myrank == 0 )
    {
        std::cout << "Time " << totalelapsed1 / mpisize << std::endl;
    }

    return 0;
}

