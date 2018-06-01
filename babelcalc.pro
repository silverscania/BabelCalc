TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS += lib tests app

tests.depends = lib
app.depends = lib

#Execute tests when 'make check' is run
unix:check.commands = ./tests/tests
win32:check.commands= tests/tests.exe
QMAKE_EXTRA_TARGETS += check
