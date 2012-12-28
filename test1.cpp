#include "test1.h"
#include "unitformatter.h"
#include "unitparser.h"
#include "database/database.h"
#include "partmodel.h"

void Test1::initTestCase()
{
}

void Test1::test1()
{
    QString res = UnitFormatter::formatParameter(PartParameter::Capacitance, 4700);
    QString expected("4,7kF");
    QCOMPARE(res, expected);    

    res = UnitFormatter::formatParameter(PartParameter::Capacitance, 4780000);
    expected = QString("4,78MF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatParameter(PartParameter::Capacitance, 4780000000.0);
    expected = QString("4,78GF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatParameter(PartParameter::Capacitance, 47000);
    expected = QString("47kF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatParameter(PartParameter::Capacitance, 0.1);
    expected = QString("100mF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatParameter(PartParameter::Capacitance, 0.001);
    expected = QString("1mF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatParameter(PartParameter::Capacitance, 0.0000001);
    expected = QString("100nF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatParameter(PartParameter::Capacitance, 0.00000000001);
    expected = QString("10pF");
    QCOMPARE(res, expected);

    res = UnitFormatter::formatParameter(PartParameter::Percentage,0);
    expected = QString("0%");
    QCOMPARE(res, expected);
}

bool assertDouble(double value, double expected, double delta)
{
    return (fabs(value-expected))<=delta;
}

void Test1::test2()
{    
    bool ok;
    double value;
    value = UnitParser::parseUnit(" 0R1 ", &ok);
    QVERIFY2(assertDouble(value, 0.1,0.001),"0R1 Failed");
    value = UnitParser::parseUnit(" 1k2 ", &ok);
    QVERIFY2(assertDouble(value, 1200,0.001)," 1k2 Failed");
    value = UnitParser::parseUnit(" 1,2k ", &ok);
    QVERIFY2(assertDouble(value, 1200,0.001)," 1,2k Failed");
    value = UnitParser::parseUnit("100n", &ok);
    QVERIFY2(assertDouble(value, 100*1E9,100*1E10),"100n Failed");
}

void Test1::test3()
{    
    /*
    DQConnection connection;
    EParts::Database::initTables(connection);
    PartModel partModel;
    partModel.load(1);
    QString sql = partModel.tableSchemaSql();
    QString expected("CREATE TABLE IF NOT EXISTS fixed_resistors (\nid INTEGER PRIMARY KEY,\nquantity INT,\nminimum_quantity INT,\nresistance DOUBLE,\npower DOUBLE,\ntolerance DOUBLE,\nnotes TEXT\n);");
    QCOMPARE(sql,expected);
    */
}





void Test1::cleanupTestCase()
{
}
