#ifndef PARTSTABLEMODEL_H
#define PARTSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include "database/parttype.h"
#include "partmodel.h"
#include "database/part.h"
#include "database/parametervalue.h"
#include "parttypemodel.h"


class PartRow {
public:
    PartRow(Part part) : part(part), loaded(false) {}
    Part part;
    bool loaded;
    QHash<int,ParameterValue> paramValues;
};

class PartsTableModel2 : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PartsTableModel2(QObject *parent = 0);
    ~PartsTableModel2();
    const PartTypeModel * partTypeModel() const  { return &_partTypeModel;}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //PartRow * rowData(const QModelIndex &index) const;
    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    void load(int partType);
    //void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

private:
    PartTypeModel _partTypeModel;
    QList<PartsTableRow*> _rows;
};

#endif // PARTSTABLEMODEL_H
