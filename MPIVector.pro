QT += core
QT -= gui

TARGET = MPIVector
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    MPIWapperUtils.cpp


win32{
    MPIPATH = "C:/Program Files (x86)/Microsoft SDKs/MPI"
    MPILIB = $$MPIPATH/Lib/x64
    MPIINC = $$MPIPATH/Include
    LIBS += -L$$MPILIB -lmsmpi
}

<<<<<<< HEAD
unix:!macx{
=======
macx:!unix{
    MPIPATH = /opt/mpich
    MPILIB = $$MPIPATH/lib
    MPIINC = $$MPIPATH/include
    LIBS += -L$$MPILIB -lmpi
}

unix{
>>>>>>> 1c177d50ff5bf52b663ecf62ce6c311b5d4c4d30
    MPIPATH = /opt/mpich
    MPILIB = $$MPIPATH/lib
    MPIINC = $$MPIPATH/include
    LIBS += -L$$MPILIB -lmpi
}

macx{
    MPIPATH = /opt/mpich
    MPILIB = $$MPIPATH/lib
    MPIINC = $$MPIPATH/include
    LIBS += -L$$MPILIB -lmpi
}


INCLUDEPATH += $$MPIINC
DEPENDPATH  += $$MPIINC

HEADERS += \
    MPISharedAllocator.h \
    MPIWapperUtils.h \
    MPISharedVector.h


message( $$INCLUDEPATH )
