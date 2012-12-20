#ifndef PARTPARAMETERSTABLEMODEL_H
#define PARTPARAMETERSTABLEMODEL_H

#include <QAbstractTableModel>
#include "modelrow.h"
#include "database/partparameter.h"

class PartParametersTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PartParametersTableModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void load(QVariant partId);
    void saveChanges();
    void moveUp(int row);
    void moveDown(int row);
    QModelIndex appendRow(const PartParameter & parameter);
    bool removeRows(int row, int count, const QModelIndex & parent);
signals:

public slots:

private:
    QVariant _partId;
    QList<QString> _columnNames;
    QList< ModelRow<PartParameter> > _rows;
    QList< ModelRow<PartParameter> > _removedRows;

};

#endif // PARTPARAMETERSTABLEMODEL_H
