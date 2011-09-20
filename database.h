#ifndef DATABASE_H
#define DATABASE_H

//#include "types.h"

class DQConnection;

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
    };
}

#endif // DATABASE_H
