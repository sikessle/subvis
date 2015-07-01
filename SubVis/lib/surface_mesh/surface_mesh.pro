TEMPLATE = lib
CONFIG += staticlib
HEADERS = *.h
SOURCES = *.cpp

# As Surface_Mesh includes are written as surface_mesh/header.h
INCLUDEPATH += ..

QMAKE_CFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON -= -Wall
