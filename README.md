# Teamprojekt Subdivision

This file provides some information about the workflow, environment requirements and the build process.

# Branching/Git-Workflow

https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow

# Project Structure

The project consists of the following main folders:

- **doc** Software engineering decisions and diagrams, intermediate reports and the final paper of the project.
- **SubVis** The sources (i.e. C++) of the project. Also includes the generated documentation etc.

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

- Tested only for Ubuntu 14.04 LTS
- Qt 5.4.1
- OpenGL 
    - mesa-common-dev 10.1.3-0ubuntu0.4
    - libglu1-mesa-dev: 9.0.2-0
- libQGLViewer 2.6.1
- surface_mesh 1.0


# Build

1. `qmake SubVis.pro -r -spec linux-g++ CONFIG+=debug`
2. `make doc` for documentation
2. `make` for build
3. `make clean` to cleanup

