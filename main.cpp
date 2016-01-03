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

//    //! RMAの確認 =======================================================================================================
//{
//    _MYNAMESPACE_::MPI::WindowObject<double> rmaobj( comm, 10000, 0, "test" );
//    _MYNAMESPACE_::MPI::WindowObject<int> rmaobj1( comm, 10000, 0, "test1" );
//    _MYNAMESPACE_::MPI::WindowObject<double> rmaobj2( comm, 10000, 0, "test1" );
//    comm->Barrier();

//    double val;
//    val = 0.09;
////    qDebug() << rmaobj.GetWindowObjName().c_str();

////    if( comm->GetMPIRank() == 0 )
////    {
////        rmaobj.Write( 333.333, 50 );
////        rmaobj2.Write( 454.090, 9000 );
////        rmaobj.Write( 555.555, 8000 );

//        rmaobj[50] = 0.7777 *( comm->GetMPIRank() + 1.0);
////    }
//    comm->Barrier();

//    qDebug() <<  rmaobj[50] << rmaobj[ 8000 ]/* << MPIEnvPtr->GetHostName().c_str()*/;

//    rmaobj2 = rmaobj;

//    comm->Barrier();

//    qDebug() <<  rmaobj2[50] << rmaobj2[ 8000 ]/* << MPIEnvPtr->GetHostName().c_str()*/;
//}
////MPI_Barrier( MPI_COMM_WORLD );

////     double dval = 100.0 * (comm->GetMPIRank() + 1);
////     double darray[6000] = {0.0};
//////     rmaobj.Fence();
////     rmaobj.Write( dval, 9999 );
////     rmaobj.Write( 4.5, 9999 );
////     rmaobj.Write( darray, 0, 6000 );
//////     rmaobj.Fence();

//////    MPI_Barrier( MPI_COMM_WORLD );
//////    rmaobj.Fence();

//////    if( myrank == 1 )
//////    {
//////        auto dval = rmaobj.ReadOneValue( 9999 );
//////    }

////    for( auto ii = 0; ii < comm->GetMPISize(); ii++ )
////    {
////        if( comm->GetMPIRank() == ii )
////        {
////            std::cout << MPIEnvPtr->GetHostName() << " RMA Get " << rmaobj.Read( 9999 ) << std::endl;
////        }
////        comm->Barrier();
////    }
////    qint64 totalelapsed1 = 0;
////    comm->Allreduce<_MYNAMESPACE_::MPI::SUM, qint64>( elapsed, totalelapsed1, 1);

////    MPIEnvPtr->RemoveCommunicator( comm );

////    //! ラップしたMPI_Finalize()の呼び出し。
////    MPIEnvPtr->Finalize();

////    if( myrank == 0 )
////    {
////        std::cout << MPIEnvPtr->GetHostName() << " Time " << totalelapsed1 / mpisize << std::endl;
////    }

    return 0;
}

