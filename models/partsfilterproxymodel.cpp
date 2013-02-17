#include "partsfilterproxymodel.h"

PartsFilterProxyModel::PartsFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool PartsFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
}
