TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS += lib tests app

tests.depends = lib
app.depends = lib
