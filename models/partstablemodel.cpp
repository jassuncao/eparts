#include "partstablemodel.h"
#include "database/partparameter.h"
#include "database/parametervalue.h"
#include "database/part.h"
#include <QList>
#include <QtSql>
#include <dquest.h>
#include "unitformatter.h"
#include "unitparser.h"


//using namespace EParts;

 PartsTableModel2::PartsTableModel2(QObject *parent) :
     QAbstractTableModel(parent)
 {

 }

 PartsTableModel2::~PartsTableModel2()
 {
     PartsTableRow* row;
     foreach(row,_rows){
         delete row;
     }
     _rows.clear();
 }

 int PartsTableModel2::rowCount(const QModelIndex &parent) const
 {
     return _rows.count();
 }

 int PartsTableModel2::columnCount(const QModelIndex &parent) const
 {
     return _partTypeModel.fields().count();
 }

 QVariant  PartsTableModel2::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();
     int row = index.row();
     int column = index.column();
     if(row<0 || row>=_rows.count())
         return QVariant();
     if(role!=Qt::DisplayRole && role!=Qt::EditRole)
         return QVariant();
     PartsTableRow * tableRow = _rows.at(row);
     if(!tableRow->isLoaded())
         tableRow->load();
     QVariant fieldValue = _partTypeModel.fieldValue(column,tableRow);
     PartParameter::ParameterType paramType = _partTypeModel.fieldType(column);
     if(PartParameter::isText(paramType))
         return fieldValue;
     //Don't include the unit for  edit purposes
     return UnitFormatter::formatParameter(paramType,fieldValue.toDouble(), role==Qt::DisplayRole);
 }

 bool PartsTableModel2::setData(const QModelIndex &index, const QVariant &value, int role)
 {
     if (!index.isValid())
         return false;
     int row = index.row();
     int column = index.column();
     if(row<0 || row>=_rows.count())
         return false;
     if(role!=Qt::EditRole)
         return false;
     PartsTableRow * tableRow = _rows.at(row);
     if(!tableRow->isLoaded())
         tableRow->load();
     qDebug()<<"Setting field "<<column<<" with value "<<value;
     PartParameter::ParameterType paramType = _partTypeModel.fieldType(column);
     bool result = false;
     if(PartParameter::isText(paramType)) {
        _partTypeModel.setFieldValue(column,tableRow, value);
        result = true;
     }
     else{
         bool ok;
         double numericValue = UnitParser::parseUnit(value.toString(),&ok);
         if(ok){
            _partTypeModel.setFieldValue(column,tableRow, numericValue);
            result = true;
         }

     }
     if(result)
         emit dataChanged(index, index);
     return result;
 }

 QVariant  PartsTableModel2::headerData(int section, Qt::Orientation orientation, int role) const
 {
     if (role != Qt::DisplayRole || orientation!=Qt::Horizontal)
         return QVariant();
     const QStringList & fields = _partTypeModel.fields();
     if(section<fields.count())
         return fields.at(section);
     return QVariant();
 }

 /*
 PartRow * PartsTableModel2::rowData(const QModelIndex &index) const
 {

 }
*/
 Qt::ItemFlags PartsTableModel2::flags ( const QModelIndex & index ) const
 {
     if(index.isValid()){
         return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
     }
     return Qt::NoItemFlags;
 }

 void PartsTableModel2::load(int partType)
 {
     beginResetModel();
     _partTypeModel.load(partType);
     _rows.clear();

     DQQuery<Part> partsQuery;
     partsQuery = partsQuery.filter(DQWhere("partType")==partType);
     if(partsQuery.exec()){
         while(partsQuery.next()){
             Part part = partsQuery.record();
             PartsTableRow * row = new PartsTableRow(part);
             _rows.append(row);
         }
     }
     endResetModel();
 }

 void sort(int column, Qt::SortOrder order)
 {

 }

