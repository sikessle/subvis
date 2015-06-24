TEMPLATE = subdirs
SUBDIRS = surface_mesh \
          QGLViewer


# Extra clean up commands
extradistclean.commands = rm -rf lib/QGLViewer/.moc/* \
                        lib/QGLViewer/.obj/* \
                        lib/QGLViewer/libQGLViewer.a \
                        lib/surface_mesh/*.a \
                        lib/surface_mesh/*.o

distclean.depends = extradistclean
QMAKE_EXTRA_TARGETS += distclean extradistclean
