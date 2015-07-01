TEMPLATE = subdirs

SUBDIRS = app lib

app.depends = lib

doc.target = doc
doc.CONFIG = recursive
QMAKE_EXTRA_TARGETS += doc

