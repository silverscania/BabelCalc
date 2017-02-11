#!/usr/bin/python

import subprocess
import sys
import glob, os
import shutil
import json
import argparse
import time
import fnmatch

convert = "magick"

def exportWindowsIcons():
	exportImage(256,256)
	exportImage(48,48)
	exportImage(32,32)
	exportImage(16,16)

	proc = subprocess.Popen([convert, "tmp/*.png",
		 "tmp/icon.ico"],
		stdout=subprocess.PIPE)
	result = proc.communicate()[0] #wait

def exportImage(xRes, yRes):
	proc = subprocess.Popen([convert, "icon.svg",
		"-resize", str(xRes) + "x" + str(yRes),
		 "tmp/icon" + str(xRes) + "x" + str(yRes) + ".png"],
		stdout=subprocess.PIPE)
	result = proc.communicate()[0] #wait

#Clean tmp dir
shutil.rmtree("tmp", ignore_errors=True)
os.makedirs("tmp")

if sys.platform.startswith("win32"):
	exportWindowsIcons()
elif sys.platform.startswith("darwin"):
	print "Don't know how to make icons for this platform yet"
elif sys.platform.linux.startswith("linux"):
	print "Don't know how to make icons for this platform yet"
else:
	print "Error: Unknown platform in generateIconFiles.py"
	exit()


