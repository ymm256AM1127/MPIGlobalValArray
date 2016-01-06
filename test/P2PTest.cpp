#include "Test.h"

int P2PTest( _MYNAMESPACE_::MPI::Environment::CommPtr &comm )
{
    std::vector<double> bufferIn( 1 << 10 );
    std::vector<double> bufferOut( 1 << 10   );

    int mpisize = comm->GetMPISize();
    int src     = comm->GetMPIRank() - 1;
    int dst     = comm->GetMPIRank() + 1;

    if( src < 0 )
    {
        src = mpisize - 1;
    }

    if( dst >= mpisize )
    {
        dst = 0;
    }

    //!  P2P通信の確認(vector版)
    if( comm->GetMPIRank() == 0 )
    {
        comm->Send( bufferOut, 1 );
    }
    else if( comm->GetMPIRank() == 1 )
    {
        comm->Recv( bufferIn, 0 );
    }

    //!  非同期P2P通信の確認(vector版)
    if( comm->GetMPIRank() == 0 )
    {
        comm->Isend( bufferOut, 1 );
    }
    else if( comm->GetMPIRank() == 1 )
    {
        comm->Irecv( bufferIn, 0 );
    }

    comm->WaitAll();

    comm->Barrier();
    //!  P2P通信の確認(string版)
    std::string strout("This is send buffer");
    std::string strin;
    if( comm->GetMPIRank() == 0 )
    {
        comm->Isend( strout, 1 );
    }
    else if( comm->GetMPIRank() == 1 )
    {
        comm->Irecv( strin, 0 );
    }

    comm->WaitAll();

    std::cout << "rank" << comm->GetMPIRank() << " : " << strin << std::endl;

    comm->Barrier();

    return 0;
}
