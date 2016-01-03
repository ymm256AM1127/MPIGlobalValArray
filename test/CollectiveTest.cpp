#ifndef COLLECTIVETESTGATHER_CPP
#define COLLECTIVETESTGATHER_CPP

#include "Test.h"

int CorrectiveTest( _MYNAMESPACE_::MPI::Environment::CommPtr &comm )
{
    //! 集団通信の確認(string)--Gather ===================================================================================
    std::stringstream ssgather;
    ssgather << "Rank";
    for( auto ii = 0; ii < comm->GetMPIRank() + 1; ii++ )
    {
        ssgather << ii;
    }
    ssgather << std::endl;

    comm->Barrier();

    std::string gathersend = ssgather.str();
    std::string gatherrecv;

    comm->Gather( gathersend, gatherrecv, comm->GetRootRank() );

    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
    {
        if( comm->GetMPIRank() == ii )
        {
            std::cout << "[GATHER TEST] Rank: " << comm->GetMPIRank() << " => " << gatherrecv << std::endl;
        }
        comm->Barrier();
    }
    comm->Barrier();

    //! 集団通信の確認(string)--Scatter ==================================================================================

    std::string strscatter;

    comm->Scatter<std::string>( gatherrecv, strscatter, comm->GetRootRank() );

    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
    {
        if( comm->GetMPIRank() == ii )
        {
            std::cout << "[SCATTER TEST] Rank: " << comm->GetMPIRank() << " => " << strscatter << std::endl;
        }
        comm->Barrier();
    }
    comm->Barrier();

    return 0;
}


#endif // COLLECTIVETESTGATHER_CPP
