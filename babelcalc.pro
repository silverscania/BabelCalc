TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS += lib tests app

#Use of .depends here  only indicates the order that projects are first built in.
#PRE_TARGETDEPS in the subdir projects is used for dependency tracking and forcing rebuilds.
tests.depends = lib
app.depends = lib
