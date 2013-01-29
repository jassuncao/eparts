#ifndef PARTSMAINWIDGET_H
#define PARTSMAINWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "partmodel.h"
#include "models/partstablemodel.h"
#include "partdetailswidget.h"
#include <QtSql>
#include <QVector>
#include <QSqlTableModel>
#include "models/parttablemodel.h"
#include "models/attributesrepository.h"

class QModelIndex;
class SpinBoxDelegate;
class UnitColumnDelegate;
class QStyledItemDelegate;

namespace Ui {
    class PartsMainWidget;
}

using namespace Models;

class PartsMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PartsMainWidget(QWidget *parent = 0);
    ~PartsMainWidget();
private:    
    PartModel _partModel;    
    QSortFilterProxyModel * _proxyModel;
    QStandardItemModel * _treeModel;
    Ui::PartsMainWidget *ui;
    PartTableModel _partTableModel;
    AttributesRepository * _attributesRepo;
private slots:
    void initCategoriesTree();
    void tableDoubleClicked(const QModelIndex &index);
    void currentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
    void treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void addPart();
    void removePart();
};


#endif // PARTSMAINWIDGET_H
