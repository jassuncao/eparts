#ifndef UNITPARSER_H
#define UNITPARSER_H

#include <qstring.h>

class UnitParser
{
private:
    UnitParser();
public:
    static double parseUnit(const QString& num, bool * ok=0);
};

#endif // UNITPARSER_H
