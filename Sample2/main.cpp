#include <mpi.h>

#if defined( _WIN32 ) || defined( _WIN64 )
#  pragma warning(disable:4251)
#endif

#include "MPI/Environment.h"
#include "MPI/MPIValArray.h"
#include <thread>
#include <chrono>
#include <QDebug>
#include <string>
#include <QString>

int main(int argc, char *argv[])
{
    MPIEnvPtr->Init( argc, argv );
    auto comm = MPIEnvPtr->CreateCommunicator( MPI_COMM_WORLD );

    qDebug() << "MY Rank" << comm->GetMPIRank();

    std::string message;

    if( comm->GetMPIRank() == 0 )
    {
        message = "This is test a message.";

        auto ret = comm->Isend( message, 1  );
        comm->MPI_Requests().push_back( ret.request );
    }
    else if( comm->GetMPIRank() == 1 )
    {
        auto ret = comm->Irecv( message, 0 );
        comm->MPI_Requests().push_back( ret.request );
    }

    if( comm->GetMPIRank() == 0 || comm->GetMPIRank() == 1 )
    {
        comm->WaitAll();
    }

    qDebug() << comm->GetMPIRank() << QString::fromStdString( message );

    comm->Barrier();

    MPIEnvPtr->Finalize();

    return 0;
}

