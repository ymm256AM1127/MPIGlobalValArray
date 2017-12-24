#ifndef IMMEDIATERETURNTYPE_H
#define IMMEDIATERETURNTYPE_H

#include "../SharedLibraryDefine.h"
#include <mpi.h>

namespace _MYNAMESPACE_
{
    using namespace MPL;
    namespace  MPI
    {
        struct ImmediateRetType
        {
            ImmediateRetType() : request( MPI_REQUEST_NULL ), ReturnValue( MPI_SUCCESS )
            {

            }
            MPI_Request     request;
            MPI_Status      status;
            int             ReturnValue;
        };
    }
}

#endif // IMMEDIATERETURNTYPE_H
