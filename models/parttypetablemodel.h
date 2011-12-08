#ifndef PARTTYPETABLEMODEL_H
#define PARTTYPETABLEMODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include "modelrow.h"
#include "parttype.h"

class PartTypeTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PartTypeTableModel(QObject *parent = 0);    
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    PartType rowData(int row) const;
    void setRowData(int row, const PartType & data);
    QModelIndex add(PartType & partType);
    void load();
signals:

public slots:

private:
    QList<QString> _columnNames;
    QHash<int,QString> _categoriesMap;
    QList< ModelRow<PartType> > _rows;
    QList< ModelRow<PartType> > _removedRows;
};

#endif // PARTTYPETABLEMODEL_H
