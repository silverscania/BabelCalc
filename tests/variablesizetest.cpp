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

/**
 * Test that we do need separate 32bit and 64bit types
 * We could just do all operations on 64bit and cast and the
 * result, but any failing tests here prove that won't work.
 *
 * Don't even bother testing floats == doubles because they
 * would all fail.
 */

using namespace std;

#define SETUP(valA, valB) \
	CalcUInt a, b, c; \
	uint32_t smallA, smallB, smallC; \
	a = valA; \
	b = valB; \
	smallA = static_cast<uint32_t>(a); \
	smallB = static_cast<uint32_t>(b); \
\
	CalcInt sa, sb, sc; \
	int32_t ssmallA, ssmallB, ssmallC; \
	sa = valA; \
	sb = valB; \
	ssmallA = static_cast<int32_t>(sa); \
	ssmallB = static_cast<int32_t>(sb);
	//CalcFloat fa, fb, fc; \
	//float fsmallA, fsmallB, fsmallC; \
	//fa = valA; \
	//fb = valB; \
	//fsmallA = static_cast<float>(fa); \
	//fsmallB = static_cast<float>(fb);

#define OP(op) \
	c = a op b; \
	smallC = smallA op smallB; \
	sc = sa op sb; \
	ssmallC = ssmallA op ssmallB;
	//fc = fa op fb; \
	//fsmallC = fsmallA op fsmallB;

#define FUNC(func) \
	c = func(a, b); \
	smallC = func(smallA, smallB); \
	sc = func(sa, sb); \
	ssmallC = func(ssmallA, ssmallB);

#define CHECK() \
	QCOMPARE(static_cast<uint32_t>(c), smallC); \
	QCOMPARE(static_cast<int32_t>(sc), ssmallC);
	//QVERIFY(static_cast<float>(fc) != fsmallC);

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
		OP(*);
		CHECK();
	}

	void twoSmall()
	{
		SETUP(234, 2456);
		OP(*);
		CHECK();
	}

	void twoBig()
	{
		SETUP(234523523452345, 2345235286783452345);
		OP(*);
		CHECK();
	}

	void oneBigOneSmallPow()
	{
		SETUP(12345, 2);
		FUNC(pow);
		CHECK();
	}

	void twoSmallPow()
	{
		SETUP(234, 2);
		FUNC(pow);
		CHECK();
	}

	/**
	 * @brief This one fails
	 */
	void twoBigPow()
	{
		SETUP(123, 5);
		FUNC(pow);
		CHECK();
	}

	void negatives()
	{
		SETUP(-0x7FFFFFFF, 10);
		OP(+);
		CHECK();
	}

	void negatives2()
	{
		SETUP(0x7FFFFFFF, 0xFFFFFFFFFF);
		OP(-);
		CHECK();
	}

	void negatives3()
	{
		SETUP(0x7FFFFFFF, -0xFFFFFFFFFF);
		OP(-);
		CHECK();
	}
};

QTEST_MAIN(VariableSizeTest)
#include "variablesizetest.moc"
