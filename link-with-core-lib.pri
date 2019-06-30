# Qmake doesn't track dependencies of linked libraries. So if lib.lib changes, the app
# won't rebuild. Use PRE_TARGETDEPS to force this to happen
win32: {
    CONFIG( debug, debug|release ) {
        # debug
        LIB_SEARCH_PATH = ../core/debug
    } else {
        # release
        LIB_SEARCH_PATH = ../core/release
    }
    PRE_TARGETDEPS = $${LIB_SEARCH_PATH}/core.lib
}
else: {
    LIB_SEARCH_PATH = ../core
    PRE_TARGETDEPS = $${LIB_SEARCH_PATH}/libcore.a
}
QMAKE_LIBDIR += $${LIB_SEARCH_PATH}
LIBS += -lcore
INCLUDEPATH += ../core
