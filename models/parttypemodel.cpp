#include "parttypemodel.h"

enum FIXED_FIELDS {Quantity, MinimumQuantity,Notes,Datasheet,URL};

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
        case Quantity:
            return PartParameter::GenericNumber;
        case MinimumQuantity:
            return PartParameter::GenericNumber;
        case Notes:
            return PartParameter::LongText;
        case Datasheet:
            return PartParameter::Text;
        case URL:
            return PartParameter::Text;
    }
    fieldIndex = fieldIndex-5;
    return (PartParameter::ParameterType)_parameters[fieldIndex].type.get().toUInt();
}

QVariant PartTypeModel::fieldValue(int fieldIndex, const PartsTableRow * tableRow) const
{
    switch(fieldIndex)   {
        case Quantity:
            return tableRow->constPart().quantity.get();
        case MinimumQuantity:
            return tableRow->constPart().minimumQuantity.get();
        case Notes:
            return tableRow->constPart().notes.get();
        case Datasheet:
            return tableRow->constPart().dataSheet.get();
        case URL:
            return tableRow->constPart().url.get();
    }
    fieldIndex = fieldIndex-5;
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
