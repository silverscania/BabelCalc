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
#include <cmath>
#include <gtest/gtest.h>

#include "input.h"

TEST(TestInput, teststringToFloatBase10Units)
{
	bool ok = false;
	EXPECT_EQ(Input::stringToFloat(".0",   10, &ok), 0.0f);  EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("0",    10, &ok), 0.0f);	 EXPECT_TRUE(ok);

	EXPECT_EQ(Input::stringToFloat("1",    10, &ok), 1.0f);	 EXPECT_TRUE(ok);

	EXPECT_EQ(Input::stringToFloat("10.0", 10, &ok), 10.0f); EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("10.",  10, &ok), 10.0f); EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("10",   10, &ok), 10.0f); EXPECT_TRUE(ok);
}

TEST(TestInput, teststringToFloatBase10Fractions)
{
	bool ok = false;
	EXPECT_EQ(Input::stringToFloat(".1",             10, &ok), 0.1f);
	EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("00.2",           10, &ok), 0.2f);
	EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("0.3",            10, &ok), 0.3f);
	EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("0.456",          10, &ok), 0.456f);
	EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("10.123456789",   10, &ok), 10.123456789f);
	EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("987654.3212345", 10, &ok), 987654.3212345f);
	EXPECT_TRUE(ok);
}

TEST(TestInput, teststringToFloatBase3Units)
{
	bool ok = false;
	EXPECT_EQ(Input::stringToFloat(".0",   3, &ok), 0.0f);  EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("0",    3, &ok), 0.0f);	 EXPECT_TRUE(ok);

	EXPECT_EQ(Input::stringToFloat("1",    3, &ok), 1.0f);	 EXPECT_TRUE(ok);

	EXPECT_EQ(Input::stringToFloat("10.0", 3, &ok), 3.0f);  EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("10.",  3, &ok), 3.0f);  EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("10",   3, &ok), 3.0f);  EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("10.0", 3, &ok), 3.0f);  EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("10.",  3, &ok), 3.0f);  EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("10",   3, &ok), 3.0f);  EXPECT_TRUE(ok);
}

TEST(TestInput, teststringToFloatBase3Fractions)
{
	bool ok = false;
	EXPECT_FLOAT_EQ(Input::stringToFloat(".1",    3, &ok), 0.3333333333333f);
	EXPECT_TRUE(ok);
	EXPECT_FLOAT_EQ(Input::stringToFloat("00.2",  3, &ok), 0.6666666666666f);
	EXPECT_TRUE(ok);
	EXPECT_FLOAT_EQ(Input::stringToFloat("00.21", 3, &ok), 0.7777777777777f);
	EXPECT_TRUE(ok);
	EXPECT_FLOAT_EQ(Input::stringToFloat(".121112", 3, &ok), 0.61179698216f);
	EXPECT_TRUE(ok);

	EXPECT_FLOAT_EQ(Input::stringToFloat("20.2",  3, &ok), 6.6666666666666f);
	EXPECT_TRUE(ok);
	EXPECT_FLOAT_EQ(Input::stringToFloat("1.21", 3, &ok), 1.7777777777777f);
	EXPECT_TRUE(ok);
	EXPECT_FLOAT_EQ(Input::stringToFloat("10002.121112", 3, &ok), 83.61179698216f);
	EXPECT_TRUE(ok);

	EXPECT_EQ(Input::stringToFloat("0.3",   3, &ok), 0.0f);
	EXPECT_FALSE(ok);
	EXPECT_EQ(Input::stringToFloat("0.456", 3, &ok), 0.0f);
	EXPECT_FALSE(ok);
}

TEST(TestInput, testEmptyString)
{
	bool ok = false;
	EXPECT_EQ(Input::stringToFloat("",     2,  &ok), 0.0f);  EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("",     3,  &ok), 0.0f);  EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("",     10, &ok), 0.0f);  EXPECT_TRUE(ok);
	EXPECT_EQ(Input::stringToFloat("",     26, &ok), 0.0f);  EXPECT_TRUE(ok);
}
