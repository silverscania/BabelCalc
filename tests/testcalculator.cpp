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

#include <gtest/gtest.h>

#include "value.h"
#include "calculator.h"

/**
 * Test harness to enable testing the Calculator class
 * without any of the GUI stuff.
 */
class CalculatorTest : public ::testing::Test, public QObject{
public:
	Calculator calculator;
	Value lastDisplayValue;
	bool lastUserInputValue;

	virtual void SetUp() {
		connect(&calculator, &Calculator::displayValueChanged, this, &CalculatorTest::calculatorDisplayValueChanged);
	}

private slots:
	void calculatorDisplayValueChanged(const Value& value, bool userInput)
	{
		lastDisplayValue = value;
		lastUserInputValue = userInput;
	}
};

/**
 * Tests follow
 */

TEST_F(CalculatorTest, TestSubtract) {
	calculator.setInput(Value(100, 0.0f, 0));
	calculator.inputOperator(std::make_unique<SubtractionOperator>());
	calculator.setInput(Value(2, 0.0f, 0));
	calculator.inputEquals();

	EXPECT_EQ(lastDisplayValue.intVal, 98);
}

