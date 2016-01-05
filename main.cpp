#include <iostream>
#include <mpi.h>
#include <vector>
#include <QElapsedTimer>
#include <QDebug>
#include <valarray>

#include "test/Test.h"

#include "include/MPI/ValArray.h"

using namespace MyMPI::MPI;

int main(int argc, char *argv[])
{
    //! MPIの初期化
    MPIEnvPtr->Init( argc, argv );
    auto comm = MPIEnvPtr->CreateCommunicator( MPI_COMM_WORLD );

//    P2PTest( comm );
//    BcastTest( comm );
//    CorrectiveTest( comm );
//    CorrectiveAllTest( comm );

//    WindowObjectTest( comm );

    ValArray<double> array1( comm, 100, 0 );
    auto array2 = array1;
    auto array3 = array1;

    array1[ 20 ] = 10.0;
    array1[ 80 ] = 30.0;
    array2[ 20 ] = 1.0;
    array2[ 80 ] = 3.0;
    array3[ 20 ] = -13.0;
    array3[ 80 ] = -328.0;

    comm->Barrier();

    array1 = ( array1 + array2 + Cos( array3 ) );

    comm->Barrier();

    qDebug() << array1[ 20 ] << array1[ 80 ];

    return 0;
}

