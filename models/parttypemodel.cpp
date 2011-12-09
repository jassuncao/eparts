#include "parttypemodel.h"

PartTypeModel::PartTypeModel(QObject * parent) : QObject(parent)
{
}

bool PartTypeModel::load(const int partTypeId)
{
    _partType.load(DQWhere("id")==partTypeId);
    _fieldNames.clear();
    _parameters.clear();
    _fieldNames<<tr("Quantity");
    _fieldNames<<tr("Minimum Quantity");
    _fieldNames<<tr("Notes");
    _fieldNames<<tr("Datasheet");
    _fieldNames<<tr("URL");

    DQQuery<PartParameter> parametersQuery;
    parametersQuery = parametersQuery.filter(DQWhere("partType") == partTypeId).orderBy("orderIndex");
    if(parametersQuery.exec()){
        while(parametersQuery.next()){
            PartParameter param = parametersQuery.record();
            _parameters.append(param);
            _fieldNames.append(param.name);
        }
    }
    return true;
}

PartParameter::ParameterType PartTypeModel::fieldType(int fieldIndex) const
{
    switch(fieldIndex)   {
        case 0://Quantity
            return PartParameter::GenericNumber;
        case 1://Minimum Quantity
            return PartParameter::GenericNumber;
        case 2://Notes
            return PartParameter::LongText;
        case 3://Datasheet
            return PartParameter::Text;
        case 4://URL
            return PartParameter::Text;
    }
    fieldIndex = fieldIndex-5;
    return (PartParameter::ParameterType)_parameters[fieldIndex].type.get().toUInt();
}
