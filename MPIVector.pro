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

macx:!unix{
    MPIPATH = /opt/mpich
    MPILIB = $$MPIPATH/lib
    MPIINC = $$MPIPATH/include
    LIBS += -L$$MPILIB -lmpi
}

unix{
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
