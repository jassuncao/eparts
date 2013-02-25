#include "partsfilterproxymodel.h"

PartsFilterProxyModel::PartsFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void PartsFilterProxyModel::addCriterion(const Criterion *criterion)
{
    m_criteria.push_back(criterion);
}

void PartsFilterProxyModel::clearCriteria()
{
    qDeleteAll(m_criteria);
}

bool PartsFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    bool accept=true;
    for(int i=0; accept && i<m_criteria.count(); ++i){
        const Criterion *criterion=m_criteria.at(i);
        QModelIndex index = sourceModel()->index(sourceRow, criterion->column(), sourceParent);
        QVariant value = sourceModel()->data(index);
        accept = criterion->accepts(value);
    }
    return accept;
}
