#mac stuff
macx: {
    #c++14 workaround http://lists.qt-project.org/pipermail/qt-creator/2015-July/004848.html
    QMAKE_CXXFLAGS_CXX11 += -std=c++14 -stdlib=libc++
    CONFIG += c++11
}

#linux but not mac
unix:!macx {
    CONFIG += c++14
}

!win32: {
    # We always want to use the libc math functions, not GCCs. One failure
    # mode without this is a unit test with constants and optimisation turned
    # on. GCCs pow() will saturate and clamp, where as calling glibc, returning
    # a double and casting to uint32 will overflow.
    QMAKE_CXXFLAGS += -fno-builtin
}
