#include "partstablerow.h"

PartsTableRow::PartsTableRow(Part part) :
    _part(part), _loaded(false)
{}

void PartsTableRow::load()
{
    qDebug()<<"Loading row data "<<_part.id;
    DQQuery<ParameterValue> valuesQuery;
    valuesQuery = valuesQuery.filter(DQWhere("part")==_part.id);
    if(valuesQuery.exec()){
        while(valuesQuery.next()){
            ParameterValue paramValue = valuesQuery.record();
            int paramId = paramValue.partParameter.get().toInt();
            _paramValues[paramId] = paramValue;
        }
    }
    _loaded = true;
}

void PartsTableRow::save()
{

}
