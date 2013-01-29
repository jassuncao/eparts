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

using namespace Widgets;

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

    QToolBar * toolbar = new QToolBar(ui->frame_2);
    toolbar->setIconSize(QSize(24,24));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolbar->addAction(QIcon(":/images/add_part_24x24.png"),"Add Part",this,SLOT(addPart()));
    toolbar->addAction(QIcon(":/images/remove_part_24x24.png"),"Remove Part",this,SLOT(removePart()));
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    toolbar->addWidget(spacerWidget);
    QSearchLineEdit * searchLineEdit = new QSearchLineEdit(this);
    searchLineEdit->setPlaceholderText("Search...");
    toolbar->addWidget(searchLineEdit);
    ui->verticalLayout_3->insertWidget(1,toolbar);   

    initCategoriesTree();
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setModel(&_partTableModel);    
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(tableDoubleClicked(QModelIndex)));
}

PartsMainWidget::~PartsMainWidget()
{    
    delete ui;    
}

void PartsMainWidget::addPart()
{

}

void PartsMainWidget::removePart()
{
}

static QStandardItem* createCategoryItem(const QVariant &id, const QString &name, const QString &description)
{
    QStandardItem * item = new QStandardItem(QIcon(":/images/drawer-close_16x16.png"),name);
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
        QVariant nodeId = index.data(TREE_NODE_ID);
        qDebug()<<"Selected category type"<<nodeId;
        _partTableModel.setCategory(nodeId.toInt());       
    }
}
