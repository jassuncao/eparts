#include <QVariant>
#include "parttablemodel.h"


namespace Models {


struct PartRow
{

    PartRow(int partId, int columnsCount) :
        _partId(partId),
        _values(columnsCount),
        _loaded(false)
    {}

    inline QVariant value(int column) { return _values.at(column);}
    inline int partId(){ return _partId;}
    inline bool isLoaded() const { return _loaded;}
    inline void setLoaded(bool b) {_loaded=b;}
    inline void setValue(int column, QVariant value)
    {
        _values[column]=value;
    }

private:
    const int _partId;
    QVector<QVariant> _values;
    bool _loaded;
};

class PartColumn
{
public:
    PartColumn(QString columnName, QString label, int type, int attrId=-1) :
        _columnName(columnName),
        _label(label),
        _attributeId(attrId),
        _type(type),
        _formatter(AttributeFormatterFactory::formatterFor(type)),
        _alignment(Qt::AlignRight|Qt::AlignVCenter)
    {
        if(type==Models::ATTRIBUTE_TEXT) {
            _findAttributeValueQuery.prepare("SELECT value FROM text_value WHERE part=:partId AND attribute=:attrId");
        }
        else {
            _findAttributeValueQuery.prepare("SELECT value FROM float_value WHERE part=:partId AND attribute=:attrId");
        }
    }

    ~PartColumn()
    {
        delete _formatter;
    }

    inline const QString columnName() const {return _columnName;}
    inline const QString label() const {return _label;}
    inline const int attributeId() const {return _attributeId;}
    inline const int type() const {return _type;}
    inline const AttributeFormatter * formatter() const {return _formatter;}
    inline Qt::Alignment alignment() const { return _alignment;}
    inline void setAlignment(Qt::Alignment alignment) {_alignment=alignment;}


    QVariant loadAttrValue(int partId, bool * ok)
    {
        QVariant res;
        _findAttributeValueQuery.bindValue(0,partId);
        _findAttributeValueQuery.bindValue(1,_attributeId);
        if(_findAttributeValueQuery.exec()){
            *ok=true;
            if(_findAttributeValueQuery.next()){
                 res=_findAttributeValueQuery.value(0);
            }
        }
        else{
            *ok=false;
        }
        _findAttributeValueQuery.finish();
        return res;
    }

private:
    const QString _columnName;
    const QString _label;
    const int _attributeId;
    const int _type;
    const AttributeFormatter * _formatter;
    QSqlQuery _findAttributeValueQuery;
    Qt::Alignment _alignment;
};


PartTableModel::PartTableModel(QObject *parent) :
    QAbstractTableModel(parent),
    _catId(-1),
    _sortColumn(3),//part number
    _sortOrder(Qt::AscendingOrder)
{
    _selectActiveAttributes.prepare("SELECT a.id, a.type, a.name FROM attribute a "
                                    "INNER JOIN category_active_attributes c "
                                    "WHERE c.category=:catId AND a.id=c.attribute ORDER BY c.idx; ");

}

PartTableModel::~PartTableModel()
{    
    qDeleteAll(_rows);
    qDeleteAll(_columns);
    _rows.clear();
    _columns.clear();
}

int PartTableModel::rowCount(const QModelIndex &parent) const
{
    return _rows.count();
}

int PartTableModel::columnCount(const QModelIndex &parent) const
{
    return _columns.count();
}

QVariant PartTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    QVariant result;
    if (role & ~(Qt::DisplayRole | Qt::EditRole))
        return result;
    int row = index.row();
    int column = index.column();
    if(row<0 || row>=_rows.count())
        return result;
    switch(role)
    {
        case Qt::DisplayRole:
        case Qt::EditRole:
        {
            PartRowPtr partRow = _rows.at(row);
            if(!partRow->isLoaded())
                loadPartAttributes(partRow);
            QVariant value = partRow->value(column);
            result = _columns.at(column)->formatter()->format(value, role);
        }
        break;
    case Qt::TextAlignmentRole:
        result = QVariant(_columns.at(column)->alignment());
        break;
    default:
        break;
    }
    return result;
}

QVariant PartTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation!=Qt::Horizontal)
        return QVariant();    
    if(section>=0 && section<_columns.count()){
        return _columns.at(section)->label();
    }
    return QVariant();
}

Qt::ItemFlags PartTableModel::flags ( const QModelIndex & index ) const
{
    if(index.isValid()){
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled /* | Qt::ItemIsEditable*/;
    }
    return Qt::NoItemFlags;
}

void PartTableModel::sort(int column, Qt::SortOrder order)
{
    if(column!=_sortColumn || _sortOrder!=order) {
        _sortColumn = column;
        _sortOrder = order;
        beginResetModel();
        loadRows();
        endResetModel();
    }
}

void PartTableModel::setCategory(int category)
{
    if(category==_catId) return;
    _catId = category;
    beginResetModel();
    loadColumns();
    loadRows();
    endResetModel();    
}

int PartTableModel::itemFromIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return -1;
    int row = index.row();
    if(row<0 || row>=_rows.count())
        return -1;
    return _rows.at(row)->partId();
}

void PartTableModel::setDirty(int partId)
{
    beginResetModel();
    loadRows();
    endResetModel();
}

void PartTableModel::loadColumns()
{
    qDeleteAll(_columns);
    _columns.clear();

    _columns.append(new PartColumn(QString("quantity"),tr("Quantity"),Models::ATTRIBUTE_GENERIC_INTEGER));
    _columns.append(new PartColumn(QString("minimumQuantity"),tr("Min. Quantity"),ATTRIBUTE_GENERIC_INTEGER));
    _columns.append(new PartColumn(QString("partNumber"),tr("Part Number"),ATTRIBUTE_TEXT));
    _columns.append(new PartColumn(QString("description"),tr("Description"),ATTRIBUTE_TEXT));

    _selectActiveAttributes.bindValue(":catId",_catId);
    _selectActiveAttributes.exec();
    while(_selectActiveAttributes.next()){
        int attrId = _selectActiveAttributes.value(0).toInt();
        int attrType = _selectActiveAttributes.value(1).toInt();
        QVariant attrName = _selectActiveAttributes.value(2);
        _columns.append(new PartColumn(QString(), attrName.toString(), attrType,attrId));
        qDebug()<<"Added attribute column "<<attrName;
    }
    _selectActiveAttributes.finish();
}

void PartTableModel::loadRows()
{
    QSqlQuery query;
    int columnCount = _columns.count();

    qDeleteAll(_rows);
    _rows.clear();

    if(_sortColumn>=0 && _sortColumn<columnCount)
    {
        const char* sortOrderStm = _sortOrder==Qt::AscendingOrder ? "ASC" : "DESC";
        PartColumn * column = _columns.at(_sortColumn);
        QString columnName = column->columnName();
        if(columnName.isNull())
        { //Sort by attribute value
            const char* valueTable = column->type()==Models::ATTRIBUTE_TEXT ?
                        "text_value" :
                        "float_value";

            QString statement("SELECT p.id, p.quantity, p.minimumQuantity, p.partNumber, p.description FROM part p "
                              "INNER JOIN %0 v WHERE p.category=:catId AND v.part=p.id AND v.attribute=:attrId ORDER BY v.value %1;");
            statement=statement.arg(valueTable,sortOrderStm);
            qDebug()<<"Query:"<<statement;
            query.prepare(statement);
            query.bindValue(":attrId",column->attributeId());
        }
        else
        { //Sort by entity column
            QString statement("SELECT id, quantity, minimumQuantity, partNumber, description FROM part WHERE category=:catId ORDER BY %1 %2;");
            statement=statement.arg(columnName,sortOrderStm);
            qDebug()<<"Query:"<<statement;
            query.prepare(statement);
        }
    }
    else
    {
        QString statement("SELECT id, quantity, minimumQuantity, partNumber, description FROM part WHERE category=:catId");
        qDebug()<<"Query:"<<statement;
        query.prepare(statement);
    }
    query.bindValue(":catId",_catId);
    if(query.exec())
    {
        while(query.next())
        {
            int partId = query.value(0).toInt();
            PartRow *row = new PartRow(partId,columnCount);

            QVariant partQuant = query.value(1);
            QVariant partMinQuant = query.value(2);
            QVariant partNumber = query.value(3);
            QVariant description = query.value(4);
            row->setValue(0,partQuant);
            row->setValue(1,partMinQuant);
            row->setValue(2,partNumber);
            row->setValue(3,description);
            _rows.append(row);
        }
        qDebug()<<"Loaded "<<_rows.count()<<" rows";
    }
    else{
        qWarning()<<"SQL query error"<<query.lastError().text();
        //TODO: SHow some error msg
    }
    query.finish();

}

void PartTableModel::loadPartAttributes(PartRow *partRow) const
{
    int partId = partRow->partId();
    bool ok;
    for(int i=0; i<_columns.count();++i)
    {
        PartColumn* columnPtr=_columns.at(i);
        if(columnPtr->attributeId()>=0){
            QVariant attrVal = columnPtr->loadAttrValue(partId, &ok);
            if(ok)
                partRow->setValue(i,attrVal);
            else{
//TODO: Show some error msg
            }
        }
    }
    partRow->setLoaded(true);
}

}//namespace
