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
#include <QList>
#include <QSqlTableModel>
#include "models/parttablemodel.h"
#include "models/attributesrepository.h"
#include "attributefilterrow.h"

class QModelIndex;
class SpinBoxDelegate;
class UnitColumnDelegate;
class QStyledItemDelegate;
class QStandardItemModel;


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
    QStandardItemModel * _treeModel;
    Ui::PartsMainWidget *ui;
    PartTableModel _partTableModel;
    AttributesRepository * _attributesRepo;
    QMap<int,AttributeFilterRow*> m_attributeFilterRows;

    void addFilterForAttribute(int attributeId);    
    void initAddFilterCombo(const QList<const AbstractPartAttribute*> & attributes);
private slots:
    void initCategoriesTree();
    void tableDoubleClicked(const QModelIndex &index);
    void currentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
    void treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void addPart();
    void removePart();    
    void toggleFilterWidget(bool checked);
    void clearFilter();
    void addFilterComboSelected(int index);
    void filterRemoved(const AbstractPartAttribute *attr);
};

template <class T> class VPtr
{
public:
    static T* asPtr(QVariant v)
    {
    return  (T *) v.value<void *>();
    }

    static QVariant asQVariant(T* ptr)
    {
    return qVariantFromValue((void *) ptr);
    }
};


#endif // PARTSMAINWIDGET_H
