TEMPLATE = lib
CONFIG += staticlib
HEADERS = *.h
SOURCES = *.cpp

DESTDIR = ../../build/surface_mesh
OBJECTS_DIR = ../../build/surface_mesh

QMAKE_CFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON -= -Wall
