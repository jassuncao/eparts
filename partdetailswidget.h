#ifndef PARTDETAILSWIDGET_H
#define PARTDETAILSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QTableView>
#include <QtGui/QTextEdit>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QHash>
#include "widgets/qunitlineedit.h"
#include "models/partstablemodel.h"

class PartModel;

class PartDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PartDetailsWidget(PartModel *partModel, QWidget *parent = 0);
    void retranslateUi(QWidget *widget);
    void setData(PartRow * data);
private:
    PartModel * _partModel;
    QLineEdit * valueField;
    QLabel * quantityLabel;
    QLabel * minimumQuantityLabel;
    QLabel * notesLabel;
    QLabel * valueLabel;
    QFormLayout *formLayout;
    QSpinBox * quantitySpinBox;
    QSpinBox * minimumQuantitySpinBox;
    QTextEdit * notesTextEdit;
    QHash<int,QLineEdit*> _params;

signals:

public slots:

};

#endif // PARTDETAILSWIDGET_H
