include(src/babelcalc.pri)

#SUBDIRS += tests

#INCLUDEPATH += src
SOURCES += src/main.cpp

QMAKE_EXTRA_TARGETS += iconTarget

# install
target.path = /Applications/BabelCalc
INSTALLS += target

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
