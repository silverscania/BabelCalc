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

#include <QtTest/QtTest>
#include "value.h"
#include "calculator.h"

using namespace std;

/**
 * Test the calculator class without any of the GUI stuff
 */
class CalculatorTest: public QObject
{
	Q_OBJECT

	//Keep this for all tests. It's better to test that
	//actions don't have any side effects than to test
	//a brand new Calculator for every test.
	Calculator calculator;
	Value lastDisplayValue;
	bool lastUserInputValue;

private slots:
	void calculatorDisplayValueChanged(const Value& value, bool userInput)
	{
		lastDisplayValue = value;
		lastUserInputValue = userInput;
	}

	void initTestCase()
	{
		qDebug("called before everything else");
		//connect(calculator, &Calculator::displayValueChanged, this, &CalculatorTest::calculatorDisplayValueChanged);
	}

	void oneBigOneSmall()
	{
		Value input(100, 0.0f, 0);
		calculator.setInput(Value(100, 0.0f, 0));
		calculator.inputOperator(std::make_unique<SubtractionOperator>());
		calculator.setInput(Value(2, 0.0f, 0));
		calculator.inputEquals();

		Q_ASSERT(lastDisplayValue.intVal == 98);
	}
};

QTEST_MAIN(CalculatorTest)
#include "calculatortest.moc"
