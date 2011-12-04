#include "partparameterstablemodel.h"

/*
  Fields to columns mapping:
  name -> 0
  type -> 1
  description -> 2
  fixedValues -> 3

  Hidden fields: partType, orderIndex;

  */

const int COLUMN_COUNT = 4;

PartParametersTableModel::PartParametersTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    _columnNames<<tr("Name")<<tr("Type")<<tr("Description")<<tr("Fixed values");
}

int PartParametersTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _rows.size();
}

int PartParametersTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

QVariant PartParametersTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role==Qt::DisplayRole && orientation==Qt::Horizontal){
        if(section>=0 && section<_columnNames.count())
            return _columnNames[section];
    }
    return QVariant();
}

QVariant PartParametersTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role != Qt::DisplayRole && role!=Qt::EditRole)
        return QVariant();
    int row = index.row();
    int column = index.column();
    if (row >= _rows.count() || row < 0)
        return QVariant();
    if(column<0 || column>=COLUMN_COUNT)
        return QVariant();

    ModelRow<PartParameter> rowData = _rows.at(row);
    switch(column){
    case 0:
        return rowData.item.name;
        break;
    case 1:
        return rowData.item.type;
        break;
    case 2:
        return rowData.item.description;
    case 3:
        return rowData.item.fixedValues;
        break;
    }
    return QVariant();
}

bool PartParametersTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    int row = index.row();
    if (row >= _rows.size() || row < 0)
        return false;
    bool res = false;
    if(role==Qt::EditRole)
    {

        ModelRow<PartParameter> & rowData = _rows[row];
        switch(index.column()){
        case 0:
            res = true;
            rowData.item.name = value;
            break;
        case 1:
            res = true;
            rowData.item.type = value;
            break;
        case 2:
            res = true;
            rowData.item.description = value;
            break;
        case 3:
            res = true;
            rowData.item.fixedValues = value;
        default:
            return false;
        }
        if(res){
            rowData.state = ModelRow<PartParameter>::Modified;
            /*
            ModelRow<PartParameter> modelRow(rowData.item);            
            _rows[row].state = ModelRow<PartParameter>::Modified;
            _rows.replace(row, modelRow);
            */
            emit dataChanged(index,index);
        }
    }
    return res;
}

void PartParametersTableModel::load(QVariant partId)
{
    _partId = partId;
    beginResetModel();
    DQQuery<PartParameter> query;
    query = query.filter(DQWhere("partType")==partId).orderBy("orderIndex");
    if(query.exec()){
        while(query.next()){
            PartParameter param;
            query.recordTo(param);
            ModelRow<PartParameter> row(param);
            _rows.append(row);
        }
    }
    else{
        //TODO: Show some error
    }
    endResetModel();
}

void PartParametersTableModel::saveChanges()
{
    qDebug()<<"Saving changes";
    QListIterator< ModelRow<PartParameter> > it2(_removedRows);
    while(it2.hasNext()){
        ModelRow<PartParameter> row = it2.next();
        row.item.remove();
    }

    QListIterator< ModelRow<PartParameter> > it(_rows);
    int orderIndex = 1;
    while(it.hasNext()){
        ModelRow<PartParameter> row = it.next();
        row.item.orderIndex = orderIndex++;
        row.item.save();
        qDebug()<<"Saving"<<row.item;
    }

}


void PartParametersTableModel::moveUp(int row)
{
    if(row<=0) return;
    beginMoveRows(QModelIndex(),row,row,QModelIndex(),row-1);
    ModelRow<PartParameter> & currentRow = _rows[row];
    ModelRow<PartParameter> & aboveRow = _rows[row-1];
    int aboveIndex = aboveRow.item.orderIndex;
    aboveRow.item.orderIndex = currentRow.item.orderIndex;
    aboveRow.state = ModelRow<PartParameter>::Modified;
    currentRow.item.orderIndex = aboveIndex;
    currentRow.state = ModelRow<PartParameter>::Modified;
    _rows.move(row,row-1);
    endMoveRows();
}

void PartParametersTableModel::moveDown(int row)
{
    int maxRow = _rows.count()-1;
    if(row>=maxRow) return;
    QModelIndex index;
    beginMoveRows(index,row,row,index,row+2);
    ModelRow<PartParameter> & currentRow = _rows[row];
    ModelRow<PartParameter> & belowRow = _rows[row+1];
    int belowIndex = belowRow.item.orderIndex;
    belowRow.item.orderIndex = currentRow.item.orderIndex;
    belowRow.state = ModelRow<PartParameter>::Modified;
    currentRow.item.orderIndex = belowIndex;
    currentRow.state = ModelRow<PartParameter>::Modified;
    _rows.move(row,row+1);
    endMoveRows();
}


QModelIndex PartParametersTableModel::appendRow(const PartParameter & parameter)
{
    int rowPos = _rows.count();
    QModelIndex index = createIndex(rowPos,0);
    beginInsertRows(index ,rowPos, rowPos);
    PartParameter copy(parameter);
    copy.partType = _partId;
    ModelRow<PartParameter> row(copy);
    row.state = ModelRow<PartParameter>::New;
    _rows.append(row);
    endInsertRows();
    return index;
}

bool PartParametersTableModel::removeRows(int row, int count, const QModelIndex & parent)
{
    int lastRow = row+count-1;
    if(lastRow>_rows.count())
        lastRow = _rows.count()-1;
    beginRemoveRows(parent, row, lastRow);
    for(int idx = lastRow;idx>=row;--idx){
        _removedRows.append(_rows.at(idx));
        _rows.removeAt(idx);
    }
    endRemoveRows();
    return true;
}
