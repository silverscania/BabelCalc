TEMPLATE = subdirs

CONFIG += ordered
SUBDIRS += core tests app
VERSION = 0.2

#Use of .depends here  only indicates the order that projects are first built in.
#PRE_TARGETDEPS in the subdir projects is used for dependency tracking and forcing rebuilds.
tests.depends = core
app.depends = core

#Execute tests when 'make check' is run
unix:check.commands = ./tests/tests
win32:check.commands= tests/tests.exe
QMAKE_EXTRA_TARGETS += check

# Create an installer/release package when 'make installer' is run
installer.depends = app
unix:installer.commands = \
                rm -rf makeself-tmp && \
                mkdir makeself-tmp && \
                cp app/babelcalc \
                   $${PWD}/README.md \
                   $${PWD}/LICENSE \
                   $${PWD}/resources/font/*.txt \
                   $${PWD}/icon/icon.svg \
                   $${PWD}/linux-release/setup.sh makeself-tmp && \
                $${PWD}/linux-release/makeself-release-2.4.0/makeself.sh makeself-tmp \
                babelcalc-$${VERSION}.run BabelCalc \
                ./setup.sh
QMAKE_EXTRA_TARGETS += installer
