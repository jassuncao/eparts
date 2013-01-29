#ifndef PARTTABLEMODEL_H
#define PARTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVector>
#include <QtSql>
#include "part.h"
#include "attributeformatter.h"

namespace Models {

struct PartRow;
class PartColumn;
typedef struct PartRow * PartRowPtr;

class PartTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PartTableModel(QObject *parent = 0);
    ~PartTableModel();
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
    void setCategory(int category);
    int itemFromIndex(const QModelIndex &index)const;
private:
    void loadColumns();
    void loadRows();
    void loadPartAttributes(PartRow * partRow) const;
    int _catId;
    int _sortColumn;
    Qt::SortOrder _sortOrder;
    QList<PartRowPtr> _rows;
    QVector<PartColumn*> _columns;
    QSqlQuery _selectActiveAttributes;    
};

}//namespace

#endif // PARTTABLEMODEL_H
