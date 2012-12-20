#ifndef TEST1_H
#define TEST1_H

#include "AutoTest.h"
#include <QObject>

class Test1 : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void test1();
    void test2();
    void test3();
    void cleanupTestCase();
};

DECLARE_TEST(Test1)

#endif // TEST1_H
