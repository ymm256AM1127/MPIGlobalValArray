#ifndef P2PTEST_H
#define P2PTEST_H

#include "include/MPI/Environment.h"
#include "include/MPI/Communicator.h"

int P2PTest( _MYNAMESPACE_::MPI::Environment::CommPtr &comm );
int BcastTest( _MYNAMESPACE_::MPI::Environment::CommPtr &comm );
int CorrectiveTest( _MYNAMESPACE_::MPI::Environment::CommPtr &comm );
int CorrectiveAllTest( _MYNAMESPACE_::MPI::Environment::CommPtr &comm );
int WindowObjectTest(  _MYNAMESPACE_::MPI::Environment::CommPtr &comm );

#endif // P2PTEST_H
