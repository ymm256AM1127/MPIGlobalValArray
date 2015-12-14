#include "include/MPI/CustomDataType.h"

#include <iostream>

#ifdef DEBUG_MPI
    #define _DEBUG_MPI_RET(a) std::cout() << (a) << std::endl;
#else
    #define _DEBUG_MPI_RET(a) a
#endif

MPI_Datatype _MYNAMESPACE_::MPI::MPI_COMPLEX_FLOAT  = MPI_Datatype();
MPI_Datatype _MYNAMESPACE_::MPI::MPI_COMPLEX_DOUBLE = MPI_Datatype();

namespace
{
    void registNewDataType( int count , const MPI_Datatype &old_type, MPI_Datatype &new_type )
    {
        _DEBUG_MPI_RET( MPI_Type_contiguous( count, old_type, &new_type ) );
        _DEBUG_MPI_RET( MPI_Type_commit( &new_type ) );
    }
}

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        void CustomDataTypeRegister()
        {
            registNewDataType( 2, MPI_FLOAT,  _MYNAMESPACE_::MPI::MPI_COMPLEX_FLOAT );
            registNewDataType( 2, MPI_DOUBLE, _MYNAMESPACE_::MPI::MPI_COMPLEX_DOUBLE );
        }
    }
}
