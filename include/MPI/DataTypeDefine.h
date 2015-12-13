#ifndef DATATYPEDEFINE_H
#define DATATYPEDEFINE_H

#include "../SharedLibraryDefine.h"
#include <mpi.h>
#include "../MPL/is_complex.h"
#include "../MPL/is_pod.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        /*!
         * Base template MPIDATATYPE
         */
        template < class T > inline MPI_Datatype MPIDATATYPE() { return MPI_Datatype(0); }

        template < class T > inline MPI_Datatype MPIDataType()
        {
            return MPIDATATYPE< typename std::enable_if< MPL::is_pod_with_complex< T >::value, T >::type >();
        }

        typedef enum _MPIOPTYPE_
        {
            MIN,
            MAX,
            SUM,
            PROD,
            LAND,
            BAND,
            LOR,
            BOR,
            XOR,
            LXOR,
            BXOR,
            MINLOC,
            MAXLOC,
            REPLACE,
            MPIOPTYPESIZE
        } MPIOPTYPE ;

        #define _MPI_DATA_TYPE_SETTER_( datatype, mpi_datatype ) template <> \
        inline MPI_Datatype MPIDATATYPE< datatype >(){ return mpi_datatype; }

        /*!
         * Base template MPIOPERATIONTYPE
         */

        template < int OP >
        inline MPI_Op MPIOPERATIONTYPE(){ return MPI_SUM; }

        template < typename T, int OP >
        inline MPI_Op MPIOpType()
        {
            return MPIOPERATIONTYPE<OP>();
        }

        #define _MPI_OP_TYPE_SETTER_( Optype, mpi_Optype ) template <> \
        inline MPI_Op MPIOPERATIONTYPE< Optype >(){ return mpi_Optype; }

        typedef enum _MPI_BOOL_
        {
            FALSE = 0,
            TRUE  = 1
        } MPI_BOOL;

        typedef enum _ASSERT_
        {
            DEFAULTASSERT = 0
        }ASSERT;

    }
}


#endif // DATATYPEDEFINE_H
