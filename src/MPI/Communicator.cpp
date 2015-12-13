#include "../../include/MPI/Communicator.h"
#include "../../include/Utility/ScopedMutex.h"

_MYNAMESPACE_::MPI::Communicator::Communicator() : m_i32Tag( 0 ), m_Commnunicator( MPI_COMM_WORLD )
{

}

_MYNAMESPACE_::MPI::Communicator::~Communicator()
{

}

/*!
 * \brief _MYNAMESPACE_::MPI::Communicator::Barrier
 */
void _MYNAMESPACE_::MPI::Communicator::Barrier() const
{
    MPI_Barrier( m_Commnunicator );
}

/*!
 * \brief _MYNAMESPACE_::MPI::Communicator::SetCommunicator
 * \param comm
 */
void _MYNAMESPACE_::MPI::Communicator::SetCommunicator(const MPI_Comm &comm)
{
    m_Commnunicator = comm;
}

/*!
 * \brief MyMPI::MPI::Communicator::SetTag
 * \param tag
 */
void _MYNAMESPACE_::MPI::Communicator::SetTag(const int tag)
{
    m_i32Tag = tag;
}

/*!
 * \brief _MYNAMESPACE_::MPI::Communicator::WaitAll
 */
void _MYNAMESPACE_::MPI::Communicator::WaitAll()
{
    Utility::ScopedMutex<std::mutex> locker( &m_Mutex );
    MPI_Waitall( m_vectMPI_Requests.size(), m_vectMPI_Requests.data(), MPI_STATUSES_IGNORE );
    std::vector< MPI_Request > newRequest;
    m_vectMPI_Requests.swap( newRequest );
}

