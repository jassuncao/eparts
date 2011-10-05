#ifndef PARTSMAINWIDGET_H
#define PARTSMAINWIDGET_H

#include <QWidget>
#include "partmodel.h"

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
};

#endif // PARTSMAINWIDGET_H
