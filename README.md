# Teamprojekt Subdivision

This file provides some information about the workflow, environment requirements and the build process.

# Branching/Git-Workflow

Create a local branch per new feature. If feature is working merge to develop branch. Master branch contains only released versions, like beta, vX.Y etc.

# Project Structure

The project consists of the following main folders:

- **dev-doc** Software engineering decisions and diagrams, intermediate reports and the final report of the project.
- **SubVis** The sources (i.e. C++) of the project. Also includes the generated documentation etc. Subdirectories:
  - **app**: Main project
  - **lib**: 3rd party libraries
  - **build**: Created on build, binaries, Doxygen doc(created by Make)
  - **objs**: Object files with 3D models to test
  
# Usage

## Edit Mode

To edit a mesh point:

1. Activate edit mode.
2. Double click on any point.
3. It got selected if it is red
4. Move point around with CTRL+Left Mouse Button
5. Double click anywhere else to apply changes to mesh

To switch editing modes press **S**. 
(Modes: On plane of vertex normal or on plane of orthogonal vertex normal).

# Documentation

Should be inlined with **DoxyGen**. Also see section Build.

# Style Guide

For formatting we use astyle with a project specific style.astylerc option file.
We strive to use the good parts of the Google Style Guide.

## Namespaces

All lower case.

## Variables

Lower case with underscore to separate words: my_member_variable

## Classes

Capitalize first letter, separate with underscore: Subdivison_algorithm.

## Functions

Lower case with underscore to separate words:check_that_value();

# Requirements

- Developed for Ubuntu 14.04 LTS with:
- Qt 5.4.1
- OpenGL 
  - mesa-common-dev 10.1.3-0ubuntu0.4
  - libglu1-mesa-dev: 9.0.2-0
- libQGLViewer 2.6.1 (included in sources, statically linked)
- surface_mesh 1.0 (included in sources, statically linked)
- doxygen 1.8.6 (required for make doc target)

# Build

Ensure in Qt Creator that shadow builds are disabled.

1. `qmake SubVis.pro -r -spec linux-g++`
2. `make doc` for documentation 
3. `make` for build
4. `make clean` to cleanup app builds
5. `make distclean` to cleanup app builds and static library files (*.a)


