include(../c++14.pri)

QT += widgets testlib
QT -= gui

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase

CONFIG( debug, debug|release ) {
    # debug
    LIB_SEARCH_PATH += ../lib/debug
} else {
    # release
    LIB_SEARCH_PATH += ../lib/release
}

# Qmake doesn't track dependencies of linked libraries. So if lib.lib changes, the app
# won't rebuild. Use PRE_TARGETDEPS to force this to happen
win32: {
    PRE_TARGETDEPS = $${LIB_SEARCH_PATH}/lib.lib
}
else: {
    PRE_TARGETDEPS = $${LIB_SEARCH_PATH}/liblib.a
}

QMAKE_LIBDIR += $${LIB_SEARCH_PATH}
LIBS += -llib

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
    testcalculator.cpp \
    testvariablesizes.cpp

HEADERS +=
