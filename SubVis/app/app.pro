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

# Extra clean up commands
extraclean.commands = rm -rf ../build
clean.depends = extraclean
QMAKE_EXTRA_TARGETS += clean extraclean


# Target for documentation
doc.target = doc
doc.depends = $(SOURCES) doxygen.conf
doc.commands = doxygen doxygen.conf
QMAKE_EXTRA_TARGETS += doc

RESOURCES += media.qrc

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
    subvis_app.h \
    plugins/subvis_plugin.h \
    plugins/plugin_manager.h \
    plugins/subdivision/sd_catmull.h \
    plugins/subdivision/sd_doosabin.h \
    plugins/subdivision/sd_loop.h \
    plugins/subdivision/sd_butterfly.h \
    plugins/subdivision/subdivision_algorithms_plugin.h \
    plugins/subdivision/sd_algorithm.h \
    plugins/subdivision/gl_renderer.h \
    plugins/subdivision/gl_bspline_renderer.h \
    plugins/subdivision/debug.h \
    plugins/subdivision/sd_quad.h \
    plugins/subdivision/sd_triangle.h \
    plugins/subdivision/sd_modbutterfly.h

SOURCES += main.cpp \
    view/mainwindow.cpp \
    view/viewer_widget.cpp \
    view/viewer_plugin_widget.cpp \
    view/viewer_mesh_widget.cpp \
    model/mesh_data.cpp \
    subvis_app.cpp \
    plugins/plugin_manager.cpp \
    plugins/subdivision/sd_catmull.cpp \
    plugins/subdivision/sd_doosabin.cpp \
    plugins/subdivision/sd_loop.cpp \
    plugins/subdivision/sd_butterfly.cpp \
    plugins/subdivision/subdivision_algorithms_plugin.cpp \
    plugins/subdivision/sd_algorithm.cpp \
    plugins/subdivision/gl_renderer.cpp \
    plugins/subdivision/gl_bspline_renderer.cpp \
    plugins/subdivision/debug.cpp \
    plugins/subdivision/sd_quad.cpp \
    plugins/subdivision/sd_triangle.cpp \
    plugins/subdivision/sd_modbutterfly.cpp

FORMS    += view/mainwindow.ui




