#ifndef PARTSFILTERPROXYMODEL_H
#define PARTSFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "criteria.h"
#include <QList>

class PartsFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    PartsFilterProxyModel(QObject *parent = 0);
    void addCriterion(const Criterion *criterion);
    void clearCriteria();
protected:
     bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
private:
     QList<const Criterion*> m_criteria;
};

#endif // PARTSFILTERPROXYMODEL_H
