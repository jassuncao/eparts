#ifndef PARTSTABLEMODEL_H
#define PARTSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include "parttype.h"
#include "partmodel.h"
#include "part.h"
#include "parametervalue.h"


class PartRow {
public:
    PartRow(Part part) : part(part), loaded(false) {}
    Part part;
    bool loaded;
    QHash<int,ParameterValue> paramValues;
};

class PartsTableModel : public QAbstractTableModel 
{
    Q_OBJECT
public:
    explicit PartsTableModel(const PartModel * partModel, QObject *parent = 0);
    ~PartsTableModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    PartRow * rowData(const QModelIndex &index) const;
    void load();

signals:

public slots:

private:
    const PartModel * _partModel;
    QList<QString> _columnNames;
    QList<PartRow*> _rows;
    void loadRowData(PartRow * row) const;
};

#endif // PARTSTABLEMODEL_H
