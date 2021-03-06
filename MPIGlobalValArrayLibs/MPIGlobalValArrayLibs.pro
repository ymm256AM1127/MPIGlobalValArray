QT += core
QT -= gui

TARGET = MPIGlobalValArray
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = lib
win32:CONFIG += dll

DEFINES += ENABLE_SHARED_LIBRARY

CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/../debug
} else {
    DESTDIR = $$OUT_PWD/../release
}

DLLDESTDIR = $$DESTDIR/lib

include( ../MPIGlobalValArray.pri )

SOURCES += \
    ../src/MPI/Environment.cpp \
    ../src/MPI/Communicator.cpp \
    ../src/MPI/CustomDataType.cpp \
    ../src/MPI/CustomOperationType.cpp \
#    test/P2PTest.cpp \
#    test/BcastTest.cpp \
#    test/CollectiveTest.cpp \
#    test/CorrectiveAllTest.cpp \
#    test/WindowObjectTest.cpp

HEADERS += \
    ../include/SharedLibraryDefine.h \
    ../include/MPI/Environment.h \
    ../include/Utility/SingletonHolder.h \
    ../include/MPI/ErrorCode.h \
    ../include/MPI/Communicator.h \
    ../include/MPL/is_complex.h \
    ../include/MPL/is_container.h \
    ../include/MPL/is_pod.h \
    ../include/MPL/is_string.h \
    ../include/MPL/remove_modifier.h \
    ../include/MPL/meta_function.h \
    ../include/MPI/detail/CommunicatorImpl.h \
    ../include/MPI/CommunicatorTraits.h \
    ../include/MPI/DataTypeDefine.h \
    ../include/MPI/CustomDataType.h \
    ../include/MPI/CustomOperationType.h \
    ../include/MPI/PreDefinedDataType.h \
    ../include/MPI/PreDefinedOperationType.h \
    ../include/Utility/ScopedMutex.h \
    ../include/MPI/PotinToPointCommPod.h \
    ../include/MPI/detail/PointToPointCommPodImpl.h \
    ../include/MPI/detail/PointToPointCommSTLContainerImpl.h \
    ../include/MPI/PotinToPointCommSTLContainer.h \
    ../include/MPI/CollectiveCommPod.h \
    ../include/MPI/CollectiveCommPod.h \
    ../include/MPI/detail/CollectiveCommPodImpl.h \
    ../include/MPI/detail/CollectiveCommSTLContainerImpl.h \
    ../include/MPI/CollectiveCommSTLContainer.h \
    ../include/MPL/ZeroValue.h \
    ../include/MPI/WindowObject.h \
    ../include/MPI/detail/WindowObjectImpl.h \
    ../include/MPI/WindowObjAllocator.h \
    ../include/MPI/detail/WindowObjAllocatorImpl.h \
    ../include/MPI/ExpressionTemplate.h \
    ../include/MPI/LocalValArray.h \
    ../include/MPI/DataTypes.h \
    ../include/MPI/detail/LocalValArrayImpl.h \
    ../include/MPL/has_member_function.h \
    ../include/MPI/GlobalValArray.h \
    ../include/MPI/detail/GlobalValArrayImpl.h \
    ../include/MPI/ExpressionTemplateForGlobalValArray.h \
    ../include/Utility/ForwardAccessIterator.h \
    ../include/Utility/ForwardAccessIteratorImpl.h \
    ../include/Utility/MathHelper.h \
    ../include/MPI/MPIValArray.h


