#include "Test.h"

#include "../include/MPI/WindowObject.h"
#include <QDebug>

using namespace _MYNAMESPACE_::MPI;

int WindowObjectTest(  _MYNAMESPACE_::MPI::Environment::CommPtr &comm )
{
    comm->Barrier();

    std::size_t globalsize = 100;
    std::size_t localhalo  = 10;

    WindowObject<double> RMA01( comm, globalsize, localhalo, "Test01" );

    for( auto ii = 0UL; ii < RMA01.GetLocalSize(); ii++ )
    {
        RMA01.GetBasePtr()[ ii ] = static_cast<double>( ii * ( comm->GetMPIRank() + 1 ) );
    }

    qDebug() << MPIEnvPtr->GetHostName().c_str() << RMA01.GetLocalSize() << RMA01.GetGlobalSize();

    comm->Barrier();

    if( comm->IsRootRank() )
    {
        for( auto ii = 0UL; ii < RMA01.GetGlobalSize(); ii++ )
        {
            qDebug() << MPIEnvPtr->GetHostName().c_str() << " Index: " << ii << " value = " << RMA01[ii];
        }
    }

    return 0;
}
