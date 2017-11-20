include(../c++14.pri)

QT += widgets testlib
QT -= gui

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase

#Link with the babelcalc library
LIBS += ../lib/liblib.a
INCLUDEPATH += ../lib

SOURCES += \ #tests/variablesizetest.cpp \
    calculatortest.cpp

#Execute tests after linking
unix:QMAKE_POST_LINK=./$$TARGET
win32:QMAKE_POST_LINK=$${TARGET}.exe
