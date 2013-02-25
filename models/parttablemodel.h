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
typedef struct PartRow * PartRowPtr;

class PartColumn
{
public:
    PartColumn(QString columnName, QString label, int type, int index, int attrId=-1);
    ~PartColumn();

    inline const QString columnName() const {return _columnName;}
    inline const QString label() const {return _label;}
    inline int attributeId() const {return _attributeId;}
    inline int type() const {return _type;}
    inline int index() const {return _index;}
    inline const AttributeFormatter * formatter() const {return _formatter;}
    inline Qt::Alignment alignment() const { return _alignment;}
    inline void setAlignment(Qt::Alignment alignment) {_alignment=alignment;}
    QVariant loadAttrValue(int partId, bool * ok);

private:
    const QString _columnName;
    const QString _label;
    const int _attributeId;
    const int _type;
    const int _index;
    const AttributeFormatter * _formatter;
    QSqlQuery _findAttributeValueQuery;
    Qt::Alignment _alignment;
};

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
    inline int category() const {return _catId;}
    int itemFromIndex(const QModelIndex &index)const;
    void setDirty(int partId);
    const QVector<PartColumn*> columns();
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
