#ifndef MPIVALARRAY_H
#define MPIVALARRAY_H

#include "GlobalValArray.h"

template < class T >
using MPIValArray = _MYNAMESPACE_::MPI::GlobalValArray< _MYNAMESPACE_::MPI::WindowObject< T > > ;

template < class T >
using ValArray = _MYNAMESPACE_::MPI::LocalValArray< T > ;


#endif // MPIVALARRAY_H

