#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <numeric>
#include "MPIWapperUtils.h"
#include "MPISharedVector.h"
#include <QElapsedTimer>

const int ExecCount = 100;

int main(int argc, char *argv[])
{
    //! MPIの初期化
    MPI_Init( &argc, &argv );

    std::vector<double> bufferIn( 1 << 20 );
    std::vector<double> bufferOut( 1 << 20 );

    std::cout << "rank " << MPIRank() << " " << bufferIn.size() << std::endl;

    int src = MPIRank() - 1;
    int dst = MPIRank() + 1;

    if( src < 0 )
    {
        src = MPISize() - 1;
    }

    if( dst >= MPISize() )
    {
        dst = 0;
    }

    MPI_Request request[2];
    qint64 elapsed = 0;

    for( auto ii = 0; ii < ExecCount; ii++ )
    {
        QElapsedTimer timer;
        timer.start();

        MPI_Isend( bufferOut.data(), bufferIn.size(),  MPI_DOUBLE, dst, 0, MPI_COMM_WORLD, &request[0] );
        MPI_Irecv( bufferIn.data() , bufferOut.size(), MPI_DOUBLE, src, 0, MPI_COMM_WORLD, &request[1] );

        MPI_Waitall( sizeof( request ), request, MPI_STATUSES_IGNORE );

        elapsed += timer.elapsed();

        MPI_Barrier( MPI_COMM_WORLD );
    }

    MPI_Barrier( MPI_COMM_WORLD );

    std::cout << "Rank " << MPIRank() << ": " << elapsed << std::endl;


//    MPISharedVector<double> vecIn( (1 << 20) * MPISize() );
//    MPISharedVector<double> vecOut( (1 << 20) * MPISize() );

    //! ラップしたMPI_Finalize()の呼び出し。
    MPIFinalize();

    return 0;
}

