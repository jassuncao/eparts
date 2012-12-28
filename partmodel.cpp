#include "partmodel.h"
#include "database/database.h"



PartModel::PartModel(QObject * parent) : QObject(parent)
{
}

enum FIXED_FIELDS {Quantity=0, MinimumQuantity,FIXED_FIELDS_LEN};

bool PartModel::load(const int partId)
{    
    _partType.load(DQWhere("id")==partId);
    //_tableName = EParts::Database::makeTableName(_partType.partTableName);
    _parameters.clear();
    /*
    _fieldNames.clear();
    _fieldNames<<tr("Quantity");
    _fieldNames<<tr("Minimum Quantity");
    */
    DQQuery<PartParameter> parametersQuery;
    parametersQuery = parametersQuery.filter(DQWhere("partType") == partId).orderBy("orderIndex");
    if(parametersQuery.exec()){
        while(parametersQuery.next()){
            PartParameter param = parametersQuery.record();
            _parameters.append(param);
            //_fieldNames.append(param.name.get().toString());
        }
    }
    else{
        return false;
    }
    return true;
}

QString PartModel::tableSchemaSql() const
{
    QString statement = QString("CREATE TABLE IF NOT EXISTS %1 (\n%2\n);");

    QStringList columnDefList;
    columnDefList << "id INTEGER PRIMARY KEY";
    columnDefList << "quantity INT";
    columnDefList << "minimum_quantity INT";    
    QList<PartParameter>::const_iterator it;
    for(it=_parameters.constBegin();it!=_parameters.constEnd();++it)
    {
        QString columnType = (*it).isText() ? "TEXT" : "DOUBLE";
        QString columnDef = QString("%1 %2")
                                    .arg((*it).columnName)
                                    .arg(columnType);
        columnDefList << columnDef;
    }
    QString sql;
    sql = statement
            .arg(_tableName)
            .arg(columnDefList.join(",\n"));
    return sql;
}

QString PartModel::selectAllSql(int offset, int limit, int orderColumn, Qt::SortOrder order) const
{
    QString statement = QString("SELECT %1 FROM %2 ORDER BY :orderField :order LIMIT :limit OFFSET :offset;");
    QStringList fieldsList;
    fieldsList << "id";
    fieldsList << "quantity";
    fieldsList << "minimum_quantity";
    QList<PartParameter>::const_iterator it;
    for(it=_parameters.constBegin();it!=_parameters.constEnd();++it)
    {
        fieldsList << (*it).columnName;
    }
    QString sql;
    sql = statement
            .arg(_tableName)
            .arg(fieldsList.join(","));
    QSqlQuery query;
    query.prepare(sql);
    return sql;
}

/*
PartParameter::ParameterType valueType() const
{
    return (PartParameter::ParameterType)partType.valueType.get().toUInt();
}

const QList<PartParameter> parameters() const
{
    return _parameters;
}
*/
