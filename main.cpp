#include <iostream>
#include <mpi.h>

#include "MPIWapperUtils.h"
#include "MPISharedVector.h"

int main(int argc, char *argv[])
{
    //! MPIの初期化
    MPI_Init( &argc, &argv );

//    MPISharedVector<double>     vec( 100, 0.0 );

    //! ラップしたMPI_Finalize()の呼び出し。
    MPIFinalize();

    return 0;
}

