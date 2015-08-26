# Teamprojekt Subdivision

This file provides some information about the workflow, environment requirements and the build process.

# Branching/Git-Workflow

Create a local branch per new feature. If feature is working merge to develop branch. Master branch contains only released versions, like beta, vX.Y etc.

# Project Structure

The project consists of the following main folders:

- **dev-doc** Software engineering decisions and diagrams, intermediate reports and the final paper of the project.
- **SubVis** The sources (i.e. C++) of the project. Also includes the generated documentation etc. Subdirectories:
  - **app**: Main project
  - **lib**: 3rd party libraries
  - **build**: Created on build, binaries, Doxygen doc(created by Make)
  - **test**: Unit tests
  - **objs**: Object files with 3D models to test
  - **plugins**: Default plugins

# Documentation

Should be inlined with **DoxyGen**.

# Style Guide

For formatting we use astyle with a project specific .astylerc option file.

## Variables

Lower case with underscore to separate words: my_member_variable

## Classes

Capitalize first letter, separate with underscore: Subdivison_algorithm.

## Functions

Lower case with underscore to separate words:check_that_value();

# Requirements

- Tested only for Ubuntu 14.04 LTS with:
- Qt 5.4.1
- OpenGL 
  - mesa-common-dev 10.1.3-0ubuntu0.4
  - libglu1-mesa-dev: 9.0.2-0
- libQGLViewer 2.6.1
- surface_mesh 1.0
- doxygen 1.8.6

# Build

1. `qmake SubVis.pro -r -spec linux-g++`
2. `make doc` for documentation 
3. `make` for build
4. `make clean` to cleanup app builds
5. `make distclean` to cleanup app builds and static library files (*.a)
