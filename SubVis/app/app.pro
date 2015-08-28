QT       += core gui widgets opengl xml

CONFIG   += debug c++11

# Build output settings
DESTDIR = ../build
OBJECTS_DIR = ../build
MOC_DIR = ../build
RCC_DIR = ../build

# UI files generated from Qt
UI_DIR = view/


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
HEADERS  += view/mainwindow.h \
    view/viewer_widget.h \
    view/viewer_plugin_widget.h \
    view/viewer_mesh_widget.h \
    model/mesh_data.h \
    controller/io_controller.h \
    controller/draw_controller.h \
    controller/controller.h \
    subvis_app.h \
    plugins/subvis_plugin.h \
    plugins/plugin_manager.h \
    plugins/subdivision/algorithm.h \
    plugins/subdivision/subdivision_algorithms.h \
    plugins/subdivision/types.h \
    plugins/subdivision/utils.h \
    plugins/subdivision/sd_catmull.h \
    plugins/subdivision/sd_doosabin.h \
    plugins/subdivision/sd_loop.h \
    plugins/subdivision/sd_butterfly.h

SOURCES += main.cpp \
    view/mainwindow.cpp \
    view/viewer_widget.cpp \
    view/viewer_plugin_widget.cpp \
    view/viewer_mesh_widget.cpp \
    model/mesh_data.cpp \
    controller/io_controller.cpp \
    controller/draw_controller.cpp \
    controller/controller.cpp \
    subvis_app.cpp \
    plugins/plugin_manager.cpp \
    plugins/subdivision/algorithm.cpp \
    plugins/subdivision/subdivision_algorithms.cpp \
    plugins/subdivision/utils.cpp \
    plugins/subdivision/sd_catmull.cpp \
    plugins/subdivision/sd_doosabin.cpp \
    plugins/subdivision/sd_loop.cpp \
    plugins/subdivision/sd_butterfly.cpp

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

RESOURCES += media.qrc



