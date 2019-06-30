QT += widgets
include(../c++14.pri)

TEMPLATE = app
INCLUDEPATH += ../lib
SOURCES += main.cpp

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


#QMAKE_EXTRA_TARGETS += iconTarget

# Change name of output binary
TARGET = babelcalc

# install
target.path = /Applications/BabelCalc
INSTALLS += target

#mac stuff
macx: {
    OBJECTIVE_HEADERS =
    OBJECTIVE_SOURCES =
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10
    QMAKE_MAC_SDK = macosx
    LIBS += -framework Cocoa
}

#mac stuff
macx: {
        #mac icon
        iconTarget.target = icon.icns
        iconTarget.depends =
        iconTarget.commands = cd $${PWD}/icon/ && python generateIconFiles.py && mv tmp/icon.icns $${OUT_PWD}/icon.icns
        ICON = $${OUT_PWD}/icon.icns
}

#windows icon
#win32: {
#        iconTarget.target = icon.rc
#        iconTarget.depends =
#        iconTarget.commands = cd $${PWD}\icon\ && python generateIconFiles.py && move tmp\icon.rc $${OUT_PWD} && move tmp\icon.ico $${OUT_PWD}
#
#        RC_FILE = $${OUT_PWD}/icon.rc
#}

RESOURCES += \
    ../resources/resources.qrc

FORMS += \
    dialog.ui
