TEMPLATE = subdirs
SUBDIRS = surface_mesh \
          QGLViewer


# Extra clean up commands
extradistclean.commands = rm -rf QGLViewer/libQGLViewer.a \
                        surface_mesh/*.a

distclean.depends = extradistclean
# add doc as dummy
QMAKE_EXTRA_TARGETS += distclean extradistclean doc

