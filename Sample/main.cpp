#include <mpi.h>

#if defined( _WIN32 ) || defined( _WIN64 )
#  pragma warning(disable:4251)
#endif

#include "MPI/Environment.h"
#include "MPI/MPIValArray.h"
#include <thread>
#include <chrono>

int main(int argc, char *argv[])
{
    MPIEnvPtr->Init( argc, argv );
    auto comm = MPIEnvPtr->CreateCommunicator( MPI_COMM_WORLD );
    MPIValArray< double > array( comm, 10000, 0 );

    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
    {
        if( comm->GetMPIRank() == ii )
        {
            std::cout << "Rank" << comm->GetMPIRank() << " => Global Array Count: " << array.GetGlobalSize() << " | Local Array Count: " << array.GetLocalSize() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(150));
        comm->Barrier();
    }

    comm->Barrier();

    if( comm->IsRootRank() )
    {
        std::cout << "Root Rank writes 100.0 to array at index " << array.GetLocalSize() << "\n";
        array[ array.GetLocalSize() ] = 100.0;
    }
    comm->Barrier();

    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
    {
        if( comm->GetMPIRank() == ii )
        {
            std::cout << "Rank" << comm->GetMPIRank()
                      << " => Global Array: array[ " << array.GetLocalSize() << " ] => " << array[ array.GetLocalSize() ]
                      << " => Local Array: array[ " << 0 << " ] => " << array.GetLocalValArray()[ 0 ] << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(150));
        comm->Barrier();
    }

    if( comm->IsRootRank() )
    {
        std::cout << "Root Rank writes 300.0 to array at index " << 10 << "\n";
        array[ 10 ] = 300.0;
    }
    comm->Barrier();

    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
    {
        if( comm->GetMPIRank() == ii )
        {
            std::cout << "Rank" << comm->GetMPIRank()
                      << " => Global Array: array[ " << 10 << " ] => " << array[ 10 ]
                      << " => Local Array: array[ " << 10 << " ] => " << array.GetLocalValArray()[ 10 ] << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(150));
        comm->Barrier();
    }

    if( comm->IsRootRank() )
    {
        std::cout << "Root Rank writes -700.0 to array at index " << array.GetGlobalSize() - 10 << "\n";
        array[ array.GetGlobalSize() -10 ] = -700.0;
    }
    comm->Barrier();

    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
    {
        if( comm->GetMPIRank() == ii )
        {
            std::cout << "Rank" << comm->GetMPIRank()
                      << " => Global Array: array[ " << array.GetGlobalSize() - 10 << " ] => " << array[ array.GetGlobalSize() - 10 ]
                      << " => Local Array: array[ " << array.GetLocalSize() - 10 << " ] => " << array.GetLocalValArray()[ array.GetLocalSize() - 10 ] << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(150));
        comm->Barrier();
    }

    comm->Barrier();

    auto norm = array.norm();
    auto localnorm = array.GetLocalValArray().norm();

    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
    {
        if( comm->GetMPIRank() == ii )
        {
            std::cout << "Rank" << comm->GetMPIRank() << " => Norm  => Global Array:  " << norm << " => Local Array: " << localnorm << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(150));
        comm->Barrier();
    }
    comm->Barrier();

    return 0;
}

