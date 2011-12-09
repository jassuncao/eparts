#ifndef PARTSMAINWIDGET_H
#define PARTSMAINWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "partmodel.h"
#include "models/partstablemodel.h"
#include "partdetailswidget.h"

class QModelIndex;

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
    PartDetailsWidget * _detailsWidget;
    PartModel _partModel;
    PartsTableModel * _tableModel;
    QStandardItemModel * _treeModel;
    Ui::PartsMainWidget *ui;    
    void buildPartsModel();
//    void initDetailsViewWidget();
private slots:
    void currentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
    void treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

};

#endif // PARTSMAINWIDGET_H
