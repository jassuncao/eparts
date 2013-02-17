#include "readonlyattributeslistmodel.h"
#include <QDebug>

namespace Models {

ReadOnlyAttributesListModel::ReadOnlyAttributesListModel(QObject * parent) :
    QAbstractListModel(parent)
{
}

int ReadOnlyAttributesListModel::rowCount(const QModelIndex &parent) const
{
    return m_attributes.count();
}

QVariant ReadOnlyAttributesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    int row = index.row();
    if (row >= m_attributes.count())
        return QVariant();

    if(role == Qt::DisplayRole){
        const AbstractPartAttribute * attribute = m_attributes.at(row);
        return QVariant(attribute->name());
    }
    else if(role == Qt::UserRole){
        const  AbstractPartAttribute * attribute = m_attributes.at(row);
        return QVariant(attribute->id());
    }
    return QVariant();
}

void ReadOnlyAttributesListModel::setAttributes(const QList<const AbstractPartAttribute *> attributes)
{
    beginResetModel();
    m_attributes = attributes;
    endResetModel();
}

const AbstractPartAttribute *ReadOnlyAttributesListModel::getAttribute(int row) const
{
    if (row >= m_attributes.count())
        return 0;
    return m_attributes.at(row);
}

} //namespace
