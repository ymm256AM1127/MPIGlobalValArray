#include <iostream>
#include <mpi.h>

#include "MPIWapperUtils.h"
#include "MPISharedVector.h"

int main(int argc, char *argv[])
{
    //! MPI�̏�����
    MPI_Init( &argc, &argv );

//    MPISharedVector<double>     vec( 100, 0.0 );

    //! ���b�v����MPI_Finalize()�̌Ăяo���B
    MPIFinalize();

    return 0;
}

