#include <QtGui/QApplication>
#include "mainwindow.h"
#include "connection.h"

#include <QtCore/QCoreApplication>
#include <dquest.h>
#include <QSqlDatabase>

#include "database/category.h"
#include "database/parttype.h"
#include "database/partparameter.h"
#include "database/part.h"
//#include "database/parametervalue.h"
#include "database/database.h"
#include "AutoTest.h"

#if 0
// This is all you need to run all the tests
TEST_MAIN
#else
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DQConnection connection;    
    if (!Database::Database::initTables(connection))
            return 1;
    Database::Database::generateMockData();
    MainWindow w;
    w.show();
    int res = a.exec();
    connection.close();
    return res;
}
#endif

