#include "attributefilterwidget.h"
#include <QtGui>
#include <models/readonlyattributeslistmodel.h>
#include "attributefilterrow.h"


class AttributesFilterProxyModel : public QSortFilterProxyModel
{
public:
    AttributesFilterProxyModel(QObject *parent = 0) :
        QSortFilterProxyModel(parent=0),
        m_filteredAttributes(0)
    {}

    void setFilterAttributes(const QList<int> * filteredAttributes)
    {
        m_filteredAttributes = filteredAttributes;
        invalidateFilter();
    }

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
    {
        if(m_filteredAttributes==0) return true;
        QModelIndex source_index = sourceModel()->index(source_row, 0, source_parent);
        if (!source_index.isValid()) // the column may not exist
            return true;
        int attrId = sourceModel()->data(source_index, Qt::UserRole).toInt();
        bool res = !m_filteredAttributes->contains(attrId);
        return res;
    }
private:
    const QList<int> * m_filteredAttributes;
};

AttributeFilterWidget::AttributeFilterWidget(QWidget *parent) :
    QWidget(parent)
{

    m_filterAttributesModel = new Models::ReadOnlyAttributesListModel(this);
    m_attributesFilterProxyModel = new AttributesFilterProxyModel(this);
    m_attributesFilterProxyModel->setSourceModel(m_filterAttributesModel);

    QVBoxLayout * auxLayout = new QVBoxLayout(this);
    setLayout(auxLayout);



    m_addFilterCombo = new QComboBox(this);
    QLabel *comboLabel = new QLabel(tr("Add filter"),this);
    comboLabel->setBuddy(m_addFilterCombo);
    m_addFilterCombo->setModel(m_attributesFilterProxyModel);
    connect(m_addFilterCombo,SIGNAL(activated(int)),this,SLOT(addFilterComboSelected(int)));

    m_layout = new QGridLayout();

    QHBoxLayout * hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(comboLabel);
    hLayout->addWidget(m_addFilterCombo);
    m_layout->addLayout(hLayout,0,3);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QIcon applyIcon(QString::fromUtf8(":/images/dialog-ok-apply.png"));
    QIcon clearIcon(QString::fromUtf8(":/images/edit_clear_16x16.png"));
    QPushButton *applyFilterButton = new QPushButton(applyIcon,tr("Apply"),this);
    m_clearFilterButton = new QPushButton(clearIcon,tr("Clear"),this);
    buttonsLayout->addWidget(applyFilterButton);
    buttonsLayout->addWidget(m_clearFilterButton);
    buttonsLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    auxLayout->addLayout(m_layout);
    auxLayout->addLayout(buttonsLayout);

    connect(applyFilterButton,SIGNAL(clicked()),this,SLOT(onApplyFilter));
    connect(m_clearFilterButton,SIGNAL(clicked()),this,SLOT(onClearFilter()));
}


AttributeFilterWidget::~AttributeFilterWidget()
{
    qDeleteAll(m_attributeFilterRows);
}

void AttributeFilterWidget::setAttributes(QList<const AbstractPartAttribute *> attributes)
{
    /*
    const AbstractPartAttribute * attr;
    int row=0;
    foreach(attr,attributes)
    {
        qDebug()<<"Creating filter for attr "<<attr->name();
        AttributeFilterRow *filterRow = new AttributeFilterRow(attr,m_layout,row);
        filterRow->disableFilter();
        m_attributeFilterRows[attr->id()] = filterRow;
        ++row;
    }
    */
    m_addFilterCombo->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);//Workaround a bug in QComboBox
    m_filterAttributesModel->setAttributes(attributes);
    m_addFilterCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

void AttributeFilterWidget::addFilterComboSelected(int row)
{
    if(row<0) return;
    m_addFilterCombo->setCurrentIndex(-1);
    QModelIndex proxyIndex = m_attributesFilterProxyModel->index(row,0);
    QModelIndex sourceIndex = m_attributesFilterProxyModel->mapToSource(proxyIndex);
    int modelRow = sourceIndex.row();
    const AbstractPartAttribute * attr = m_filterAttributesModel->getAttribute(modelRow);
    if(attr){
        m_filteredAttributes.append(attr->id());
        m_attributesFilterProxyModel->setFilterAttributes(&m_filteredAttributes);
        int gridRow = m_attributeFilterRows.count();
        qDebug()<<"Creating filter for attr "<<attr->name();
        AttributeFilterRow *filterRow = new AttributeFilterRow(attr,m_layout,gridRow);
        connect(filterRow,SIGNAL(filterRemoved(const AbstractPartAttribute*)),this,SLOT(filterRemoved(const AbstractPartAttribute*)));
        m_attributeFilterRows[attr->id()] = filterRow;
    }
    /*
    QVariant item = m_addFilterCombo->itemData(index, Qt::UserRole);
    if(!item.isValid()) return;
    int attrId = item.toInt();
    */
    //m_attributeFilterRows[attrId]->enableFilter();
    m_clearFilterButton->setEnabled(true);


}

void AttributeFilterWidget::filterRemoved(const AbstractPartAttribute *attribute)
{
    qDebug()<<"Removing filter for attr "<<attribute->name();
    int attrId = attribute->id();
    AttributeFilterRow* rowFilter = m_attributeFilterRows[attrId];
    int row = rowFilter->row();
    deleteRow(m_layout, row);
    m_attributeFilterRows.remove(attrId);
    delete rowFilter;
    m_filteredAttributes.removeOne(attrId);
    m_attributesFilterProxyModel->setFilterAttributes(&m_filteredAttributes);
}

void AttributeFilterWidget::deleteRow(QGridLayout *layout, int row)
{

    int columnCount = layout->columnCount()-1;
    //int rowCount = layout->rowCount()-1;
    for(int column=0;column<columnCount;++column){
        QLayoutItem *item = layout->itemAtPosition(row,column);
        if(item){
            layout->removeItem(item);
            delete item;
        }
    }

    /*
    for(int i=row;i<layout->rowCount();++i){
        int x=i+1;
        for(int column=0;column<columnCount;++column){
            QLayoutItem *item = layout->itemAtPosition(x,column);
            if(item){
                layout->removeItem(item);
                layout->addItem(item,i,column);
            }
        }
    }
    */

}

void AttributeFilterWidget::onApplyFilter()
{
}

void AttributeFilterWidget::onClearFilter()
{
    m_filteredAttributes.clear();
    foreach(AttributeFilterRow* row, m_attributeFilterRows.values()) {
        row->disableFilter();
    }
    m_clearFilterButton->setEnabled(false);

}



