QT += widgets testlib
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle

SOURCES = tests/variablesizetest.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target
