#ifndef PARTPARAMETERSLISTMODEL_H
#define PARTPARAMETERSLISTMODEL_H

#include <QAbstractListModel>
#include "database/partparameter.h"
#include "modelrow.h"

class PartParametersListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PartParametersListModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void load(QVariant partId);
    bool removeRows(int row, int count, const QModelIndex & parent);
    QModelIndex add(PartParameter & parameter);
    PartParameter getParameter(int row) const;
    void updateParameter(int row, const PartParameter & parameter);
    void moveUp(int row);
    void moveDown(int row);
    void saveChanges();

signals:

public slots:
private:
    QList< ModelRow<PartParameter> > _rows;
    QList< ModelRow<PartParameter> > _removedRows;
    void moveRow(int from, int to);

};

#endif // PARTPARAMETERSLISTMODEL_H
