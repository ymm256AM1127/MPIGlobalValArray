#include <mpi.h>

#if defined( _WIN32 ) || defined( _WIN64 )
#  pragma warning(disable:4251)
#endif

#include "MPI/MPIValArray.h"

int main(int argc, char *argv[])
{
    MPIEnvPtr->Init( argc, argv );
    auto comm = MPIEnvPtr->CreateCommunicator( MPI_COMM_WORLD );
    MPIValArray< double > array( comm, 10000, 0 );

    array[0] = 100.0;

    std::cout << "array[0] = " << array.at( 0 ) << "\n";

    return 0;
}

