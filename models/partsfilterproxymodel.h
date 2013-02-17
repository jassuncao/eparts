#ifndef PARTSFILTERPROXYMODEL_H
#define PARTSFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class PartsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    PartsFilterProxyModel(QObject *parent = 0);
protected:
     bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};

#endif // PARTSFILTERPROXYMODEL_H
