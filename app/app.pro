QT += widgets
INCLUDEPATH += ../lib
SOURCES += main.cpp

LIBS += ../lib/liblib.a

QMAKE_EXTRA_TARGETS += iconTarget

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
    #c++14 workaround http://lists.qt-project.org/pipermail/qt-creator/2015-July/004848.html
    QMAKE_CXXFLAGS_CXX11 = -std=c++14 -stdlib=libc++
    CONFIG += c++11
}

#linux but not mac
unix:!macx {
   CONFIG += c++14
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
win32: {
        iconTarget.target = icon.rc
        iconTarget.depends =
        iconTarget.commands = cd $${PWD}\icon\ && python generateIconFiles.py && move tmp\icon.rc $${OUT_PWD} && move tmp\icon.ico $${OUT_PWD}

        RC_FILE = $${OUT_PWD}/icon.rc
}

RESOURCES += \
    ../resources/resources.qrc
