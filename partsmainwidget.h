#ifndef PARTSMAINWIDGET_H
#define PARTSMAINWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "partmodel.h"
#include "models/partstablemodel.h"
#include "partdetailswidget.h"

class QModelIndex;
class SpinBoxDelegate;

namespace Ui {
    class PartsMainWidget;
}

class PartsMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PartsMainWidget(QWidget *parent = 0);
    ~PartsMainWidget();

private:
   // PartDetailsWidget * _detailsWidget;
    PartModel _partModel;
    PartsTableModel2 _tableModel;
    QSortFilterProxyModel * _proxyModel;
    //PartsTableModel * _tableModel;
    QStandardItemModel * _treeModel;
    Ui::PartsMainWidget *ui;
    SpinBoxDelegate * _spinBoxDelegate;
    void buildPartsModel();
//    void initDetailsViewWidget();
private slots:
    void currentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
    void treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void buttonBoxClicked(QAbstractButton*);
    void addPart();
    void removePart();

};

#endif // PARTSMAINWIDGET_H
