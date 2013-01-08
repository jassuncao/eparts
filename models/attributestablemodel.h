#ifndef ATTRIBUTESTABLEMODEL_H
#define ATTRIBUTESTABLEMODEL_H

#include <QAbstractTableModel>
#include "attributesrepository.h"

namespace Models {

class AttributesTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:    
    explicit AttributesTableModel(QObject *parent = 0);
    void setRepository(AttributesRepository * repository);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
signals:
    
public slots:
private:
    AttributesRepository * _repository;
    QList<AbstractPartAttribute*> _attributes;
};

}//namespace

#endif // ATTRIBUTESTABLEMODEL_H
