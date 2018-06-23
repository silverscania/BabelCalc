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

#include <cstring>
#include <cstdint>

using CalcFloat = float;
using CalcDouble = double;
using CalcInt = std::int64_t;
using CalcUInt = std::uint64_t;

static_assert(sizeof(CalcInt) == sizeof(CalcUInt), "Types must all be 64 bits");
static_assert(sizeof(CalcInt) == sizeof(CalcDouble), "Types must all be 64 bits");
static_assert(sizeof(CalcInt) == 64/8, "Types must all be 64 bits");


struct Value {
	inline Value(CalcInt intVal, CalcDouble doubleVal, CalcUInt uIntVal, CalcFloat floatVal) :
		intVal(intVal),
		doubleVal(doubleVal),
		uIntVal(uIntVal),
		floatVal(floatVal)
    {
    }

	inline Value(CalcUInt rawIntVal, CalcUInt rawDoubleVal, CalcUInt uIntVal, CalcUInt rawFloatVal) :
		intVal(rawToX<CalcInt>(rawIntVal)),
		doubleVal(rawToX<CalcDouble>(rawDoubleVal)),
		uIntVal(uIntVal),
		floatVal(rawToX<CalcFloat>(rawFloatVal))
	{
	}

    inline Value() :
		intVal(0),
		doubleVal(0),
		uIntVal(0),
		floatVal(0)
    {
    }

//	inline static CalcUInt floatToRaw(CalcDouble value) {
//		CalcUInt ret;
//		std::memcpy(&ret, &value, sizeof(CalcDouble));
//		return ret;
//	}

//	inline static CalcUInt floatToRaw(CalcInt value) {
//		CalcUInt ret;
//		std::memcpy(&ret, &value, sizeof(CalcInt));
//		return ret;
//	}

	template <class T>
	inline static T rawToX(CalcUInt value) {
		T ret;
		std::memcpy(&ret, &value, sizeof(T));
		return ret;
	}

	CalcInt intVal;
	CalcDouble doubleVal;
	CalcUInt uIntVal;
	CalcFloat floatVal;
};
