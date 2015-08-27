TEMPLATE        = lib
CONFIG         += debug plugin c++11
QT             += widgets
INCLUDEPATH    += ../../app \
                  ../../lib
TARGET          = $$qtLibraryTarget(subdivision)


# Build output settings
DESTDIR = ../../build/plugins/
OBJECTS_DIR = ../../build/plugins/
MOC_DIR = ../../build/plugins/
RCC_DIR = ../../build/plugins/

HEADERS = subdivision_algorithms.h \
    algorithm.h

SOURCES = subdivision_algorithms.cpp \
    algorithm.cpp

DISTFILES += subdivision.json

