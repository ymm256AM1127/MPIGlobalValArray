#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <numeric>
#include "MPIWapperUtils.h"
#include "MPISharedVector.h"

int main(int argc, char *argv[])
{
    //! MPIの初期化
    MPI_Init( &argc, &argv );

    MPISharedVector<double>     vec( 100, 0.0 );

    if( MPIRank() == 0 )
    {
        vec[9] = 12.8;
        vec[10] = 0.9998;
    }

    MPI_Barrier( MPI_COMM_WORLD );

    if( MPIRank() == 1  )
    {
        double val = std::accumulate( vec.begin(), vec.end(), 0.0 );
        vec[0] = val;
    }

    std::cout << vec[0] << std::endl;

    //! ラップしたMPI_Finalize()の呼び出し。
    MPIFinalize();

    return 0;
}

