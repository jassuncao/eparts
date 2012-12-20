#ifndef DATABASE_H
#define DATABASE_H

//#include "types.h"

class DQConnection;
class QString;
class PartType;

namespace EParts {

    class Database
    {
    private:
        Database();
    public:
        /*
        static QList<Parameter> * fetchPartTypeParameters(const int partTypeId);
        static QList<Part> * fetchParts(const int partTypeId);
        */
        static bool initTables(DQConnection &connection);
        static QString makeTableName(const QString& partName);
        static QString generateTableSchema(const PartType& partType);
    };
}

#endif // DATABASE_H
