#ifndef PARTSTABLEMODEL_H
#define PARTSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include "parttype.h"


class PartRow;
class PartParameter;
class PartType;

class PartsTableModel : public QAbstractTableModel 
{
    Q_OBJECT
public:
    explicit PartsTableModel(const int partTypeId, QObject *parent = 0);
    ~PartsTableModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void load();

signals:

public slots:

private:
    const int _partTypeId;
    PartType _partType;
    QList<QString> _columnNames;
    QList<PartRow*> _rows;
    QList<PartParameter> _parameters;

    void loadRowData(PartRow * row) const;
};

/*

class PartRow
{
public:
    PartRow(int id, const QString items[]);
    ~PartRow();

    inline int id() const { return _id;}
    const QString& operator[](const int i) const { return _items[i];}

private:
    bool isLoaded;
    const int _id;
    const QString * _items;
};

*/

#endif // PARTSTABLEMODEL_H
