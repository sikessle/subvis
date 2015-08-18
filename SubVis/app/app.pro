QT       += core gui widgets opengl xml

CONFIG   += debug c++11

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
           algo/utils.cpp \
    view/viewer_widget.cpp \
    view/viewer_plugin_widget.cpp \
    view/viewer_mesh_widget.cpp \
    subvisapp.cpp \
    model/mesh_data.cpp

HEADERS  += view/mainwindow.h \
            algo/sd_catmull.h \
            algo/utils.h \
            algo/types.h \
    view/viewer_widget.h \
    view/viewer_plugin_widget.h \
    view/viewer_mesh_widget.h \
    subvisapp.h \
    model/mesh_data.h

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

RESOURCES += \
    media.qrc



