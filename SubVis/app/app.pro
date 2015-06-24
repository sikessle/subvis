QT       += core gui widgets

CONFIG   += debug

# Build output settings
DESTDIR = ../build
OBJECTS_DIR = ../build
MOC_DIR = ../build
RCC_DIR = ../build

# Add astylerc
DISTFILES = .astylerc


# Main App

TARGET = SubVis
TEMPLATE = app

# Libraries
LIBS += -L"../lib/surface_mesh" -lsurface_mesh

SOURCES += main.cpp \
           view/mainwindow.cpp

HEADERS  += view/mainwindow.h

FORMS    += view/mainwindow.ui



