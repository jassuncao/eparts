#include <QtGui/QApplication>
#include "mainwindow.h"
#include "connection.h"

#include <QtCore/QCoreApplication>
#include <dquest.h>
#include <QSqlDatabase>

#include "category.h"
#include "parttype.h"
#include "partparameter.h"
#include "part.h"
#include "parametervalue.h"
#include "database.h"
#include "AutoTest.h"

#if 0
// This is all you need to run all the tests
TEST_MAIN
#else
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DQConnection connection;
    EParts::Database::initTables(connection);
    if (!EParts::Database::initTables(connection))
            return 1;
    MainWindow w;
    w.show();
    int res = a.exec();
    connection.close();
    return res;
}
#endif

