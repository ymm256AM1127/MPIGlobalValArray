#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <numeric>
//#include "MPISharedVector.h"
#include <QElapsedTimer>
#include <memory.h>
#include <QDebug>

#include "include/MPI/Environment.h"
#include "include/MPI/Communicator.h"

const int ExecCount = 100;

int main(int argc, char *argv[])
{
    //! MPIの初期化
    MPIEnvPtr->Init( argc, argv );

    std::vector<double> bufferIn( 1 << 12 );
    std::vector<double> bufferOut( 1 << 12   );

    int myrank  = MPICommPtr->GetMPIRank();
    int mpisize = MPICommPtr->GetMPISize();
    int src     = MPICommPtr->GetMPIRank() - 1;
    int dst     = MPICommPtr->GetMPIRank() + 1;

    if( src < 0 )
    {
        src = mpisize - 1;
    }

    if( dst >= mpisize )
    {
        dst = 0;
    }

    //!  P2P通信の確認(vector版)
    if( MPICommPtr->GetMPIRank() == 0 )
    {
        MPICommPtr->Send( bufferOut, 1 );
    }
    else if( MPICommPtr->GetMPIRank() == 1 )
    {
        MPICommPtr->Recv( bufferIn, 0 );
    }

    //!  非同期P2P通信の確認(vector版)
    if( MPICommPtr->GetMPIRank() == 0 )
    {
        MPICommPtr->Isend( bufferOut, 1 );
    }
    else if( MPICommPtr->GetMPIRank() == 1 )
    {
        MPICommPtr->Irecv( bufferIn, 0 );
    }

    MPICommPtr->WaitAll();

    MPICommPtr->Barrier();
    //!  P2P通信の確認(string版)
    std::string strout("This is send buffer");
    std::string strin;
    if( MPICommPtr->GetMPIRank() == 0 )
    {
        MPICommPtr->Send( strout, 1 );
    }
    else if( MPICommPtr->GetMPIRank() == 1 )
    {
        MPICommPtr->Recv( strin, 0 );
        std::cout << strin << std::endl;
    }

    MPICommPtr->Barrier();

    //! 集団通信の確認(string)
    std::string strbcast;
    if( MPICommPtr->IsRootRank() )
    {
        strbcast = std::string( "This is from RootRank." );
    }

    MPICommPtr->Bcast( strbcast, MPICommPtr->GetRootRank() );

    for( auto ii = 0; ii < MPICommPtr->GetMPISize(); ii++ )
    {
        if( MPICommPtr->GetMPIRank() == ii )
        {
            std::cout << "[BCAST TEST] Rank: " << MPICommPtr->GetMPIRank() << " => " << strbcast << std::endl;
        }
        MPICommPtr->Barrier();
    }
    MPICommPtr->Barrier();

    //! 集団通信の確認(string)--Gather ===================================================================================
    std::stringstream ssgather;
    ssgather << "Rank";
    for( auto ii = 0; ii < MPICommPtr->GetMPIRank() + 1; ii++ )
    {
        ssgather << ii;
    }
//    ssgather << std::endl;

    MPICommPtr->Barrier();

    std::string gathersend = ssgather.str();
    std::string gatherrecv;

    MPICommPtr->Gather( gathersend, gatherrecv, MPICommPtr->GetRootRank() );

    for( auto ii = 0; ii < MPICommPtr->GetMPISize(); ii++ )
    {
        if( MPICommPtr->GetMPIRank() == ii )
        {
            std::cout << "[GATHER TEST] Rank: " << MPICommPtr->GetMPIRank() << " => " << gatherrecv << std::endl;
        }
        MPICommPtr->Barrier();
    }
    MPICommPtr->Barrier();

    //! 集団通信の確認(string)--Scatter ==================================================================================

    std::string strscatter;

    MPICommPtr->Scatter<std::string>( gatherrecv, strscatter, MPICommPtr->GetRootRank() );

    for( auto ii = 0; ii < MPICommPtr->GetMPISize(); ii++ )
    {
        if( MPICommPtr->GetMPIRank() == ii )
        {
            std::cout << "[SCATTER TEST] Rank: " << MPICommPtr->GetMPIRank() << " => " << strscatter << std::endl;
        }
        MPICommPtr->Barrier();
    }
    MPICommPtr->Barrier();

    //! 集団通信の確認(string)--AllGather ================================================================================
    std::stringstream ssallgather;
    ssallgather << "Rank" << MPICommPtr->GetMPIRank();
    for( auto ii = 0; ii < MPICommPtr->GetMPIRank() + 1; ii++ )
    {
        ssallgather << ii;
    }
//    ssallgather << std::endl;

    std::string allgathersend = ssallgather.str();
    std::string allgatherrecv;

    MPICommPtr->Barrier();

    MPICommPtr->AllGather<std::string>( allgathersend, allgatherrecv );

    for( auto ii = 0; ii < MPICommPtr->GetMPISize(); ii++ )
    {
        if( MPICommPtr->GetMPIRank() == ii )
        {
            std::cout << "[AllGATHER TEST] Rank: " << MPICommPtr->GetMPIRank() << " => \n" << allgatherrecv  << " size: " << allgatherrecv.size() << std::endl;
        }
        MPICommPtr->Barrier();
    }
    MPICommPtr->Barrier();

    //! 集団通信の確認(string)--Alltoall =================================================================================

    std::string stralltoall;

    MPICommPtr->Alltoall<std::string>( allgatherrecv, stralltoall );

    for( auto ii = 0; ii < MPICommPtr->GetMPISize(); ii++ )
    {
        if( MPICommPtr->GetMPIRank() == ii )
        {
            std::cout << "[ALLTOALL TEST] Rank: " << MPICommPtr->GetMPIRank() << " => " << stralltoall << std::endl;
        }
        MPICommPtr->Barrier();
    }
    MPICommPtr->Barrier();


    qint64 elapsed = 0;

    for( auto ii = 0; ii < ExecCount; ii++ )
    {
        QElapsedTimer timer;
        timer.start();

        MPICommPtr->Isend<double>( bufferOut[0], dst, bufferIn.size() );
        MPICommPtr->Irecv<double>( bufferIn[0], src, bufferOut.size() );
        MPICommPtr->WaitAll();

        elapsed += timer.elapsed();

        MPICommPtr->Barrier();
    }

    MPICommPtr->Barrier();

    qint64 totalelapsed1 = 0;
    MPICommPtr->Allreduce<_MYNAMESPACE_::MPI::SUM, qint64>( elapsed, totalelapsed1, 1);

    //! ラップしたMPI_Finalize()の呼び出し。
    MPIEnvPtr->Finalize();

    if( myrank == 0 )
    {
        std::cout << MPIEnvPtr->GetHostName() << " Time " << totalelapsed1 / mpisize << std::endl;
    }

    return 0;
}

