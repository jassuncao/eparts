#ifndef CATEGORIESLISTMODEL_H
#define CATEGORIESLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "category.h"
#include <QSortFilterProxyModel>

class CategoryRow;

class CategoriesListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CategoriesListModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void load();
    bool removeRows(int row, int count, const QModelIndex & parent);
    void add(Category * category);
    void saveChanges();
signals:

public slots:

private:
    QList<CategoryRow> _rows;
    QList<CategoryRow> _removedRows;

};



class CategoryRow {
public:
    enum RowState {New,Modified,Default};
    CategoryRow(Category category) : category(category), state(Default) {}
    Category category;
    RowState state;
};

#endif // CATEGORIESLISTMODEL_H
