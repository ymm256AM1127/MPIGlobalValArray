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
#include "include/MPI/WindowObject.h"

const int ExecCount = 100;

int main(int argc, char *argv[])
{
    //! MPIの初期化
    MPIEnvPtr->Init( argc, argv );
//    _MYNAMESPACE_::MPI::Communicator comm( MPIEnvPtr, MPI_COMM_WORLD, 0 );
    auto comm = MPIEnvPtr->CreateCommunicator( MPI_COMM_WORLD );

    std::vector<double> bufferIn( 1 << 10 );
    std::vector<double> bufferOut( 1 << 10   );

    int myrank  = comm->GetMPIRank();
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
        comm->Send( strout, 1 );
    }
    else if( comm->GetMPIRank() == 1 )
    {
        comm->Recv( strin, 0 );
        std::cout << strin << std::endl;
    }

    comm->Barrier();

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

    //! 集団通信の確認(string)--Gather ===================================================================================
    std::stringstream ssgather;
    ssgather << "Rank";
    for( auto ii = 0; ii < comm->GetMPIRank() + 1; ii++ )
    {
        ssgather << ii;
    }
//    ssgather << std::endl;

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

    //! 集団通信の確認(string)--AllGather ================================================================================
    std::stringstream ssallgather;
    ssallgather << "Rank" << comm->GetMPIRank();
    for( auto ii = 0; ii < comm->GetMPIRank() + 1; ii++ )
    {
        ssallgather << ii;
    }
//    ssallgather << std::endl;

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


    qint64 elapsed = 0;

    for( auto ii = 0; ii < ExecCount; ii++ )
    {
        QElapsedTimer timer;
        timer.start();

        comm->Isend<double>( bufferOut[0], dst, bufferIn.size() );
        comm->Irecv<double>( bufferIn[0], src, bufferOut.size() );
        comm->WaitAll();

        elapsed += timer.elapsed();

        comm->Barrier();
    }

    comm->Barrier();

    //! RMAの確認 =======================================================================================================
{
    _MYNAMESPACE_::MPI::WindowObject<double> rmaobj( comm, 10000, 0, "test" );
    _MYNAMESPACE_::MPI::WindowObject<int> rmaobj1( comm, 10000, 0, "test1" );
    _MYNAMESPACE_::MPI::WindowObject<double> rmaobj2( comm, 10000, 0, "test1" );
    comm->Barrier();

    double val;
    val = 0.09;
//    qDebug() << rmaobj.GetWindowObjName().c_str();

//    if( comm->GetMPIRank() == 0 )
//    {
//        rmaobj.Write( 333.333, 50 );
//        rmaobj2.Write( 454.090, 9000 );
//        rmaobj.Write( 555.555, 8000 );

        rmaobj[50] = 0.7777 *( comm->GetMPIRank() + 1.0);
//    }
    comm->Barrier();

    qDebug() <<  rmaobj[50] << rmaobj[ 8000 ]/* << MPIEnvPtr->GetHostName().c_str()*/;

    rmaobj2 = rmaobj;

    comm->Barrier();

    qDebug() <<  rmaobj2[50] << rmaobj2[ 8000 ]/* << MPIEnvPtr->GetHostName().c_str()*/;
}
//MPI_Barrier( MPI_COMM_WORLD );

//     double dval = 100.0 * (comm->GetMPIRank() + 1);
//     double darray[6000] = {0.0};
////     rmaobj.Fence();
//     rmaobj.Write( dval, 9999 );
//     rmaobj.Write( 4.5, 9999 );
//     rmaobj.Write( darray, 0, 6000 );
////     rmaobj.Fence();

////    MPI_Barrier( MPI_COMM_WORLD );
////    rmaobj.Fence();

////    if( myrank == 1 )
////    {
////        auto dval = rmaobj.ReadOneValue( 9999 );
////    }

//    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
//    {
//        if( comm->GetMPIRank() == ii )
//        {
//            std::cout << MPIEnvPtr->GetHostName() << " RMA Get " << rmaobj.Read( 9999 ) << std::endl;
//        }
//        comm->Barrier();
//    }
//    qint64 totalelapsed1 = 0;
//    comm->Allreduce<_MYNAMESPACE_::MPI::SUM, qint64>( elapsed, totalelapsed1, 1);

//    MPIEnvPtr->RemoveCommunicator( comm );

//    //! ラップしたMPI_Finalize()の呼び出し。
//    MPIEnvPtr->Finalize();

//    if( myrank == 0 )
//    {
//        std::cout << MPIEnvPtr->GetHostName() << " Time " << totalelapsed1 / mpisize << std::endl;
//    }

    return 0;
}

