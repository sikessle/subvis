QT       += core gui widgets opengl xml

CONFIG   += debug

# Build output settings
DESTDIR = ../build
OBJECTS_DIR = ../build
MOC_DIR = ../build
RCC_DIR = ../build

# UI files generated from Qt
UI_DIR = view/ui-gen/


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
           view/mainwindow.cpp \
           algo/sd_catmull.cpp \
    algo/utils.cpp

HEADERS  += view/mainwindow.h \
            algo/sd_catmull.h \
    algo/utils.h \
    algo/types.h

FORMS    += view/mainwindow.ui



# Extra clean up commands
extraclean.commands = rm -rf ../build
clean.depends = extraclean
QMAKE_EXTRA_TARGETS += clean extraclean


# Add astylerc
DISTFILES = .astylerc

# Target for documentation
doc.target = doc
doc.depends = $(SOURCES) doxygen.conf
doc.commands = doxygen doxygen.conf
QMAKE_EXTRA_TARGETS += doc


