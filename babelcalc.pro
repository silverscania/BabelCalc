QT += widgets

INCLUDEPATH += include/

HEADERS       = include/basicinput.h \
    include/binaryinput.h \
    include/button.h \
    include/calculator.h \
    include/gui.h \
    include/horizontalbuttongroup.h \
    include/input.h \
    include/modes.h \
    include/narrowlineedit.h \
    include/operators.h \
    include/osxtransparencysupport.h \
    include/value.h

SOURCES       = src/basicinput.cpp \
    src/binaryinput.cpp \
    src/button.cpp \
    src/calculator.cpp \
    src/gui.cpp \
    src/horizontalbuttongroup.cpp \
    src/input.cpp \
    src/main.cpp \
    src/narrowlineedit.cpp


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

	iconTarget.target = icon.icns
	iconTarget.depends =
	iconTarget.commands = cd $${PWD}/icon/ && python generateIconFiles.py && mv tmp/icon.icns $${OUT_PWD}/icon.icns

	ICON = $${OUT_PWD}/icon.icns
}

#linux but not mac
unix:!macx {
   CONFIG += c++14
}

#windows icon
win32: {
	RC_FILE = icon/icon.rc
}

RESOURCES += \
    resources/resources.qrc

OBJECTIVE_SOURCES += \
   src/osxtransparencysupport.mm
