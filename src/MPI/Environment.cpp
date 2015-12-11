#include "../../include/MPI/Environment.h"

#include <mpi.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "../../include/MPI/ErrorCode.h"

//#define _MPI_MULTI_THREAD_ENABLE_


_MYNAMESPACE_::MPI::Environment::Environment()
{
    m_i32MPIRank                  = 0;
    m_i32MPISize                  = 0;
}

_MYNAMESPACE_::MPI::Environment::~Environment()
{

}

void _MYNAMESPACE_::MPI::Environment::Init(int argc, char *argv[])
{
#ifndef _MPI_MULTI_THREAD_ENABLE_
    MPI_Init( &argc, &argv );
#else
    int provided = 0;
    MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE, &provided );
    if( provided != MPI_SUCCESS )
    {
        std::stringstream ss;
        ss << "Error Code: " << NOTSUPPORTTHREADMULTIPLE << " => it does not definde MPI Thread Multiple." << std::endl;
        throw std::domain_error( ss.str() );
        Finalize();
        Abort( NOTSUPPORTTHREADMULTIPLE );
    }
#endif
    MPI_Comm_rank( MPI_COMM_WORLD, &m_i32MPIRank );
    MPI_Comm_size( MPI_COMM_WORLD, &m_i32MPISize );
}

void _MYNAMESPACE_::MPI::Environment::Finalize()
{
    MPI_Finalize();
}

void _MYNAMESPACE_::MPI::Environment::Abort(const int error_code)
{
    MPI_Abort( MPI_COMM_WORLD, error_code );
}

bool _MYNAMESPACE_::MPI::Environment::IsRootRank() const
{
    return ( m_i32MPIRank == 0 );
}

int _MYNAMESPACE_::MPI::Environment::GetRootRank() const
{
    return 0;
}

int _MYNAMESPACE_::MPI::Environment::GetMPIRank() const
{
    return m_i32MPIRank;
}

int _MYNAMESPACE_::MPI::Environment::GetMPISize() const
{
    return m_i32MPISize;
}
