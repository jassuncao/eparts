#include "partsmainwidget.h"
#include "ui_partsmainwidget.h"
#include "models/partstablemodel.h"
#include <QStandardItem>
#include <QtSql>
#include <dquest.h>
#include "database/category.h"
#include "database/parttype.h"
#include "database/partparameter.h"
#include <QLineEdit>
#include <QLabel>
#include "unitformatter.h"
#include "widgets/qunitlineedit.h"
#include <QtGui>
#include "spinboxdelegate.h"
#include "widgets/qsearchlineedit.h"
#include "unitcolumndelegate.h"
#include "database/entities.h"
#include "database/database.h"
#include "editpartdialog.h"
#include "widgets/qcloseableheaderview.h"

using namespace Widgets;
using namespace Models;

struct Node {
    int * _parentId;
    QStandardItem * _item;

    Node(QStandardItem * item, int * parent=0) :
        _parentId(parent), _item(item)
    {}
};

static const int TREE_NODE_TYPE = Qt::UserRole+1;
static const int TREE_NODE_ID = Qt::UserRole+2;
enum NodeType{
    CategoryNode, PartTypeNode
};

PartsMainWidget::PartsMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PartsMainWidget)
{
    ui->setupUi(this);                  
    _attributesRepo = new AttributesRepository();
    _attributesRepo->load();    

    /*
    QToolBar * toolbar = new QToolBar(ui->frame_2);
    toolbar->setIconSize(QSize(24,24));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolbar->addAction(QIcon(":/images/add_part_24x24.png"),"Add Part",this,SLOT(addPart()));
    toolbar->addAction(QIcon(":/images/remove_part_24x24.png"),"Remove Part",this,SLOT(removePart()));
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    toolbar->addWidget(spacerWidget);
    */

    initCategoriesTree();    
    m_partsFilterProxyModel.setSourceModel(&_partTableModel);
    ui->tableView->setModel(&m_partsFilterProxyModel);
    ui->tableView->setSortingEnabled(true);
    QCloseableHeaderView * header = new QCloseableHeaderView(Qt::Horizontal, ui->tableView);
    ui->tableView->setHorizontalHeader(header);
    const QList<const AbstractPartAttribute*> attributes;
    initAddFilterCombo(attributes);

    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(tableDoubleClicked(QModelIndex)));    
    connect(ui->addFilterComboBox,SIGNAL(activated(int)),this,SLOT(addFilterComboSelected(int)));
    connect(ui->clearFilterButton,SIGNAL(clicked()),this,SLOT(clearFilter()));
    connect(ui->applyFilterButton,SIGNAL(clicked()),this,SLOT(applyFilter()));
}

PartsMainWidget::~PartsMainWidget()
{    
    delete ui;
    qDeleteAll(m_attributeFilterRows);
}

void PartsMainWidget::addFilterForAttribute(int attributeId)
{
    _attributesRepo->findById(attributeId);
}

void PartsMainWidget::initAddFilterCombo(const QList<const AbstractPartAttribute *> &attributes)
{
    const AbstractPartAttribute* attr;
    QStandardItemModel * model = new QStandardItemModel(0,1,ui->addFilterComboBox);
    QStandardItem * emptyItem = new QStandardItem(tr("<filter attribute>"));
    emptyItem->setSelectable(false);
    model->appendRow(emptyItem);
    foreach(attr, attributes){
        QStandardItem * item = new QStandardItem(attr->name());
        item->setData(VPtr<const AbstractPartAttribute>::asQVariant(attr));
        model->appendRow(item);
    }
    ui->addFilterComboBox->setEnabled(attributes.count()>0);
    ui->addFilterComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);//Workaround for a bug in QComboBox
    ui->addFilterComboBox->setModel(model);
    ui->addFilterComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->clearFilterButton->setEnabled(m_attributeFilterRows.count()>0);
}

void PartsMainWidget::initAddFilterCombo()
{
    QStandardItemModel * model = new QStandardItemModel(0,1,ui->addFilterComboBox);
    QStandardItem * emptyItem = new QStandardItem(tr("<filter attribute>"));
    emptyItem->setSelectable(false);
    model->appendRow(emptyItem);
    foreach(const PartColumn * column, _partTableModel.columns()){
        QStandardItem * item = new QStandardItem(column->label());
        item->setData(VPtr<const PartColumn>::asQVariant(column));
        model->appendRow(item);
    }
    ui->addFilterComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);//Workaround for a bug in QComboBox
    ui->addFilterComboBox->setModel(model);
    ui->addFilterComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->addFilterComboBox->setEnabled(true);
    ui->clearFilterButton->setEnabled(m_attributeFilterRows.count()>0);

}

void PartsMainWidget::addPart()
{
    EditPartDialog dlg(_attributesRepo, this);
    dlg.setCategory(_partTableModel.category());
    dlg.setPart(-1);
    if(dlg.exec()==QDialog::Accepted)
        _partTableModel.setDirty(dlg.part());
}

void PartsMainWidget::removePart()
{
}

void PartsMainWidget::applyFilter()
{
    if(m_attributeFilterRows.count()==0) return;
    QMap<int,PartFilterRow*>::const_iterator it = m_attributeFilterRows.constBegin();
    while (it != m_attributeFilterRows.constEnd()) {
        int column = it.key();
        PartFilterRow * filterRow = it.value();
    }
}

void PartsMainWidget::clearFilter()
{    
    qDeleteAll(m_attributeFilterRows);
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->addFilterComboBox->model());
    if(model!=0){
        int count = ui->addFilterComboBox->count();
        for(int i=1;i<count;++i)
            model->item(i)->setEnabled(true);
    }
    ui->clearFilterButton->setEnabled(false);
}

void PartsMainWidget::addFilterComboSelected(int index)
{
    if(index<0) return;
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->addFilterComboBox->model());
    if(model==0){
        qWarning("Model for filter ComboBox not set");
        return;
    }
    QStandardItem *item = model->item(index);
    item->setEnabled(false);
    const PartColumn* column =VPtr<const PartColumn>::asPtr(item->data());
    qDebug("Select attribute %s",qPrintable(column->label()));
    PartFilterRow * filterRow = new PartFilterRow(column->label(),column->type(),ui->filterGridLayout,index);
    //AttributeFilterRow *filterRow = new AttributeFilterRow(column,ui->filterGridLayout,row, index);
    m_attributeFilterRows[index]=filterRow;
    connect(filterRow,SIGNAL(filterRemoved(int)),this,SLOT(filterRemoved(int)));
    ui->clearFilterButton->setEnabled(true);
}

void PartsMainWidget::filterRemoved(int tag)
{    
    PartFilterRow *filterRow = m_attributeFilterRows.value(tag);
    if(filterRow){
        //int row = filterRow->row();
        //int comboxIndex = filterRow->tag();
        /*
        QGridLayout *layout = ui->filterGridLayout;
        for(int column=0;column<3;++column){
            QLayoutItem *item = layout->itemAtPosition(row,column);
            if(item){
                layout->removeItem(item);
                delete item;
            }
        }
        */
        m_attributeFilterRows.remove(tag);
        filterRow->deleteLater();
        //Reenable the item in the combo
        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->addFilterComboBox->model());
        if(model==0){
            qWarning("Model for filter ComboBox not set");
            return;
        }
        QStandardItem *item = model->item(tag);
        item->setEnabled(true);
        ui->clearFilterButton->setEnabled(m_attributeFilterRows.count()>0);
    }
}

static QStandardItem* createCategoryItem(const QVariant &id, const QString &name, const QString &description)
{
    QStandardItem * item = new QStandardItem(/*QIcon(":/images/drawer-close_16x16.png"),*/name);
    item->setToolTip(description);
    item->setData(CategoryNode,TREE_NODE_TYPE);
    item->setData(id,TREE_NODE_ID);
    item->setEditable(false);
    return item;
}

void PartsMainWidget::initCategoriesTree()
{
    QMap<int,struct Node*> lookup;
    QStandardItem * item;
    struct Node * node;
    DQQuery<DQCategory> query;
    QStandardItem *rootItem;
    DQCategory cat;

    qDebug("Building categories tree model");
    _treeModel = new QStandardItemModel(ui->treeView);
    rootItem = _treeModel->invisibleRootItem();
    if(query.exec()){
        while(query.next()){
            query.recordTo(cat);
            item = createCategoryItem(cat.id,cat.name,cat.description);
            int * parent = 0;
            if(!cat.parent.get().isNull()){
                parent = new int(cat.parent.get().toInt());
            }
            lookup[cat.id.get().toInt()]= new Node(item,parent);
        }
        QList<Node*> nodes = lookup.values();
        foreach(node,nodes){
            if(node->_parentId){
                lookup[*node->_parentId]->_item->appendRow(node->_item);
            }
            else{
                rootItem->appendRow(node->_item);
            }
        }
        qDeleteAll(lookup);
    }
    else{
        //TODO: Show some error msg
    }
    ui->treeView->setModel(_treeModel);
    QItemSelectionModel *selectionModel= ui->treeView->selectionModel();
    connect(selectionModel,
            SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(treeSelectionChanged(const QItemSelection &, const QItemSelection &)));
}

void PartsMainWidget::tableDoubleClicked(const QModelIndex &index)
{
    int partId = _partTableModel.itemFromIndex(index);
    if(partId==-1)
        return;
    EditPartDialog dlg(_attributesRepo, this);
    dlg.setPart(partId);
    if(dlg.exec()==QDialog::Accepted)
        _partTableModel.setDirty(partId);
}

void PartsMainWidget::currentRowChanged ( const QModelIndex & current, const QModelIndex & previous ){  
    qDebug()<<"Row changed"<<current.row();
}


void PartsMainWidget::treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    if(index.isValid()){
        int categoryId = index.data(TREE_NODE_ID).toInt();
        qDebug()<<"Selected category type"<<categoryId;
        _partTableModel.setCategory(categoryId);        
        clearFilter();                
        //initAddFilterCombo(_attributesRepo->listCategoryAttributes(categoryId));
        initAddFilterCombo();
    }
}
