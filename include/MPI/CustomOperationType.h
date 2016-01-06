#ifndef CUSTOMOPERATIONTYPE_H
#define CUSTOMOPERATIONTYPE_H

#include <mpi.h>
#include "DataTypeDefine.h"
#include <complex>

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        typedef enum _MPIOPTYPE_EXTENSION_
        {
            COMPLEX_FLOAT_SUM = MPIOPTYPESIZE + 1,
            COMPLEX_DOUBLE_SUM
        } MPIOPTYPE_EXETENSION ;

        // Custom Operation Type(s)
        extern MPI_Op MPI_COMPLEX_FLOAT_SUM;
        extern MPI_Op MPI_COMPLEX_DOUBLE_SUM;

        _MPI_OP_TYPE_SETTER_( COMPLEX_FLOAT_SUM,  MPI_COMPLEX_FLOAT_SUM )
        _MPI_OP_TYPE_SETTER_( COMPLEX_DOUBLE_SUM, MPI_COMPLEX_DOUBLE_SUM )

        template <>
        inline MPI_Op MPIOpType< std::complex<float>, COMPLEX_FLOAT_SUM >()
        {
            return MPIOPERATIONTYPE<COMPLEX_FLOAT_SUM>();
        }

        template <>
        inline MPI_Op MPIOpType< std::complex<double>, COMPLEX_DOUBLE_SUM >()
        {
            return MPIOPERATIONTYPE<COMPLEX_DOUBLE_SUM>();
        }

        void CustomOperationTypeRegister();
    }
}

#endif // CUSTOMOPERATIONTYPE_H
