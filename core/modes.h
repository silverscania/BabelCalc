// Copyright (C) 2017 James Clark <james@bitsofbeards.co.uk>
//
// This file is part of BabelCalc, a calculator that operates on
// multiple numerical bases simultaneously.
//
// BabelCalc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// BabelCalc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with BabelCalc.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

/**
 * In theory these modes are the ones that actually affect the outcome of calculations.
 * For example multiplication only reuturns the same signed and unsigned value for the
 * low (16 bits) half of a 32 bit value.
 * Because the imult instruction must be used to get the full 32bit result, both the
 * signed and unsigned values must be stored. Addition and subtraction would
 * be the same so it's unneccesary for this case.
 */
enum class Mode {Float, Unsigned, Signed};

/**
 * This enum determines whether floats are displayed in human readable
 * form (in any base). For example 10.5 or 0xA.8
 *
 * Or whether they are displayed in machine form, which would be the binary representation
 * of IEEE-745.
 *
 * For signed ints it determines whether they are displayed with a ± sign (Human) or wether
 * they are displayed as twos complement (Machine).
 *
 * For unsigned ints there is no difference...
 */
enum class ReprMode {Human, Machine};

enum class BitWidth {ThirtyTwo, SixtyFour};
enum class AngleMode {Radians, Degrees};
