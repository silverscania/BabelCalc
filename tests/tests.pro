include(../c++14.pri)

QT += widgets testlib
QT -= gui

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase

#Link with the babelcalc library
LIBS += ../lib/liblib.a

INCLUDEPATH += \
    ../lib \
    deps/googletest/googlemock \
    deps/googletest/googletest \
    deps/googletest/googlemock/include \
    deps/googletest/googletest/include

SOURCES += \ #tests/variablesizetest.cpp \
    deps/googletest/googlemock/src/gmock-all.cc \
    deps/googletest/googletest/src/gtest-all.cc \
    main.cpp \
    testcalculator.cpp

#Execute tests after linking
unix:QMAKE_POST_LINK=./$$TARGET
win32:QMAKE_POST_LINK=$${TARGET}.exe

HEADERS +=
