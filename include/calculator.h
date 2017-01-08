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

#include <QObject>

#include <memory>
#include <QString>
#include <random>
#include <vector>
#include "operators.h"
#include "modes.h"

enum class CalcStackType { Value, Operator, Bracket };

struct CalcStackItem {
	explicit CalcStackItem(const Value& val);
	explicit CalcStackItem(std::unique_ptr<Operator> operater);
	explicit CalcStackItem();

	Value val;
	std::unique_ptr<Operator> operater;
	CalcStackType type;
};

class Calculator : public QObject {
	Q_OBJECT
	public:
		Calculator();

		void setMode(Mode mode);
		void setAngleMode(AngleMode mode);
		inline AngleMode getAngleMode() {return angleMode;}

		///User input
		void setInput(const Value& value);
		void inputEquals();
		void inputOperator(std::unique_ptr<Operator> operater);
		void inputRandomNumber();
		void inputC();
		void inputOpenBracket();
		void inputCloseBracket();

	signals:
		void displayValueChanged(const Value& value, bool userInput);
		void nextInputClearsChanged(bool inputClears);

	private:
		Mode mode {Mode::TwosComp};
		AngleMode angleMode {AngleMode::Degrees};
		Width width {Width::SixtyFour};

		std::mt19937 randgen;
		std::vector<CalcStackItem> stack;

		void evaluate(std::vector<CalcStackItem>::iterator from, std::vector<CalcStackItem>::iterator to);
};
