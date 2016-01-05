#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "../SharedLibraryDefine.h"
#include "../Utility/SingletonHolder.h"

#include <string>
#include <vector>
#include <set>
#include <mpi.h>
#include "CommunicatorTraits.h"
#include <mutex>
#include <thread>
#include <functional>
#include "Environment.h"
//#include "WindowObject.h"

namespace _MYNAMESPACE_
{
    using namespace MPL;
    namespace  MPI
    {
        class  SHAREDLIBRARYDEFINE_EXPORT Communicator
        {
        public:
            explicit Communicator( const MPI_Comm comm );
            ~Communicator();

            void                                SetTag( const int tag );
            void                                WaitAll();

            //! 以下はヘッダーに定義しているので、インライン化が必要
            inline void                         Barrier() const;
            inline bool                         IsRootRank()  const;
            inline int                          GetRootRank() const;
            inline int                          GetMPIRank()  const;
            inline int                          GetMPISize()  const;
            inline int                          GetTag() const;
            inline MPI_Comm                     GetCommunicator();
            inline std::vector<MPI_Request>&    MPI_Requests();
            inline std::mutex&                  Mutex();
            inline std::vector<std::thread>&    ThreadPool();

            /*!
             * \brief point to point communication =====================================================================
             */
            template< class T > int Send     ( const T &dataSend, const int dest,    const int count = 0 );
            template< class T > int Recv     ( T &dataRecv,       const int source,  const int count = 0 );
            template< class T > int Isend    ( const T &dataSend, const int dest,    const int count = 0 );
            template< class T > int Irecv    ( T &dataRecv,       const int source,  const int count = 0 );

            /*!
             * \brief corrective communication =========================================================================
             */
            template< class T > int Bcast    ( T &dataBufer,      const int srcRank, const int itemCount = 0 );
            template< class T > int Gather   ( const T &SendBuffer, T &RecvBuffer,
                                               const int rootRank, const int itemCount = 0 );
            template< class T > int Scatter  ( const T &SendBuffer, T &RecvBuffer,
                                               const int rootRank, const int itemCount = 0 );
            template< class T > int AllGather( const T &SendBuffer,
                                               T &RecvBuffer, const int itemCount = 0 );
            template< class T > int Alltoall ( const T &SendBuffer,
                                               T &RecvBuffer, const int itemCount = 0 );

            template< class T > int Reduce   ( const typename reducible_type<T>::type &SendBuffer,
                                               typename reducible_type<T>::type &RecvBuffer,
                                               MPI_Op Op,
                                               const int rootRank, const int itemCount = 0 );

            template< class T > int Allreduce( const typename reducible_type<T>::type &SendBuffer,
                                               typename reducible_type<T>::type &RecvBuffer,
                                               MPI_Op Op,
                                               const int itemCount = 0 );

            using WinObsSetType = std::set< std::function< void() >* >;



            int                         RegisterWinObjSet( const std::function<void()>* pfunc );
            int                         EraseWinObjSet( const std::function<void()>* pfunc );

        private:
            int                         m_i32Tag;
            MPI_Comm                    m_Commnunicator;
            std::vector<MPI_Request>    m_vectMPI_Requests;
            std::mutex                  m_Mutex;
            std::vector<std::thread>    m_vectThreadPool;
            int                         m_i32MPIRank;
            int                         m_i32MPISize;
            WinObsSetType               m_WindowObjSet;
        };
    }
}
#endif // COMMUNICATOR_H

#include "detail/CommunicatorImpl.h"

