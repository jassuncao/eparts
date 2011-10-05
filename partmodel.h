#ifndef PARTMODEL_H
#define PARTMODEL_H
#include <QList>
#include "partparameter.h"
#include "parttype.h"

class PartModel
{    
public:
    PartModel();
    bool load(const int partId);
    inline PartParameter::ParameterType valueType() const {return (PartParameter::ParameterType)_partType.valueType.get().toUInt();}
    inline const QList<PartParameter> * parameters() const { return &_parameters;}
private:
    PartType _partType;
    QList<PartParameter> _parameters;

};

#endif // PARTMODEL_H
