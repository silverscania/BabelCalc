# BabelCalc
BabelCalc is a cross-platform calculator for developers that operates on multiple numerical bases simultaneously. Its primary use is in generating values when reverse engineering save files and application binaries.

Built using C++14 and Qt

![Video of basic usage]
(https://i.imgur.com/btjTHDt.gif)

#### Features
The following features are an improvement, or are missing from other calculators that are available:
* Input, display and calculation of arbritary bases from 2 to 26
* Display as many different number bases as you want simultaneously
* Edit numbers in place (no need to type it out again if you make a mistake)
* Modulus operator (%)
* Edit binary representation of floating point numbers (IEEE 754, if that's what your system uses)
* Make the window any size you want
* 32bit or 64bit data type mode
* Signed or unsigned mode (operations are performed on the equivalent data type)
* Cross platform
* List of previous calculations (TODO)
* Open source

#### Non features
The following features are not present in the calculator:
* High-precision

#### Purpose
The calculator is intended for programmers and therefore uses the standard library for operations such as cos(θ) and n√x. The underlying data types are either [64bit signed, 64bit unsigned, 64bit float] or [32bit signed, 32bit unsigned, 32bit float], depending on the mode selected. And calculations will overflow and display the result as if they were executed in a program.

It was borne out of the frustration of creating hex values (or ranges of values) to search for while reverse engineering a binary save file. While most calculators support multiple bases, you must navigate through menus to change the mode. This calculator supports editing of any base, and seeing the effect on the other bases in real time.

##### Dependencies
* Qt 5.5.1
* A compiler that supports C++14 
* ImageMagick 7.0+
* Python 2.7

##### Supported platforms
* OSX 10.11+
* Windows 8+
* Linux (Ubuntu) (TODO)

##### License
This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
