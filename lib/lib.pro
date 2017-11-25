include(../c++14.pri)

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
    value.h \
    calculationhistoryview.h \
    base10input.h

SOURCES += \
    basicinput.cpp \
    binaryinput.cpp \
    button.cpp \
    calculator.cpp \
    gui.cpp \
    horizontalbuttongroup.cpp \
    input.cpp \
    narrowlineedit.cpp \
    calculationhistoryview.cpp \
    base10input.cpp

OBJECTIVE_SOURCES += \
   osxtransparencysupport.mm
