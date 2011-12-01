#ifndef CATEGORIESLISTMODEL_H
#define CATEGORIESLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "category.h"
#include <QSortFilterProxyModel>
#include "modelrow.h"

//class ModelRow<Category>;

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
    Category getCategory(int row) const;
    void updateCategory(int row, const Category & category);
    void saveChanges();
signals:

public slots:

private:
    QList< ModelRow<Category> > _rows;
    QList< ModelRow<Category> > _removedRows;

};


#endif // CATEGORIESLISTMODEL_H
