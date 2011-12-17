#include "parttypemodel.h"

enum FIXED_FIELDS {Quantity=0, MinimumQuantity,FIXED_FIELDS_LEN};

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
        case Quantity:
            return PartParameter::GenericNumber;
        case MinimumQuantity:
            return PartParameter::GenericNumber;       
    }
    fieldIndex = fieldIndex-FIXED_FIELDS_LEN;
    return (PartParameter::ParameterType)_parameters[fieldIndex].type.get().toUInt();
}

QVariant PartTypeModel::fieldValue(int fieldIndex, const PartsTableRow * tableRow) const
{
    switch(fieldIndex)   {
        case Quantity:
            return tableRow->constPart().quantity.get();
        case MinimumQuantity:
            return tableRow->constPart().minimumQuantity.get();       
    }
    fieldIndex = fieldIndex-FIXED_FIELDS_LEN;
    PartParameter partParam = _parameters[fieldIndex];
    int paramId = partParam.id.get().toInt();
    const ParameterValue paramValue = tableRow->constParamValue(paramId);
    if(partParam.isText()){
        return paramValue.textValue.get();
    }
    else{
        return paramValue.numericValue.get();
    }
}

void PartTypeModel::setFieldValue(int fieldIndex, PartsTableRow * tableRow, QVariant value)
{

}
