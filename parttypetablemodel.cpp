#include "parttypetablemodel.h"
#include <QtDebug>

PartTypeTableModel::PartTypeTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}


int PartTypeTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _rows.size();
}

int PartTypeTableModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant PartTypeTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal){
        switch (section){
        case 0:
            return tr("Name");
            break;
        case 1:
            return tr("Category");
            break;
        case 2:
            return tr("Description");
            break;
        }
    }
    return QVariant();
}

QVariant PartTypeTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int column = index.column();
    if (row >= _rows.size() || row < 0)
        return QVariant();
    if(column>=3 || column<0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        ModelRow<PartType> rowData = _rows.at(row);
        switch(column){
        case 0:
            return rowData.item.name;
            break;
        case 1:
            return rowData.item.category->name;
            break;
        case 2:
            return rowData.item.description;
            break;
        }
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
