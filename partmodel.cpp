#include "partmodel.h"

PartModel::PartModel()
{
}

bool PartModel::load(const int partId)
{    
    _partType.load(DQWhere("id")==partId);
    _parameters.clear();

    DQQuery<PartParameter> parametersQuery;
    parametersQuery = parametersQuery.filter(DQWhere("partType") == partId).orderBy("orderIndex");
    if(parametersQuery.exec()){
        while(parametersQuery.next()){
            PartParameter param = parametersQuery.record();
            _parameters.append(param);
        }
    }
    else{
        return false;
    }
    return true;
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
