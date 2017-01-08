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

#include "value.h"
#include <cmath>
#include "modes.h"

constexpr double pi = 3.141592653589793238462643383279502884;
constexpr double e = 2.71828182845904523536028747135266249775724709369995;

struct Operator {
	explicit inline Operator(int precedence) :
		precedence(precedence)
	{}

	Operator(const Operator&) = delete;
	void operator=(const Operator& x) = delete;

	inline virtual ~Operator() {}

	AngleMode angleMode {AngleMode::Degrees};
	const int precedence;
};

struct UnaryOperator : public Operator {
	inline UnaryOperator() : Operator(0) {}
	virtual Value evaluate(const Value& rhs) const = 0;
};

struct BinaryOperator : public Operator {
	inline BinaryOperator(int precedence) : Operator(precedence) {}
	virtual Value evaluate(const Value& lhs, const Value& rhs) const = 0;
};

#define BINARY_OPERATOR(Type, Op, Precedence) struct Type : public BinaryOperator { \
	inline Type() : BinaryOperator(Precedence) {} \
	inline Value evaluate(const Value &lhs, const Value &rhs) const override { \
		return Value(lhs.intVal Op rhs.intVal, lhs.floatVal Op rhs.floatVal, lhs.uIntVal Op rhs.uIntVal); \
	} \
};

#define UNARY_OPERATOR(Type, Op) struct Type : public UnaryOperator { \
	inline Value evaluate(const Value &rhs) const override { \
		return Value(static_cast<CalcInt>(Op(rhs.intVal)), static_cast<CalcFloat>(Op(rhs.floatVal)), static_cast<CalcUInt>(Op(rhs.uIntVal))); \
	} \
};

#define UNARY_ANGLE_OPERATOR(Type, Op) struct Type : public UnaryOperator { \
	inline Value evaluate(const Value &rhs) const override { \
		constexpr CalcFloat mult = pi/180; \
		if(angleMode == AngleMode::Degrees) \
			return Value(static_cast<CalcInt>(Op(rhs.intVal*mult)), static_cast<CalcFloat>(Op(rhs.floatVal*mult)), static_cast<CalcUInt>(Op(rhs.uIntVal*mult))); \
		else \
			return Value(static_cast<CalcInt>(Op(rhs.intVal)), static_cast<CalcFloat>(Op(rhs.floatVal)), static_cast<CalcUInt>(Op(rhs.uIntVal))); \
	} \
};

//places signed int result into float (bitwise ops not supported for floats)
#define BITWISE_OPERATOR(Type, Op, PostOp, Precedence) struct Type : public BinaryOperator { \
	inline Type() : BinaryOperator(Precedence) {} \
	inline Value evaluate(const Value &lhs, const Value &rhs) const override { \
		const CalcInt intVal = PostOp (lhs.intVal Op rhs.intVal); \
		const CalcUInt uIntVal = PostOp (lhs.uIntVal Op rhs.uIntVal); \
		const CalcUInt fVal = PostOp (*reinterpret_cast<const CalcUInt*>(&lhs.floatVal) Op *reinterpret_cast<const CalcUInt*>(&rhs.floatVal)); \
		return Value(intVal, *reinterpret_cast<const CalcFloat*>(&fVal), uIntVal); \
	} \
};

#define BITWISE_UNARY_OPERATOR(Type, Op, PostOp) struct Type : public UnaryOperator { \
	inline Value evaluate(const Value &rhs) const override { \
		const CalcInt intVal = Op rhs.intVal PostOp; \
		const CalcUInt uIntVal = Op rhs.uIntVal PostOp; \
		const CalcUInt fVal = Op *reinterpret_cast<const CalcUInt*>(&rhs.floatVal); \
		return Value(intVal, *reinterpret_cast<const CalcFloat*>(&fVal), uIntVal); \
	} \
};

BINARY_OPERATOR(AdditionOperator,			+,		2)
BINARY_OPERATOR(SubtractionOperator,		-,		2)
BINARY_OPERATOR(MultiplicationOperator,		*,		1)
BINARY_OPERATOR(DivisionOperator,			/,		1)

BITWISE_OPERATOR(AndOperator,				&,  ,	0)
BITWISE_OPERATOR(OrOperator,				|,  ,	0)
BITWISE_OPERATOR(NorOperator,				|, ~,	0)
BITWISE_OPERATOR(NandOperator,				&, ~,	0)
BITWISE_OPERATOR(XorOperator,				^,  ,	0)
BITWISE_OPERATOR(ShiftLeftByOperator,		<<, ,	0)
BITWISE_OPERATOR(ShiftRightByOperator,		>>, ,	0)

UNARY_OPERATOR(OneOverOperator, 1/)
UNARY_OPERATOR(NegationOperator, -)

UNARY_ANGLE_OPERATOR(ASinhOperator, std::asinh)
UNARY_ANGLE_OPERATOR(ACoshOperator, std::acosh)
UNARY_ANGLE_OPERATOR(ATanhOperator, std::atanh)
UNARY_ANGLE_OPERATOR(ASinOperator, std::asin)
UNARY_ANGLE_OPERATOR(ACosOperator, std::acos)
UNARY_ANGLE_OPERATOR(ATanOperator, std::atan)

UNARY_ANGLE_OPERATOR(SinhOperator, std::sinh)
UNARY_ANGLE_OPERATOR(CoshOperator, std::cosh)
UNARY_ANGLE_OPERATOR(TanhOperator, std::tanh)
UNARY_ANGLE_OPERATOR(SinOperator, std::sin)
UNARY_ANGLE_OPERATOR(CosOperator, std::cos)
UNARY_ANGLE_OPERATOR(TanOperator, std::tan)

UNARY_OPERATOR(lnOperator, std::log)
UNARY_OPERATOR(Log10Operator, std::log10)
UNARY_OPERATOR(RootOperator, std::sqrt)
UNARY_OPERATOR(CubeRootOperator, std::cbrt)

BITWISE_UNARY_OPERATOR(NotOperator,			~,	)
BITWISE_UNARY_OPERATOR(ShiftLeftOperator,    , <<1)
BITWISE_UNARY_OPERATOR(ShiftRightOperator,	 , >>1)

///Custom unary operators
struct TenPowOperator : public UnaryOperator {
	inline Value evaluate(const Value &rhs) const override {
		return Value(static_cast<CalcInt>(std::pow(10, rhs.intVal)),	std::pow(10, rhs.floatVal), static_cast<CalcUInt>(std::pow(10, rhs.uIntVal)));
	}
};

struct RotateLeftOperator : public UnaryOperator {
	inline CalcUInt rotate(CalcUInt value) const {
		CalcUInt result = value << 1;
		result |= (value >> 63) & 0x1;
		return result;
	}

	inline Value evaluate(const Value &rhs) const override {
		const CalcUInt intVal = rotate(static_cast<CalcUInt>(rhs.intVal));
		const CalcUInt uIntVal = rotate(rhs.uIntVal);
		const CalcUInt fVal = rotate(*reinterpret_cast<const CalcUInt*>(&rhs.floatVal));

		return Value(intVal, fVal, uIntVal);
	}
};

struct RotateRightOperator : public UnaryOperator {
	inline CalcUInt rotate(CalcUInt value) const {
		CalcUInt result = value >> 1;
		result |= (value << 63) & 0x8000'0000'0000'0000;
		return result;
	}

	inline Value evaluate(const Value &rhs) const override {
		const CalcUInt intVal = rotate(static_cast<CalcUInt>(rhs.intVal));
		const CalcUInt uIntVal = rotate(rhs.uIntVal);
		const CalcUInt fVal = rotate(*reinterpret_cast<const CalcUInt*>(&rhs.floatVal));

		return Value(intVal, fVal, uIntVal);
	}
};

struct FactorialOperator : public UnaryOperator {
	template<class T>
	T factorial(T val) const {
		T result = 1;
		for(int i = 2; i <= static_cast<int>(val); ++i) {
			result *= i;
		}
		return result;
	}

	inline Value evaluate(const Value &rhs) const override {
		return Value(factorial(rhs.intVal),	factorial(rhs.floatVal), factorial(rhs.uIntVal));
	}
};

struct ByteReverseOperator : public UnaryOperator {
	inline CalcUInt reverse(CalcUInt value) const {
		CalcUInt result = 0;
		result |= (value << 56) & 0xFF00'0000'0000'0000;
		result |= (value << 40) & 0x00FF'0000'0000'0000;
		result |= (value << 24) & 0x0000'FF00'0000'0000;
		result |= (value << 8 ) & 0x0000'00FF'0000'0000;

		result |= (value >> 56) & 0x0000'0000'0000'00FF;
		result |= (value >> 40) & 0x0000'0000'0000'FF00;
		result |= (value >> 24) & 0x0000'0000'00FF'0000;
		result |= (value >> 8 ) & 0x0000'0000'FF00'0000;

		return result;
	}

	inline Value evaluate(const Value &rhs) const override {
		const CalcUInt intVal = reverse(static_cast<CalcUInt>(rhs.intVal));
		const CalcUInt uIntVal = reverse(rhs.uIntVal);
		const CalcUInt fVal = reverse(*reinterpret_cast<const CalcUInt*>(&rhs.floatVal));

		return Value(intVal, fVal, uIntVal);
	}
};

struct SwapHalvesOperator : public UnaryOperator {
	inline CalcUInt reverse(CalcUInt value) const {
		CalcUInt result = 0;
		result |= (value << 32) & 0xFFFF'FFFF'0000'0000;
		result |= (value >> 32) & 0x0000'0000'FFFF'FFFF;

		return result;
	}

	inline Value evaluate(const Value &rhs) const override {
		const CalcUInt intVal = reverse(static_cast<CalcUInt>(rhs.intVal));
		const CalcUInt uIntVal = reverse(rhs.uIntVal);
		const CalcUInt fVal = reverse(*reinterpret_cast<const CalcUInt*>(&rhs.floatVal));

		return Value(intVal, fVal, uIntVal);
	}
};

struct PiOperator : public UnaryOperator {
	inline Value evaluate(const Value &) const override {
		return Value(3,	pi,	3);
	}
};

struct eOperator : public UnaryOperator {
	inline Value evaluate(const Value &) const override {
		return Value(2,	e, 2);
	}
};


struct ePowXOperator : public UnaryOperator {
	inline Value evaluate(const Value &rhs) const override {
		return Value(static_cast<CalcInt>(std::pow(e, rhs.intVal)),	std::pow(e, rhs.floatVal), static_cast<CalcUInt>(std::pow(e, rhs.uIntVal)));
	}
};

///Custom binary operators
//struct ExponentialNotation : public BinaryOperator {
//	inline Value evaluate(const Value& lhs, const Value& rhs) const override {
//		return Value(static_cast<CalcInt>(lhs.intVal * std::pow(10, rhs.intVal)), lhs.floatVal * std::pow(10, rhs.floatVal), static_cast<CalcUInt>(lhs.uIntVal * std::pow(10, rhs.uIntVal)));
//	}
//};

struct ModulusOperator : public BinaryOperator {
	inline ModulusOperator() : BinaryOperator(0) {}
	inline Value evaluate(const Value& lhs, const Value& rhs) const override {
		return Value(lhs.intVal % rhs.intVal, std::fmod(lhs.floatVal, rhs.floatVal), lhs.uIntVal % rhs.uIntVal);
	}
};

struct NRootOperator : public BinaryOperator {
	inline NRootOperator() : BinaryOperator(0) {}
	inline Value evaluate(const Value& lhs, const Value &rhs) const override {
		return Value(static_cast<CalcInt>(std::pow(lhs.intVal, 1.0/rhs.intVal)),	std::pow(lhs.floatVal, 1.0/rhs.floatVal), static_cast<CalcUInt>(std::pow(lhs.uIntVal, 1.0/rhs.uIntVal)));
	}
};

struct LogNOperator : public BinaryOperator {
	inline LogNOperator() : BinaryOperator(0) {}
	inline Value evaluate(const Value& lhs, const Value &rhs) const override {
		return Value(static_cast<CalcInt>(std::log(lhs.intVal)/std::log(rhs.intVal)), std::log(lhs.floatVal)/std::log(rhs.floatVal), static_cast<CalcUInt>(std::log(lhs.uIntVal)/std::log(rhs.uIntVal)));
	}
};
