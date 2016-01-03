#include "Test.h"

int BcastTest( _MYNAMESPACE_::MPI::Environment::CommPtr &comm )
{
    //! 集団通信の確認(string)
    std::string strbcast;
    if( comm->IsRootRank() )
    {
        strbcast = std::string( "This is from RootRank." );
    }

    comm->Bcast( strbcast, comm->GetRootRank() );

    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
    {
        if( comm->GetMPIRank() == ii )
        {
            std::cout << "[BCAST TEST] Rank: " << comm->GetMPIRank() << " => " << strbcast << std::endl;
        }
        comm->Barrier();
    }
    comm->Barrier();

    return 0;
}
