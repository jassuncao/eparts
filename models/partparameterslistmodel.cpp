#include "partparameterslistmodel.h"

PartParametersListModel::PartParametersListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}


int PartParametersListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _rows.size();
}

QVariant PartParametersListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _rows.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        ModelRow<PartParameter> row = _rows.at(index.row());
        return row.item.name;
    }
    else if(role == Qt::ToolTipRole){
        ModelRow<PartParameter> row = _rows.at(index.row());
        return row.item.description;
    }
    return QVariant();
}

bool PartParametersListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    int row = index.row();
    if (row >= _rows.size() || row < 0)
        return false;
    bool res = false;
    if(role==Qt::EditRole)
    {

        ModelRow<PartParameter> rowData = _rows.at(row);
        switch(index.column()){
            case 0:
            res = true;
            rowData.item.name = value;
            break;
        case 1:
            res = true;
            rowData.item.description = value;
            break;
        default:
            return false;
        }
        if(res){
            ModelRow<PartParameter> modelRow(rowData.item);
            modelRow.state = ModelRow<PartParameter>::Modified;
            _rows.replace(row, modelRow);
            emit dataChanged(index,index);
        }
    }
    return res;
}

void PartParametersListModel::load(QVariant partId)
{
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
}

bool PartParametersListModel::removeRows(int row, int count, const QModelIndex & parent)
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

QModelIndex PartParametersListModel::add(PartParameter & parameter)
{
    int rowPos = _rows.count();
    QModelIndex index = createIndex(rowPos,0);
    beginInsertRows(index ,rowPos, rowPos);
    PartParameter copy(parameter);
    ModelRow<PartParameter> row(copy);

    row.state = ModelRow<PartParameter>::New;
    _rows.append(row);
    endInsertRows();
    return index;
}

PartParameter PartParametersListModel::getParameter(int row) const
{
    return _rows.at(row).item;
}

void PartParametersListModel::updateParameter(int row, const PartParameter & parameter)
{
    QModelIndex topLeft = QAbstractListModel::createIndex(row,0);
    ModelRow<PartParameter> modelRow(parameter);
    modelRow.state = ModelRow<PartParameter>::Modified;
    _rows.replace(row, modelRow);
    emit dataChanged(topLeft,topLeft);
}


void PartParametersListModel::saveChanges()
{
    QListIterator< ModelRow<PartParameter> > it(_rows);
    while(it.hasNext()){
        ModelRow<PartParameter> row = it.next();
        if(row.state==ModelRow<PartParameter>::New || row.state==ModelRow<PartParameter>::Modified){
            row.item.save();
        }
    }
    QListIterator< ModelRow<PartParameter> > it2(_removedRows);
    while(it2.hasNext()){
        ModelRow<PartParameter> row = it2.next();
        row.item.remove();
    }
}

void PartParametersListModel::moveUp(int row)
{
    if(row<=0) return;
    beginMoveRows(QModelIndex(),row,row,QModelIndex(),row-1);
    ModelRow<PartParameter> currentRow = _rows.at(row);
    ModelRow<PartParameter> aboveRow = _rows.at(row-1);
    int aboveIndex = aboveRow.item.orderIndex;
    aboveRow.item.orderIndex = currentRow.item.orderIndex;
    aboveRow.state = ModelRow<PartParameter>::Modified;
    currentRow.item.orderIndex = aboveIndex;
    currentRow.state = ModelRow<PartParameter>::Modified;
    _rows.move(row,row-1);
    endMoveRows();
}

void PartParametersListModel::moveDown(int row)
{
    int maxRow = _rows.count()-1;
    if(row>=maxRow) return;
    QModelIndex index;
    beginMoveRows(index,row,row,index,row+2);
    ModelRow<PartParameter> currentRow = _rows.at(row);
    ModelRow<PartParameter> belowRow = _rows.at(row+1);
    int belowIndex = belowRow.item.orderIndex;
    belowRow.item.orderIndex = currentRow.item.orderIndex;
    belowRow.state = ModelRow<PartParameter>::Modified;
    currentRow.item.orderIndex = belowIndex;
    currentRow.state = ModelRow<PartParameter>::Modified;
    _rows.move(row,row+1);
    endMoveRows();
}

void PartParametersListModel::moveRow(int from, int to)
{


}
