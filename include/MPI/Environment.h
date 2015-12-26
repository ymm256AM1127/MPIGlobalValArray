#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../SharedLibraryDefine.h"
#include "../Utility/SingletonHolder.h"

#include <string>
#include <unordered_map>
#include <mpi.h>

static const int _MPI_NAME_LENGH_ = 1024;

namespace _MYNAMESPACE_
{
    namespace  MPI
    {
        class SHAREDLIBRARYDEFINE_EXPORT Environment
        {
        public:
            using MPIRankMap = std::unordered_map<int, std::string>;

            Environment();
            ~Environment();

            void                Init( int argc, char *argv[] );
            void                Finalize();
            void                Abort( const int error_code );
            std::string         GetHostName() const;
            bool                IsLocalNodeMaster() const;

        private:
            std::string         m_strHostName;
            MPIRankMap          m_GlobalMPIRankMap;
            MPIRankMap          m_LocalMPIRankMap;
            bool                m_IsLocalNodeMaster;

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
