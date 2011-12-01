#include "categorieslistmodel.h"

CategoriesListModel::CategoriesListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}


int CategoriesListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _rows.size();
}

QVariant CategoriesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _rows.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        ModelRow<Category> row = _rows.at(index.row());
        return row.item.name;
    }
    else if(role == Qt::ToolTipRole){
        ModelRow<Category> row = _rows.at(index.row());
        return row.item.description;
    }
    return QVariant();
}

void CategoriesListModel::load(){
    DQQuery<Category> query;
    query = query.orderBy("name");
    if(query.exec()){
        while(query.next()){
            Category cat;
            query.recordTo(cat);
            ModelRow<Category> row(cat);
            _rows.append(row);
        }
    }
    else{
        //TODO: Show some error
    }
}

bool CategoriesListModel::removeRows(int row, int count, const QModelIndex & parent)
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

void CategoriesListModel::add(Category * category)
{
    int rowPos = _rows.count();
    beginInsertRows( QModelIndex(),rowPos, rowPos);
    Category cat;
    cat = *category;
    ModelRow<Category> row(cat);

    row.state = ModelRow<Category>::New;
    _rows.append(row);
    endInsertRows();
}

Category CategoriesListModel::getCategory(int row) const
{
    return _rows.at(row).item;
}

void CategoriesListModel::updateCategory(int row, const Category & category)
{
    QModelIndex topLeft = QAbstractListModel::createIndex(row,0);
    ModelRow<Category> catRow(category);
    catRow.state = ModelRow<Category>::Modified;
    _rows.replace(row, catRow);
    emit dataChanged(topLeft,topLeft);
}

void CategoriesListModel::saveChanges()
{
    QListIterator< ModelRow<Category> > it(_rows);
    while(it.hasNext()){
        ModelRow<Category> row = it.next();
        if(row.state==ModelRow<Category>::New || row.state==ModelRow<Category>::Modified){
            row.item.save();
        }
    }
}
