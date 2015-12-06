QT += core
QT -= gui

TARGET = MPIVector
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


win32:{
    MPIPATH = "C:/Program Files (x86)/Microsoft SDKs/MPI"
    MPILIB = $$MPIPATH/Lib/x64
    MPIINC = $$MPIPATH/Include
}

LIBS += -L$$MPILIB -lmsmpi

INCLUDEPATH += $$MPIINC
DEPENDPATH  += $$MPIINC

