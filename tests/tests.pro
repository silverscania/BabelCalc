include(../src/babelcalc.pri)

QT += widgets testlib
QT       -= gui

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += testcase

#mac stuff
macx: {
	#c++14 workaround http://lists.qt-project.org/pipermail/qt-creator/2015-July/004848.html
	QMAKE_CXXFLAGS_CXX11 = -std=c++14 -stdlib=libc++
	CONFIG += c++11
}

#linux but not mac
unix:!macx {
    CONFIG += c++14
}

#INCLUDEPATH += include/
SOURCES += \ #tests/variablesizetest.cpp \
    calculatortest.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qtestlib/tutorial1
INSTALLS += target
