#include "Test.h"

int CorrectiveAllTest( MyMPI::MPI::Environment::CommPtr &comm )
{
    //! 集団通信の確認(string)--AllGather ================================================================================
    std::stringstream ssallgather;
    ssallgather << "Rank" << comm->GetMPIRank();
    for( auto ii = 0; ii < comm->GetMPIRank() + 1; ii++ )
    {
        ssallgather << ii;
    }

    std::string allgathersend = ssallgather.str();
    std::string allgatherrecv;

    comm->Barrier();

    comm->AllGather<std::string>( allgathersend, allgatherrecv );

    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
    {
        if( comm->GetMPIRank() == ii )
        {
            std::cout << "[AllGATHER TEST] Rank: " << comm->GetMPIRank() << " => \n" << allgatherrecv  << " size: " << allgatherrecv.size() << std::endl;
        }
        comm->Barrier();
    }
    comm->Barrier();

    //! 集団通信の確認(string)--Alltoall =================================================================================

    std::string stralltoall;

    comm->Alltoall<std::string>( allgatherrecv, stralltoall );

    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
    {
        if( comm->GetMPIRank() == ii )
        {
            std::cout << "[ALLTOALL TEST] Rank: " << comm->GetMPIRank() << " => " << stralltoall << std::endl;
        }
        comm->Barrier();
    }
    comm->Barrier();

    return 0;
}
