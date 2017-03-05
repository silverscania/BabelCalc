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

QTEST_MAIN(VariableSizeTest)
#include "variablesizetest.moc"
