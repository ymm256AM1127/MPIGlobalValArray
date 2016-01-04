#include <iostream>
#include <mpi.h>
#include <vector>
#include <QElapsedTimer>
#include <QDebug>

#include "test/Test.h"


int main(int argc, char *argv[])
{
    //! MPIの初期化
    MPIEnvPtr->Init( argc, argv );
    auto comm = MPIEnvPtr->CreateCommunicator( MPI_COMM_WORLD );

//    P2PTest( comm );
//    BcastTest( comm );
//    CorrectiveTest( comm );
//    CorrectiveAllTest( comm );

    WindowObjectTest( comm );

    return 0;
}

