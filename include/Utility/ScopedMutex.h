#ifndef SCOPEDMUTEX_H
#define SCOPEDMUTEX_H

#include <mutex>
#include "../SharedLibraryDefine.h"

namespace _MYNAMESPACE_
{
    namespace Utility
    {
        template< class T >
        /*!
         * \brief The ScopedMutex class \n
         */
        class ScopedMutex
        {
        public:
            ScopedMutex( T *pmutex )
            {
                m_pMutex = pmutex;
                m_pMutex->lock();
            }
            ~ScopedMutex()
            {
                m_pMutex->unlock();
            }
        private:
            T  *m_pMutex;
        };
    }
}

#endif // SCOPEDMUTEX_H
