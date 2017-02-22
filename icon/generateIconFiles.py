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

def exportMacIcons():
	folder = "tmp/icon.iconset"
	os.makedirs(folder)

	exportImage(1024,1024, folder, "icon_512x512@2x.png")
	exportImage(1024,1024, folder)

	exportImage(512,512, folder, "icon_256x256@2x.png")
	exportImage(512,512, folder)

	exportImage(256,256, folder, "icon_128x128@2x.png")
	exportImage(256,256, folder)

	exportImage(128,128, folder, "icon_64x64@2x.png")
	exportImage(128,128, folder)

	exportImage(64,64, folder, "icon_32x32@2x.png")
	exportImage(64,64, folder)

	exportImage(32,32, folder, "icon_16x16@2x.png")
	exportImage(32,32, folder)

	exportImage(16,16, folder)

	proc = subprocess.Popen(["iconutil",
		"-c", "icns",
		"tmp/icon.iconset"],
		stdout=subprocess.PIPE)
	result = proc.communicate()[0] #wait

def exportImage(xRes, yRes, folder="tmp", name="icon_{0}x{1}.png"):
	try:
		proc = subprocess.Popen([convert,
			"-density", "2000",
			"-verbose",
			"-background", "transparent",
			"icon.svg",
			"-resize", str(xRes) + "x" + str(yRes),
			os.path.join(folder, name.format(str(xRes), str(yRes)))],
			stdout=subprocess.PIPE)
		result = proc.communicate()[0] #wait
		print result
	except Exception, e:
		print "Couldn't run image magick (cmd:" + convert + "). Do you have it installed?"
		print str(e)
		exit(1)

#Clean tmp dir
shutil.rmtree("tmp", ignore_errors=True)
os.makedirs("tmp")

if sys.platform.startswith("win32"):
	exportWindowsIcons()
elif sys.platform.startswith("darwin"):
	exportMacIcons()
elif sys.platform.linux.startswith("linux"):
	print "Don't know how to make icons for this platform yet"
else:
	print "Error: Unknown platform in generateIconFiles.py"
	exit(1)


