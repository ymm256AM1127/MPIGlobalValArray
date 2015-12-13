#ifndef ERRORCODE_H
#define ERRORCODE_H

#include "../SharedLibraryDefine.h"

namespace _MYNAMESPACE_
{
    namespace MPI
    {
        typedef enum _ERRORCODE_
        {
            NOTSUPPORTTHREADMULTIPLE = -1,
            CANNOTINSERTVALTOMAP     = -2,
            P2PCOMMSIZEERROR         = -3,
            BROADCASTSIZEERROR       = -4,
            GATHERSIZEERROR          = -5
        } ERROR_CODE;
    }
}

#endif // ERRORCODE_H

