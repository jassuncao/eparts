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
#include <QtGui/QPlainTextEdit>
#include <QtGui/QDataWidgetMapper>
#include <QHash>
#include "widgets/qunitlineedit.h"
#include "models/partstablemodel.h"
#include "models/parttypemodel.h"
class PartModel;
class QAbstractItemModel;

class PartDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PartDetailsWidget(QWidget *parent = 0);
    void retranslateUi(QWidget *widget);
    void setModel(const PartTypeModel * partType, QAbstractItemModel * model);
private:

    const PartTypeModel * _partTypeModel;
    QLabel * quantityLabel;
    QLabel * minimumQuantityLabel;     
    QSpinBox * quantitySpinBox;
    QSpinBox * minimumQuantitySpinBox;    
    QFormLayout *_formLayout;
    QHash<int,QWidget*> _params;
    QDataWidgetMapper * _dataMapper;
    void createWidgets();
signals:

public slots:
    void setCurrentModelIndex(const QModelIndex & index);
    void submit();
    void revert();
};

#endif // PARTDETAILSWIDGET_H
