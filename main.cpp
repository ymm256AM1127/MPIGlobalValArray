#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <numeric>
#include <vector>
//#include "MPISharedVector.h"
#include <QElapsedTimer>
#include <memory.h>
#include <QDebug>

#include "include/MPI/Environment.h"
#include "include/MPI/Communicator.h"
#include "include/MPI/WindowObject.h"

#include "test/Test.h"

//const int ExecCount = 100;

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

