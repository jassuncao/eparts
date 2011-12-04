#include "parttypetablemodel.h"
#include <QtDebug>

const int COLUMN_COUNT = 3;

PartTypeTableModel::PartTypeTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    _columnNames<<tr("Name")<<tr("Category")<<tr("Description");
    DQQuery<Category> query;
    if(query.exec()){
        Category cat;
        while(query.next()){
            query.recordTo(cat);
            _categoriesMap[cat.id]=cat.name;
        }
    }
}


int PartTypeTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _rows.size();
}

int PartTypeTableModel::columnCount(const QModelIndex &parent) const
{
    return COLUMN_COUNT;
}

QVariant PartTypeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal){
        if(section>=0 && section<_columnNames.count())
            return _columnNames[section];
    }
    return QVariant();
}

QVariant PartTypeTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(role !=Qt::DisplayRole && role!=Qt::EditRole)
        return QVariant();
    int row = index.row();
    int column = index.column();
    if (row >= _rows.size() || row < 0)
        return QVariant();
    if(column>=COLUMN_COUNT || column<0)
        return QVariant();

    const ModelRow<PartType> & rowData = _rows.at(row);
    switch(column){
    case 0:
        return rowData.item.name.get();
        break;
    case 1:
        return _categoriesMap.value(rowData.item.category);
        break;
    case 2:
        return rowData.item.description.get();
        break;
    }
    return QVariant();
}

void PartTypeTableModel::load()
{
    DQQuery<PartType> query;
    query = query.orderBy("name");
    if(query.exec()){
        while(query.next()){
            PartType part;
            query.recordTo(part);
            ModelRow<PartType> row(part);
            _rows.append(row);
        }
    }
    else{
        //TODO: Show some error
    }
}
