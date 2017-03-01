QT += widgets testlib
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle
CONFIG	 += c++11

INCLUDEPATH += include/
SOURCES = tests/variablesizetest.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target
