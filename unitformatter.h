#ifndef UNITFORMATTER_H
#define UNITFORMATTER_H

#include <qchar.h>
#include <qstring.h>
#include "partparameter.h"

class UnitFormatter
{
private:
    const static ushort ohmSymbol = 0x2126;
    const static ushort microSymbol = 0x03BC;

    UnitFormatter();

public:
    static QString formatUnit(double value, QChar unit);

    inline static QString formatResistance(double value)
    {
        return formatUnit(value, QChar(ohmSymbol));
    }

    inline static QString formatCapacitance(double value)
    {
        return formatUnit(value, QChar('F'));
    }
    inline static QString formatInductance(double value)
    {
        return formatUnit(value, QChar('H'));
    }

    inline static QString formatPower(double value)
    {
        return formatUnit(value, QChar('W'));
    }

    static QString formatPercentage(double value);

    static QString format(PartParameter::ParameterType paramType, double value);

};

#endif // UNITFORMATTER_H
