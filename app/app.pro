QT += widgets
include(../c++14.pri)
include(../link-with-core-lib.pri)
TEMPLATE = app
SOURCES += main.cpp

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

QMAKE_EXTRA_TARGETS += iconTarget
#mac icon
macx: {
        iconTarget.target = icon.icns
        iconTarget.depends =
        iconTarget.commands = cd $${PWD}/icon/ && python generateIconFiles.py && mv tmp/icon.icns $${OUT_PWD}/icon.icns
        ICON = $${OUT_PWD}/icon.icns
}

#windows icon
win32: {
        iconTarget.target = icon.rc
        iconTarget.depends =
        iconTarget.commands = cd $${PWD}/../icon && python generateIconFiles.py && move tmp\icon.rc $${OUT_PWD} && move tmp\icon.ico $${OUT_PWD}

        RC_FILE = $${OUT_PWD}/icon.rc
}

RESOURCES += \
    ../resources/resources.qrc

FORMS += \
    dialog.ui
