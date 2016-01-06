#ifndef OPERATIONTYPE_H
#define OPERATIONTYPE_H

#include <mpi.h>
#include "DataTypeDefine.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        _MPI_OP_TYPE_SETTER_( MIN,               MPI_MIN )
        _MPI_OP_TYPE_SETTER_( MAX,               MPI_MAX )
        _MPI_OP_TYPE_SETTER_( SUM,               MPI_SUM )
    }
}

#endif // OPERATIONTYPE_H

#include "CustomOperationType.h"
