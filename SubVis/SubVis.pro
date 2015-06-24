QT       += core gui widgets

CONFIG   += debug

# Directories
ROOT_DIRECTORY = $$PWD
LIB_DIRECTORY = $${ROOT_DIRECTORY}/lib
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
EXEC_DIRECTORY = $${BUILD_DIRECTORY}

# Target for documentation
doc.target = doc
doc.commands = doxygen doxygen.conf
doc.depends = $(SOURCES) doxygen.conf
QMAKE_EXTRA_TARGETS += doc

# Extra clean up commands
# clean doc and build directories
extraclean.commands = rm -rf doc build;
clean.depends = extraclean
QMAKE_EXTRA_TARGETS += clean extraclean

# Build output settings
DESTDIR = $${EXEC_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}
MOC_DIR = $${BUILD_DIRECTORY}
RCC_DIR = $${BUILD_DIRECTORY}



# Main App

TARGET = SubVis
TEMPLATE = app

SOURCES += src/main.cpp\
        src/mainwindow.cpp

HEADERS  += src/mainwindow.h

FORMS    += src/mainwindow.ui



