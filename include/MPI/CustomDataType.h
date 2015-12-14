#ifndef CUSTOMDATATYPE_H
#define CUSTOMDATATYPE_H

#include <mpi.h>
#include "DataTypeDefine.h"
#include <complex>

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        // Custom Data Type(s)
        extern MPI_Datatype MPI_COMPLEX_FLOAT;
        extern MPI_Datatype MPI_COMPLEX_DOUBLE;

        _MPI_DATA_TYPE_SETTER_( std::complex<float> , MPI_COMPLEX_FLOAT )
        _MPI_DATA_TYPE_SETTER_( std::complex<double>, MPI_COMPLEX_DOUBLE )

        void CustomDataTypeRegister();
    }
}

#endif // CUSTOMDATATYPE_H
