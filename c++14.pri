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
