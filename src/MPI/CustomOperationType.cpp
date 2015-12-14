#include "include/MPI/CustomOperationType.h"
#include "include/MPI/PreDefinedDataType.h"
#include "include/MPI/CustomDataType.h"

#include <iostream>

#ifdef DEBUG_MPI
    #define _DEBUG_MPI_RET(a) qDebug() << (a)
#else
    #define _DEBUG_MPI_RET(a) a
#endif

MPI_Op _MYNAMESPACE_::MPI::MPI_COMPLEX_FLOAT_SUM ;
MPI_Op _MYNAMESPACE_::MPI::MPI_COMPLEX_DOUBLE_SUM ;
MPI_User_function *MPI_USER_FUNC_COMPLEX_FLOAT;

namespace
{
    //add the user-defined function here
    template < typename T >
    void STDComplexSum( void *in, void *inout, int *len, MPI_Datatype *dptr )
    {
        T *pin    = static_cast<T*>(in);
        T *pinout = static_cast<T*>(inout);
        for( int ii = 0; ii < *len; ii++ )
        {
            pinout[ii] += pin[ii];

        }
    }

    /*!
     * \brief registNewOperationDataType
     * \param function
     * \param commute
     * \param op
     */
    void registNewOperationDataType( MPI_User_function* function, int commute, MPI_Op *op )
    {
        _DEBUG_MPI_RET( MPI_Op_create( function, commute, op ) );
    }

    // specialization
    template void STDComplexSum< std::complex<float>  >( void *in, void *inout, int *len, MPI_Datatype *dptr );
    template void STDComplexSum< std::complex<double> >( void *in, void *inout, int *len, MPI_Datatype *dptr );

}

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        void CustomOperationTypeRegister()
        {
            registNewOperationDataType( STDComplexSum< std::complex<float>  >, true, &MPI_COMPLEX_FLOAT_SUM );
            registNewOperationDataType( STDComplexSum< std::complex<double> >, true, &MPI_COMPLEX_DOUBLE_SUM );
        }
    }
}


