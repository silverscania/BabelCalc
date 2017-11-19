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

using namespace std;

#define SETUP(valA, valB) \
	CalcUInt a, b, c; \
	uint32_t smallA, smallB, smallC; \
	a = valA; \
	b = valB; \
	smallA = static_cast<uint32_t>(a); \
	smallB = static_cast<uint32_t>(b); \


#define SETUP_SIGNED(valA, valB) \
	CalcInt a, b, c; \
	int32_t smallA, smallB, smallC; \
	a = valA; \
	b = valB; \
	smallA = static_cast<int32_t>(a); \
	smallB = static_cast<int32_t>(b); \

/**
 * Set of tests to attempt to determine if it's neccessary
 * to use separate 32bit and 64bit data types internally,
 * or whether 64 bits can always be used, and to obtain the
 * 32 bit value it can just be cast.
 *
 * If all of these tests pass (which they do so far),
 * then the internal 32bit value can be eliminated. The only
 * thing left to work out is whether they cover all cases...
 *
 * For example does multiplying two 32bit numbers together
 * (with overflow) equal the same 64bit numbers multiplied
 * together and then cast to a 32bit value.
 */
class VariableSizeTest: public QObject
{
	Q_OBJECT
private slots:
	void initTestCase()
	{
		qDebug("called before everything else");
	}

	void oneBigOneSmall()
	{
		SETUP(5623897562389, 2362304);

		c = a * b;
		smallC = smallA * smallB;

		QVERIFY(static_cast<uint32_t>(c) == smallC);
	}

	void twoSmall()
	{
		SETUP(234, 2456);

		c = a * b;
		smallC = smallA * smallB;

		QVERIFY(static_cast<uint32_t>(c) == smallC);
	}

	void twoBig()
	{
		SETUP(234523523452345, 2345235286783452345);

		c = a * b;
		smallC = smallA * smallB;

		QVERIFY(static_cast<uint32_t>(c) == smallC);
	}

	void oneBigOneSmallPow()
	{
		SETUP(12345, 2);

		c = pow(a, b);
		smallC = pow(a, b);

		QVERIFY(static_cast<uint32_t>(c) == smallC);
	}

	void twoSmallPow()
	{
		SETUP(234, 2);

		c = pow(a, b);
		smallC = pow(smallA, smallB);

		QVERIFY(static_cast<uint32_t>(c) == smallC);
	}

	void twoBigPow()
	{
		SETUP(123, 5);

		c = pow(a, b);
		smallC = pow(smallA, smallB);

		QVERIFY(c != smallC);
		QVERIFY(static_cast<uint32_t>(c) == smallC);
	}

	void negatives()
	{
		SETUP_SIGNED(-0x7FFFFFFF, 10);

		c = a + b;
		smallC = smallA + smallB;

		QVERIFY(static_cast<int32_t>(c) == smallC);
	}

	void negatives2()
	{
		SETUP_SIGNED(0x7FFFFFFF, 0xFFFFFFFFFF);

		c = a - b;
		smallC = smallA - smallB;

		QVERIFY(static_cast<int32_t>(c) == smallC);
	}

};

//QTEST_MAIN(VariableSizeTest)
#include "variablesizetest.moc"
