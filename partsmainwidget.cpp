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
    _spinBoxDelegate = new SpinBoxDelegate(this);           
    _attributesRepo = new AttributesRepository();
    _attributesRepo->load();
    /*
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(2,true);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(4,true);
    */

    QToolBar * toolbar = new QToolBar(ui->frame_2);
    toolbar->setIconSize(QSize(24,24));
    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QAction * action1 = toolbar->addAction(QIcon(":/images/add_part_24x24.png"),"Add Part",this,SLOT(addPart()));
    QAction * action2 = toolbar->addAction(QIcon(":/images/remove_part_24x24.png"),"Remove Part",this,SLOT(removePart()));
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    toolbar->addWidget(spacerWidget);
    QSearchLineEdit * searchLineEdit = new QSearchLineEdit(this);
    searchLineEdit->setPlaceholderText("Search...");
    toolbar->addWidget(searchLineEdit);
    ui->verticalLayout_3->insertWidget(1,toolbar);   

    /*
    buildPartsModel();
    ui->tableView->setModel(&_tableModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setItemDelegateForColumn(1,_spinBoxDelegate);
    ui->tableView->setItemDelegateForColumn(2,_spinBoxDelegate);
    _tableModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    setupTableModel();
    */
    initCategoriesTree();
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setModel(&_partTableModel);    
    //_partTableModel.setCategory(Database::Database::resistorsCat);

    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(tableDoubleClicked(QModelIndex)));
}

PartsMainWidget::~PartsMainWidget()
{    
    delete ui;
    delete _spinBoxDelegate;
    qDeleteAll(_temporaryDelegates);
    _temporaryDelegates.clear();
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

static QStandardItem* createPartsItem(const QVariant &id, const QString &name, const QString &description)
{
    QStandardItem * item = new QStandardItem(QIcon(":/images/single-drawer_16x16.png"),name);
    item->setToolTip(description);
    item->setData(PartTypeNode,TREE_NODE_TYPE);
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
    dlg.exec();


}


void PartsMainWidget::buildPartsModel()
{
    qDebug("Building parts tree model");
    _treeModel = new QStandardItemModel(ui->treeView);
    QStandardItem *rootItem = _treeModel->invisibleRootItem();
    DQQuery<Category> query;
    Category cat;
    PartType partHolder;
    if(query.exec()){
        while(query.next()){
            query.recordTo(cat);
            QStandardItem * item = createCategoryItem(cat.id,cat.name,cat.description);
            rootItem->appendRow(item);
            DQQuery<PartType> partsQuery;
            partsQuery = partsQuery.filter(DQWhere("category")==cat.id);
            if(partsQuery.exec()){
                while(partsQuery.next()){
                    partsQuery.recordTo(partHolder);
                    item->appendRow(createPartsItem(partHolder.id, partHolder.name,partHolder.description));
                }
            }
        }
    }
    else{
        //TODO: Show some info
    }    
    ui->treeView->setModel(_treeModel);
    _partModel.load(1);    
    /*
    _tableModel = new PartsTableModel(&_partModel, ui->tableView);
    _tableModel->load();
    ui->tableView->setModel(_tableModel);
    */
/*
    _tableModel.setTable(_partModel.tableName());
    _tableModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    if(!_tableModel.select())
        qDebug("Failed to select");
    //_tableModel.load(1);
    /*
    _proxyModel= new QSortFilterProxyModel(this);
    _proxyModel->setDynamicSortFilter(true);
    _proxyModel->setSortRole(Qt::EditRole);
    _proxyModel->setSourceModel(&_tableModel);
    */

    /*
    connect(ui->tableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    */
    /*
    _detailsWidget = new PartDetailsWidget(ui->frame);
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            _detailsWidget, SLOT(setCurrentModelIndex(QModelIndex)));
    */

    QItemSelectionModel *selectionModel= ui->treeView->selectionModel();
    connect(selectionModel,
            SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(treeSelectionChanged(const QItemSelection &, const QItemSelection &)));

    connect(ui->treeView,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editPart(QModelIndex)));

    //QModelIndex index = _tableModel.index(1,0);
    //qDebug()<<"Valid:"<<index.isValid();
    //selectionModel->setCurrentIndex(index,QItemSelectionModel::Select);
    //ui->tableView->setCurrentIndex(index);

    //ui->tableView->edit(index);

    /*
    connect(ui->buttonBox,SIGNAL(rejected()),_detailsWidget,SLOT(revert()));
    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(buttonBoxClicked(QAbstractButton*)));

    ui->verticalLayout->insertWidget(0,_detailsWidget);
    */
    //initDetailsViewWidget();
}

void PartsMainWidget::setupTableModel()
{
    qDeleteAll(_temporaryDelegates);
    _temporaryDelegates.clear();
    _tableModel.setTable(_partModel.tableName());
    ui->tableView->setColumnHidden(0,true);
    //Setup quantity column
    _tableModel.setColumnAlignment(1, Qt::AlignRight|Qt::AlignVCenter);
    _tableModel.setHeaderData(1, Qt::Horizontal, tr("Quantity"));
    //Setup minimum quantity column
    _tableModel.setColumnAlignment(2, Qt::AlignRight|Qt::AlignVCenter);
    _tableModel.setHeaderData(2, Qt::Horizontal, tr("Minimum Quantity"));


    QAbstractItemDelegate * delegate;
    const QList<PartParameter> * params = _partModel.parameters();
    QList<PartParameter>::const_iterator it;
    int column = 3;
    for(it=params->constBegin(); it!=params->constEnd();++it){
        const PartParameter & param = (*it);
        _tableModel.setHeaderData(column, Qt::Horizontal, param.name.get().toString());
        PartParameter::ParameterType paramType = (PartParameter::ParameterType)param.type.get().toUInt();
        Qt::Alignment alignment = Qt::AlignRight|Qt::AlignVCenter;

        switch(paramType){
            case PartParameter::Text:
            case PartParameter::LongText:
                alignment = Qt::AlignLeft|Qt::AlignVCenter;
                delegate = ui->tableView->itemDelegate();
                break;
            case PartParameter::Resistance:
            case PartParameter::Capacitance:
            case PartParameter::Inductance:
                delegate = new UnitColumnDelegate(paramType);                
                _temporaryDelegates.append(delegate);
                break;
        case PartParameter::GenericNumber:
            delegate=_spinBoxDelegate;
                break;
        case PartParameter::Power:
            delegate = new DoubleSpinBoxDelegate(UnitFormatter::getUnitSymbol(paramType),1.0/8);           
            _temporaryDelegates.append(delegate);
            break;
        case PartParameter::Percentage:
            delegate = new DoubleSpinBoxDelegate(UnitFormatter::getUnitSymbol(paramType),5.0);
            _temporaryDelegates.append(delegate);
            break;
        }
        _tableModel.setColumnAlignment(column, alignment);
        ui->tableView->setItemDelegateForColumn(column, delegate);
        ++column;
    }

    if(!_tableModel.select())
        qDebug("Failed to select");
    else
        qDebug()<<"Row count is"<<_tableModel.rowCount();
}

void PartsMainWidget::buttonBoxClicked(QAbstractButton* button)
{
    /*
    QDialogButtonBox::StandardButton standardButton = ui->buttonBox->standardButton(button);
    if(standardButton & QDialogButtonBox::Apply)
        _detailsWidget->submit();
        */
}

/*
void PartsMainWidget::initDetailsViewWidget()
{
    QRegExp rx("\\b[0-9]+(\\.[0-9]+)?[k,M,G,T,P,E,Z,Y,m,u,n,p,f,a,z,y]?\\b");
    QValidator *validator = new QRegExpValidator(rx, this);

    //Build a row of widgets for the parts value
    QLabel * valueFieldLabel = new QLabel(tr("Value:"));
    QUnitLineEdit * valueFieldValue = new QUnitLineEdit(UnitFormatter::getUnitSymbol(_partModel.valueType()));
    valueFieldValue->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    valueFieldValue->setAlignment(Qt::AlignTrailing);
    valueFieldValue->setValidator(validator);
    int row = 0;
    ui->formLayout->insertRow(row++,valueFieldLabel, valueFieldValue);

    //Create a row of widgets for each parameter
    const QList<PartParameter>  * parameters = _partModel.parameters();
    for (int i = 0; i < parameters->size(); ++i) {
        PartParameter param = parameters->at(i);
        PartParameter::ParameterType valueType = (PartParameter::ParameterType)param.type.get().toUInt();

        QLabel * fieldLabel = new QLabel(param.name+":");
        QUnitLineEdit * fieldValue = new QUnitLineEdit(UnitFormatter::getUnitSymbol(valueType));
        fieldValue->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        fieldValue->setAlignment(Qt::AlignTrailing);
        fieldValue->setValidator(validator);

        ui->formLayout->insertRow(row++,fieldLabel, fieldValue);
    }
}
*/

void PartsMainWidget::currentRowChanged ( const QModelIndex & current, const QModelIndex & previous ){
   // QAbstractItemModel * model = ui->tableView->model();
    /*
    if(model!=NULL){      
        PartRow * rowData = _tableModel->rowData(current);
        qDebug()<<"Setting data in details view";
        _detailsWidget->setData(rowData);        
    }
    */
    qDebug()<<"Row changed"<<current.row();
}


void PartsMainWidget::treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    if(index.isValid()){
        QVariant nodeId = index.data(TREE_NODE_ID);
        qDebug()<<"Selected category type"<<nodeId;
        _partTableModel.setCategory(nodeId.toInt());

        /*
        QVariant nodeType = index.data(TREE_NODE_TYPE);
        if(nodeType==PartTypeNode){
            QVariant partTypeId = index.data(TREE_NODE_ID);
            qDebug()<<"Selected part type"<<partTypeId;
            _partModel.load(partTypeId.toInt());
            setupTableModel();          
        }
        */
    }
}


/*
void PartsMainWidget::treeCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    qDebug()<<"Node changed"<<current;
    //_treeModel->data()
}
*/

QSqlTableModelWithAlignment::QSqlTableModelWithAlignment(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{
}

QVariant QSqlTableModelWithAlignment::data(const QModelIndex &idx, int role) const
{
    if(role==Qt::TextAlignmentRole && idx.isValid()){
        int column = idx.column();
        if(column<_alignments.size()){
            int value = _alignments.at(column);
            if(value!=0)
                return QVariant(value);
        }
    }
    return QSqlTableModel::data(idx, role);
}

void QSqlTableModelWithAlignment::setColumnAlignment(int column, Qt::Alignment alignment)
{
    if(column>=_alignments.size())
        _alignments.resize(column*2);
    _alignments.replace(column, alignment);
}

void QSqlTableModelWithAlignment::setTable(const QString &tableName)
{
    _alignments.clear();
    QSqlTableModel::setTable(tableName);
}

