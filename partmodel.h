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
    inline const QList<PartParameter> * parameters() const { return &_parameters;}
private:
    PartType _partType;
    QList<PartParameter> _parameters;

};

#endif // PARTMODEL_H
