TEMPLATE        = lib
CONFIG         += debug plugin c++11
QT             += core gui widgets opengl
INCLUDEPATH    += ../../app \
                  ../../lib
TARGET          = plugin_subdivision


# Build output settings
DESTDIR = ../../build/plugins/
OBJECTS_DIR = ../../build/plugins/
MOC_DIR = ../../build/plugins/
RCC_DIR = ../../build/plugins/

HEADERS = subdivision_algorithms.h \
    algorithm.h \
    test.h

SOURCES = subdivision_algorithms.cpp \
    algorithm.cpp \
    test.cpp

DISTFILES += subdivision.json

# Target for documentation
doc.target = doc
doc.depends = $(SOURCES) doxygen.conf
doc.commands = doxygen doxygen.conf
QMAKE_EXTRA_TARGETS += doc
