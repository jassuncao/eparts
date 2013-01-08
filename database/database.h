#ifndef DATABASE_H
#define DATABASE_H

#include <QList>
#include <QtSql>

class DQConnection;

namespace Database {

    class Database
    {
    private:
        Database();
    public:
        static bool initTables(DQConnection &connection);
        static void generateMockData();
    };
}

#endif // DATABASE_H
