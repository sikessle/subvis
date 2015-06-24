TEMPLATE = subdirs
CONFIG += ordered debug
SUBDIRS = app \
          lib/surface_mesh

app.depends = surface_mesh

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

