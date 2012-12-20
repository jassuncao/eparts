#ifndef PARTMODEL_H
#define PARTMODEL_H
#include <QObject>
#include <QList>
#include "database/partparameter.h"
#include "database/parttype.h"

class PartModel : QObject
{
    Q_OBJECT
public:
    explicit PartModel(QObject * parent=0);
    bool load(const int partId);    
    inline const QList<PartParameter> * parameters() const { return &_parameters;}
    //inline const QStringList * fieldNames() const { return &_fieldNames;}
    inline QString tableName() const { return _tableName;}
    QString tableSchemaSql() const;
    QString selectAllSql(int offset, int limit, int orderColumn, Qt::SortOrder order) const;
private:
    PartType _partType;
    QString _tableName;
    QList<PartParameter> _parameters;
    //QStringList _fieldNames;

};

#endif // PARTMODEL_H
