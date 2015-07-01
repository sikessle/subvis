QT       += core gui widgets opengl xml

CONFIG   += debug

# Build output settings
DESTDIR = ../build
OBJECTS_DIR = ../build
MOC_DIR = ../build
RCC_DIR = ../build



# Main App
TARGET = SubVis
TEMPLATE = app

# Libraries
LIBS += -L"../lib/surface_mesh" \
        -L"../lib/QGLViewer" \
        -lsurface_mesh \
        -lQGLViewer

INCLUDEPATH += "../lib"

# SOURCES
SOURCES += main.cpp \
           view/mainwindow.cpp

HEADERS  += view/mainwindow.h

FORMS    += view/mainwindow.ui



# Extra clean up commands
extraclean.commands = rm -rf ../doc ../build
clean.depends = extraclean
QMAKE_EXTRA_TARGETS += clean extraclean


# Add astylerc
DISTFILES = .astylerc

# Target for documentation
doc.target = doc
doc.commands = doxygen doxygen.conf
doc.depends = $(SOURCES) doxygen.conf
QMAKE_EXTRA_TARGETS += doc
