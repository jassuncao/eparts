#ifndef PARTSMAINWIDGET_H
#define PARTSMAINWIDGET_H

#include <QWidget>
#include "partmodel.h"

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
    PartModel _partModel;
    Ui::PartsMainWidget *ui;
    void buildPartsModel();
    void initDetailsViewWidget();
private slots:
    void currentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
};

#endif // PARTSMAINWIDGET_H
