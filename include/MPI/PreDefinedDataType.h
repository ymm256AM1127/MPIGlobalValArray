#ifndef PREDEFINEDDATATYPE_H
#define PREDEFINEDDATATYPE_H

#include <vector>
#include "DataTypeDefine.h"


namespace _MYNAMESPACE_
{
    namespace MPI
    {
        _MPI_DATA_TYPE_SETTER_( char,                MPI_CHAR )
        _MPI_DATA_TYPE_SETTER_( short,               MPI_SHORT )
        _MPI_DATA_TYPE_SETTER_( int,                 MPI_INT )
        _MPI_DATA_TYPE_SETTER_( long long,           MPI_LONG_LONG )

        _MPI_DATA_TYPE_SETTER_( unsigned char,       MPI_UNSIGNED_CHAR )
        _MPI_DATA_TYPE_SETTER_( unsigned short,      MPI_UNSIGNED_SHORT )
        _MPI_DATA_TYPE_SETTER_( unsigned int,        MPI_UNSIGNED )
        _MPI_DATA_TYPE_SETTER_( unsigned long,       MPI_UNSIGNED_LONG )
        _MPI_DATA_TYPE_SETTER_( unsigned long long,  MPI_UNSIGNED_LONG_LONG )

        _MPI_DATA_TYPE_SETTER_( float,               MPI_FLOAT )
        _MPI_DATA_TYPE_SETTER_( double,              MPI_DOUBLE )

        _MPI_DATA_TYPE_SETTER_( std::vector<float> , MPI_FLOAT )
        _MPI_DATA_TYPE_SETTER_( std::vector<double>, MPI_DOUBLE )
    }
}

#endif // PREDEFINEDDATATYPE_H

#include "CustomDataType.h"
