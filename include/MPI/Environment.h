#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../SharedLibraryDefine.h"
#include "../Utility/SingletonHolder.h"

#include <string>

namespace _MYNAMESPACE_
{
    namespace  MPI
    {
        class SHAREDLIBRARYDEFINE_EXPORT Environment
        {
        public:
            Environment();
            ~Environment();

            void                Init( int argc, char *argv[] );
            void                Finalize();
            void                Abort( const int error_code );
            bool                IsRootRank()  const;
            int                 GetRootRank() const;
            int                 GetMPIRank()  const;
            int                 GetMPISize()  const;

        private:
            int                 m_i32MPIRank;
            int                 m_i32MPISize;
            std::string         m_strHostName;
        };
        typedef SingletonHolder< Environment >  EnvironmentSingleton;
    }
}

#define MPIEnvPtr _MYNAMESPACE_::MPI::EnvironmentSingleton::GetInstance()

#endif // ENVIRONMENT_H
