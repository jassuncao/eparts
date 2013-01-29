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

class QSqlTableModelWithAlignment : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit QSqlTableModelWithAlignment(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const;
    void setColumnAlignment(int column, Qt::Alignment alignment);
    void setTable(const QString &tableName);
private:
    QVector<int> _alignments;
};


class PartsMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PartsMainWidget(QWidget *parent = 0);
    ~PartsMainWidget();
signals:
    void editPart(const int partId);
private:    
    QList<QAbstractItemDelegate*> _temporaryDelegates;
   // PartDetailsWidget * _detailsWidget;
    PartModel _partModel;
    QSqlTableModelWithAlignment _tableModel;
    //PartsTableModel2 _tableModel;
    QSortFilterProxyModel * _proxyModel;
    //PartsTableModel * _tableModel;
    QStandardItemModel * _treeModel;
    Ui::PartsMainWidget *ui;
    SpinBoxDelegate * _spinBoxDelegate;
    PartTableModel _partTableModel;
    AttributesRepository * _attributesRepo;
    void buildPartsModel();
//    void initDetailsViewWidget();
    void setupTableModel();
private slots:
    void initCategoriesTree();
    void tableDoubleClicked(const QModelIndex &index);
    void currentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
    void treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void buttonBoxClicked(QAbstractButton*);
    void addPart();
    void removePart();

};




#endif // PARTSMAINWIDGET_H
