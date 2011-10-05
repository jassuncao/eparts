#ifndef PARTSTABLEMODEL_H
#define PARTSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include "parttype.h"
#include "partmodel.h"


class PartRow;
class PartParameter;
class PartType;

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
