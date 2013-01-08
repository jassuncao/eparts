#include "attributestablemodel.h"

namespace Models {

#define COLUMN_COUNT 2

AttributesTableModel::AttributesTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

void AttributesTableModel::setRepository(AttributesRepository *repository)
{
    _repository = repository;
    _attributes = _repository->attributes();
    reset();
}

int AttributesTableModel::rowCount(const QModelIndex &parent) const
{
    if(_repository)
        return _repository->count();
    return 0;
}

int AttributesTableModel::columnCount(const QModelIndex &parent) const
{
    return COLUMN_COUNT;
}

QVariant AttributesTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(!_repository)
        return QVariant();
    int row = index.row();
    int column = index.column();
    if (row >= _repository->count() || row < 0 || column>COLUMN_COUNT)
        return QVariant();

    if (role == Qt::DisplayRole) {
        AbstractPartAttribute * attribute = _attributes.at(row);
        switch(column){
        /*
        case 0:
            return QVariant(attribute->id());
            */
        case 0:
            return QVariant(attribute->name());
        case 1:
            return QVariant(attribute->description());
        default:
            return QVariant();
        }
    }
    return QVariant();
}

}//namespace
