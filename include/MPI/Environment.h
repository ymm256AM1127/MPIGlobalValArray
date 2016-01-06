#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../SharedLibraryDefine.h"
#include "../Utility/SingletonHolder.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mpi.h>

static const int _MPI_NAME_LENGH_ = 1024;

namespace _MYNAMESPACE_
{
    namespace  MPI
    {
        class Communicator;
        class SHAREDLIBRARYDEFINE_EXPORT Environment
        {
        public:
            using CommPtr = std::shared_ptr<Communicator>;
            using MPIRankMap = std::unordered_map<int, std::string>;

            Environment();
            ~Environment();

            void                Init( int argc, char *argv[] );
            void                Finalize();
            void                Abort( const int error_code );
            std::string         GetHostName() const;
            bool                IsLocalNodeMaster() const;
            CommPtr             CreateCommunicator( const MPI_Comm comm );
            void                RemoveCommunicator( CommPtr comm );

        private:
            using CommAllocSet  = std::unordered_set< CommPtr >;
            std::string         m_strHostName;
            MPIRankMap          m_GlobalMPIRankMap;
            MPIRankMap          m_LocalMPIRankMap;
            bool                m_IsLocalNodeMaster;
            CommAllocSet        m_CommAllocSet;


            inline void         CreateGlobalNodeMap();
            inline void         CreateLocalNodeMap();
            inline void         CheckLocalNodeMaster();
        };
        typedef SingletonHolder< Environment >  EnvironmentSingleton;
    }
}

//! シングルトンでアクセス。
#define MPIEnvPtr _MYNAMESPACE_::MPI::EnvironmentSingleton::GetInstance()

#endif // ENVIRONMENT_H
