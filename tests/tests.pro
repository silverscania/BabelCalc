include(../c++14.pri)
include(../link-with-core-lib.pri)
QT += widgets testlib
QT -= gui

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase

INCLUDEPATH += \
    deps/googletest/googlemock \
    deps/googletest/googletest \
    deps/googletest/googlemock/include \
    deps/googletest/googletest/include

SOURCES += \
    deps/googletest/googlemock/src/gmock-all.cc \
    deps/googletest/googletest/src/gtest-all.cc \
    main.cpp \
    testcalculator.cpp \
    testvariablesizes.cpp \
    testinput.cpp

HEADERS +=
