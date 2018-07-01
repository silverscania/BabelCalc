#!/bin/bash

#
# This file is executed in the directory that makeself self
# extracting directory extracts to. It copies the stuff from
# the extracted temporary directory to the real install location.
# And also sets up the icons.
#

INSTALL_PATH="$HOME/opt/babelcalc"
BINARY_FILE="babelcalc"

echo "Installing BabelBalc..."
mkdir -p $INSTALL_PATH

# Copy stuff
cp ./* $INSTALL_PATH

# Create desktop file for icon and start menu entry
DESKTOPFILE="$INSTALL_PATH/bitsofbeards-$BINARY_FILE.desktop"
cat >$DESKTOPFILE <<EOF
[Desktop Entry]
Encoding=UTF-8
Name=BabelCalc
Comment=Calculator for software developers
Comment[bg]=калкулатор за софтуерни инжинери
Exec="$INSTALL_PATH/$BINARY_FILE" %F
Terminal=false
X-MultipleArgs=true
Type=Application
Icon=$INSTALL_PATH/icon.svg
Categories=Calculator;Development;Engineering;Science;
EOF

# Executable Desktop File for Gnome
chmod a+x $DESKTOPFILE

# Install desktop file
xdg-desktop-menu install "$DESKTOPFILE"
xdg-desktop-icon install "$DESKTOPFILE"

echo "Done!"
