TEMPLATE = lib
CONFIG += staticlib
QT += widgets

HEADERS += \
    basicinput.h \
    binaryinput.h \
    button.h \
    calculator.h \
    gui.h \
    horizontalbuttongroup.h \
    input.h \
    modes.h \
    narrowlineedit.h \
    operators.h \
    osxtransparencysupport.h \
    value.h

SOURCES += \
    basicinput.cpp \
    binaryinput.cpp \
    button.cpp \
    calculator.cpp \
    gui.cpp \
    horizontalbuttongroup.cpp \
    input.cpp \
    narrowlineedit.cpp

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

OBJECTIVE_SOURCES += \
   osxtransparencysupport.mm
