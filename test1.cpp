#include "test1.h"
#include "unitformatter.h"

void Test1::initTestCase()
{
}

void Test1::test1()
{
    QString res = UnitFormatter::formatCapacitance(4700);
    QString expected("4.7kF");
    QCOMPARE(res, expected);    

    res = UnitFormatter::formatCapacitance(4780000);
    expected = QString("4.78MF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatCapacitance(4780000000.0);
    expected = QString("4.78GF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatCapacitance(47000);
    expected = QString("47kF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatCapacitance(0.1);
    expected = QString("100mF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatCapacitance(0.001);
    expected = QString("1mF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatCapacitance(0.0000001);
    expected = QString("100nF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatCapacitance(0.000000001);
    expected = QString("10pF");
    QCOMPARE(res, expected);
}

/*
void Test1::test2()
{
    QVERIFY(1 == 0);
}
*/

void Test1::cleanupTestCase()
{
}
