# Qmake doesn't track dependencies of linked libraries. So if lib.lib changes, the app
# won't rebuild. Use PRE_TARGETDEPS to force this to happen
win32: {
    CONFIG( debug, debug|release ) {
        # debug
        LIB_SEARCH_PATH = ../lib/debug
    } else {
        # release
        LIB_SEARCH_PATH = ../lib/release
    }
    PRE_TARGETDEPS = $${LIB_SEARCH_PATH}/lib.lib
}
else: {
    LIB_SEARCH_PATH = ../lib
    PRE_TARGETDEPS = $${LIB_SEARCH_PATH}/liblib.a
}
QMAKE_LIBDIR += $${LIB_SEARCH_PATH}
LIBS += -llib
