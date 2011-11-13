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
        CategoryRow row = _rows.at(index.row());
        return row.category.name;
    }
    else if(role == Qt::ToolTipRole){
        CategoryRow row = _rows.at(index.row());
        return row.category.description;
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
            CategoryRow row(cat);
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
    CategoryRow row(cat);
    row.state = CategoryRow::New;
    _rows.append(row);
    endInsertRows();
}

void CategoriesListModel::saveChanges()
{
    QListIterator<CategoryRow> it(_rows);
    while(it.hasNext()){
        CategoryRow row = it.next();
        if(row.state==CategoryRow::New || row.state==CategoryRow::Modified){
            row.category.save();
        }
    }
}
