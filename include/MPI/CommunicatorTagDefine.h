#ifndef COMMUNICATORTAGDEFINE_H
#define COMMUNICATORTAGDEFINE_H

#include "../SharedLibraryDefine.h"
#include <mpi.h>

namespace _MYNAMESPACE_
{
    using namespace MPL;
    namespace  MPI
    {
        enum class Tags : int
        {
            DefaultTag = 0,
            ANYTAG     = MPI_ANY_TAG,
            TAG1       = 1,
            TAG2       = 2,
            TAG3       = 3,
            TAG4       = 4,
            TAG5       = 5,
            TAG6       = 6,
            TAG7       = 7,
            TAG8       = 8,
            TAG9       = 9,
            TAG10      = 10,
            TAG11      = 11,
        };
    }
}

#endif // COMMUNICATORTAGDEFINE_H
