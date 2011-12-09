#ifndef PARTTYPEMODEL_H
#define PARTTYPEMODEL_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include "parttype.h"
#include "partparameter.h"


class PartTypeModel : QObject
{
    Q_OBJECT
public:
    explicit PartTypeModel(QObject * parent=0);
    inline QString name() const { return _partType.name;}
    bool load(const int partTypeId);
    inline const QStringList & fields() const { return _fieldNames;}
    PartParameter::ParameterType fieldType(int fieldIndex) const;
private:
    PartType _partType;
    QStringList _fieldNames;
    QList<PartParameter> _parameters;
};


#endif // PARTTYPEMODEL_H
