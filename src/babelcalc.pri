QT += widgets

INCLUDEPATH += $$PWD/../include

HEADERS += \
    $$PWD/../include/basicinput.h \
    $$PWD/../include/binaryinput.h \
    $$PWD/../include/button.h \
    $$PWD/../include/calculator.h \
    $$PWD/../include/gui.h \
    $$PWD/../include/horizontalbuttongroup.h \
    $$PWD/../include/input.h \
    $$PWD/../include/modes.h \
    $$PWD/../include/narrowlineedit.h \
    $$PWD/../include/operators.h \
    $$PWD/../include/osxtransparencysupport.h \
    $$PWD/../include/value.h

SOURCES += \
    $$PWD/basicinput.cpp \
    $$PWD/binaryinput.cpp \
    $$PWD/button.cpp \
    $$PWD/calculator.cpp \
    $$PWD/gui.cpp \
    $$PWD/horizontalbuttongroup.cpp \
    $$PWD/input.cpp \
    $$PWD/narrowlineedit.cpp

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
    $$PWD/../resources/resources.qrc

OBJECTIVE_SOURCES += \
   $$PWD/osxtransparencysupport.mm
