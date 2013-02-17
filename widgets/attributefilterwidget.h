#ifndef ATTRIBUTEFILTERWIDGET_H
#define ATTRIBUTEFILTERWIDGET_H

#include <models/partattribute.h>
#include <QWidget>
#include <QList>
#include <QMap>

class QGridLayout;
class QComboBox;
class QPushButton;
class AttributesFilterProxyModel;
class AttributeFilterRow;
namespace Models{
    class ReadOnlyAttributesListModel;
}

using namespace Models;

class AttributeFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AttributeFilterWidget(QWidget *parent = 0);
    ~AttributeFilterWidget();
    void setAttributes(QList<const AbstractPartAttribute *> attributes);
signals:
    void filterChanged();
public slots:

private slots:
    void onApplyFilter();
    void onClearFilter();
    void addFilterComboSelected(int row);
    void filterRemoved(const AbstractPartAttribute* attribute);
private:
    QGridLayout *m_layout;
    QComboBox *m_addFilterCombo;
    QPushButton *m_clearFilterButton;
    QList<int> m_filteredAttributes;
    Models::ReadOnlyAttributesListModel * m_filterAttributesModel;
    AttributesFilterProxyModel * m_attributesFilterProxyModel;
    QMap<int,AttributeFilterRow*> m_attributeFilterRows;

    static void deleteRow(QGridLayout * layout, int row);
};

#endif // ATTRIBUTEFILTERWIDGET_H
