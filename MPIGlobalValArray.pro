QT += core
QT -= gui

TARGET = MPIGlobalValArray
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    src/MPI/Environment.cpp \
    src/MPI/Communicator.cpp \
    src/MPI/CustomDataType.cpp \
    src/MPI/CustomOperationType.cpp \
    test/P2PTest.cpp \
    test/BcastTest.cpp \
    test/CollectiveTest.cpp \
    test/CorrectiveAllTest.cpp \
    test/WindowObjectTest.cpp


win32{
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
    MPIPATH = /opt/mpi
    MPILIB = $$MPIPATH/lib
    MPIINC = $$MPIPATH/include
    LIBS += -L$$MPILIB -lmpi
}



INCLUDEPATH += $$MPIINC
DEPENDPATH  += $$MPIINC

HEADERS += \
    include/SharedLibraryDefine.h \
    include/MPI/Environment.h \
    include/Utility/SingletonHolder.h \
    include/MPI/ErrorCode.h \
    include/MPI/Communicator.h \
    include/MPL/is_complex.h \
    include/MPL/is_container.h \
    include/MPL/is_pod.h \
    include/MPL/is_string.h \
    include/MPL/remove_modifier.h \
    include/MPL/meta_function.h \
    include/MPI/detail/CommunicatorImpl.h \
    include/MPI/CommunicatorTraits.h \
    include/MPI/DataTypeDefine.h \
    include/MPI/CustomDataType.h \
    include/MPI/CustomOperationType.h \
    include/MPI/PreDefinedDataType.h \
    include/MPI/PreDefinedOperationType.h \
    include/Utility/ScopedMutex.h \
    include/MPI/PotinToPointCommPod.h \
    include/MPI/detail/PointToPointCommPodImpl.h \
    include/MPI/detail/PointToPointCommSTLContainerImpl.h \
    include/MPI/PotinToPointCommSTLContainer.h \
    include/MPI/CollectiveCommPod.h \
    include/MPI/CollectiveCommPod.h \
    include/MPI/detail/CollectiveCommPodImpl.h \
    include/MPI/detail/CollectiveCommSTLContainerImpl.h \
    include/MPI/CollectiveCommSTLContainer.h \
    include/MPL/ZeroValue.h \
    include/MPI/WindowObject.h \
    include/MPI/detail/WindowObjectImpl.h \
    include/MPI/WindowObjAllocator.h \
    include/MPI/detail/WindowObjAllocatorImpl.h \
    test/Test.h \
    include/MPI/ValArray.h \
    include/MPI/ExpressionTemplate.h \
    include/MPI/detail/ValArrayImpl.h \
    include/MPI/LocalValArray.h

