#ifndef UNITFORMATTER_H
#define UNITFORMATTER_H

#include <qchar.h>
#include <qstring.h>
#include "partparameter.h"

class UnitFormatter
{
private:


    UnitFormatter();

public:
    const static ushort ohmSymbol = 0x2126;
    const static ushort microSymbol = 0x03BC;
    const static QChar resistance;
    const static QChar capacitance;
    const static QChar inductance;
    const static QChar power;
    const static QChar percentage;

    static QString format(double value);
    static QString format(double value, QChar unit);

    inline static QString formatResistance(double value)
    {
        return format(value, QChar(ohmSymbol));
    }

    inline static QString formatCapacitance(double value)
    {
        return format(value, QChar('F'));
    }
    inline static QString formatInductance(double value)
    {
        return format(value, QChar('H'));
    }

    inline static QString formatPower(double value)
    {
        return format(value, QChar('W'));
    }

    static QString formatPercentage(double value);

    static QString format(PartParameter::ParameterType paramType, double value);

    static QChar getUnitSymbol(PartParameter::ParameterType paramType);

};

#endif // UNITFORMATTER_H
