

win32{
    DEFINES += _WIN64
    MPIPATH = "C:/Program Files (x86)/Microsoft SDKs/MPI"
    MPILIB = $$MPIPATH/Lib/x64
    MPIINC = $$MPIPATH/Include
    LIBS += -L$$MPILIB -lmsmpi
}

macx:!unix{
    MPIPATH = /opt/mpi
    MPILIB = $$MPIPATH/lib
    MPIINC = $$MPIPATH/include
    LIBS += -L$$MPILIB -lmpi
}

unix{
    MPIPATH = /opt/mpi/openmpi-2.1.2.gcc-6
    MPILIB  = $$MPIPATH/lib
    MPIINC  = $$MPIPATH/include
    LIBS   += -L$$MPILIB -lmpi
}

INCLUDEPATH += $$MPIINC
DEPENDPATH  += $$MPIINC

