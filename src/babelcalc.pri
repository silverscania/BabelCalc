QT += widgets

INCLUDEPATH += include

HEADERS       += include/basicinput.h \
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

SOURCES       += src/basicinput.cpp \
    src/binaryinput.cpp \
    src/button.cpp \
    src/calculator.cpp \
    src/gui.cpp \
    src/horizontalbuttongroup.cpp \
    src/input.cpp \
    src/narrowlineedit.cpp

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

RESOURCES += \
    resources/resources.qrc

OBJECTIVE_SOURCES += \
   osxtransparencysupport.mm
