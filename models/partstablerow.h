#ifndef PARTSTABLEROW_H
#define PARTSTABLEROW_H

#include <QHash>
#include "part.h"
#include "partparameter.h"
#include "parametervalue.h"


class PartsTableRow {

public:
    PartsTableRow(Part part);
    inline bool isLoaded() const { return _loaded;}
    inline Part & part() { return _part;}
    inline const Part & constPart() const { return _part;}
    inline ParameterValue & paramValue(int paramId) { return _paramValues[paramId];}
    inline const ParameterValue constParamValue(int paramId) const { return _paramValues[paramId];}
    void load();
    void save();
private:
    Part _part;
    QHash<int,ParameterValue> _paramValues;
    bool _loaded;
};

#endif // PARTSTABLEROW_H
