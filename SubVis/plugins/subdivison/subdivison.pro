TEMPLATE        = lib
CONFIG         += plugin c++11
QT             += widgets
INCLUDEPATH    += ../../app
TARGET          = $$qtLibraryTarget(subdivision)


# Build output settings
DESTDIR = ../../build/plugins/
OBJECTS_DIR = ../../build/plugins/
MOC_DIR = ../../build/plugins/
RCC_DIR = ../../build/plugins/

HEADERS         = \
    subdivision_algorithms.h
SOURCES         = \
    subdivision_algorithms.cpp

DISTFILES += \
    subdivision.json

