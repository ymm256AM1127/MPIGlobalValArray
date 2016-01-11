QT += core
QT -= gui

include( ../MPIGlobalValArray.pri )

TARGET = Sample
CONFIG += console
CONFIG -= app_bundle


TEMPLATE = app

SOURCES += main.cpp

CONFIG(debug, debug|release) {
    DESTDIR = $$OUT_PWD/../debug
    LIBS += -L$$OUT_PWD/../debug -lMPIGlobalValArray
} else {
    DESTDIR = $$OUT_PWD/../release
    LIBS += -L$$OUT_PWD/../release -lMPIGlobalValArray
}

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
