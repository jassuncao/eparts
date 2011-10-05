#include "partstablemodel.h"
#include "partparameter.h"
#include "parametervalue.h"
#include "part.h"
#include <QList>
#include <QtSql>
#include <dquest.h>
#include "unitformatter.h"


//using namespace EParts;

class PartRow{
public:
    PartRow(Part part) : part(part), loaded(false)
    {}
    Part part;
    bool loaded;
    QHash<int,ParameterValue> paramValues;
};

PartsTableModel::PartsTableModel(const PartModel * partModel, QObject *parent) :
    QAbstractTableModel (parent), _partModel(partModel)
{
}

PartsTableModel::~PartsTableModel()
{
}


void PartsTableModel::loadRowData(PartRow * row) const
{
    qDebug()<<"Loading row data "<<row->part.id;
    DQQuery<ParameterValue> valuesQuery;
    int partId = row->part.id;
    valuesQuery = valuesQuery.filter(DQWhere("part")==partId);
    if(valuesQuery.exec()){
        while(valuesQuery.next()){
            ParameterValue paramValue = valuesQuery.record();
            int paramId = paramValue.partParameter.get().toInt();
            qDebug()<<"ParamId="<<paramId;
            row->paramValues[paramId] = paramValue;
        }
        row->loaded=true;
    }
}


void PartsTableModel::load()
{
    _columnNames.clear();
    _rows.clear();
    _columnNames.append("Quantity");
    _columnNames.append("Value");

    const QList<PartParameter>  * parameters = _partModel->parameters();
    for (int i = 0; i < parameters->size(); ++i) {
        PartParameter param = parameters->at(i);
        _columnNames.append(param.name);
    }

    DQQuery<Part> partsQuery;
    if(partsQuery.exec()){        
        while(partsQuery.next()){
            Part part = partsQuery.record();
            PartRow * row = new PartRow(part);
            _rows.append(row);
        }
    }
}

int PartsTableModel::rowCount(const QModelIndex &parent) const
{
     Q_UNUSED(parent);
     return _rows.size();
}

 int PartsTableModel::columnCount(const QModelIndex &parent) const
 {
     Q_UNUSED(parent);     
     return _columnNames.size();
 }

 QVariant PartsTableModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= _rows.size() || index.row() < 0)
         return QVariant();

     if (role == Qt::DisplayRole) {
         PartRow * row = _rows.at(index.row());
         if(!row->loaded)
             loadRowData(row);
         int column = index.column();
         if(column==0){
             return row->part.quantity;
         }
         else if(column==1){
             PartParameter::ParameterType type = _partModel->valueType();
            if(type==PartParameter::Text){
                return row->part.textValue;
            }
            else{               
                return UnitFormatter::format(type, row->part.numericValue);
            }
         }
         else{
             PartParameter param = _partModel->parameters()->at(column-2);
             ParameterValue value = row->paramValues.value(param.id);
             if(param.type==PartParameter::Text){
                 return value.textValue;
             }             
             else{
                 PartParameter::ParameterType type = (PartParameter::ParameterType)param.type.get().toUInt();
                 return UnitFormatter::format(type, value.numericValue);
             }
         }
     }
     return QVariant();
 }

 QVariant PartsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
     if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal) {
         if(section>=0 && section < _columnNames.size())
         {             
            return _columnNames.at(section);
         }
         else{
             return QVariant();
         }
     }
     return QVariant();
 }

/*
 PartRow::PartRow(int id, const QString * items) : _id(id), _items(items), isLoaded(false)
 {
 }

 PartRow::~PartRow()
 {
     if(_items)
         delete[] _items;
 }
 */
