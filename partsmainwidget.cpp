#include "partsmainwidget.h"
#include "ui_partsmainwidget.h"
#include "models/partstablemodel.h"
#include <QStandardItem>
#include <dquest.h>
#include "category.h"
#include "parttype.h"
#include "partparameter.h"
#include <QLineEdit>
#include <QLabel>
#include "unitformatter.h"
#include "widgets/qunitlineedit.h"
#include <QAbstractButton>

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
    buildPartsModel();
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(2,true);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(4,true);
}

PartsMainWidget::~PartsMainWidget()
{
    delete ui;
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
    _tableModel.load(1);
    ui->tableView->setModel(&_tableModel);
    /*
    connect(ui->tableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    */
    _detailsWidget = new PartDetailsWidget(ui->frame);
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            _detailsWidget, SLOT(setCurrentModelIndex(QModelIndex)));


    QItemSelectionModel *selectionModel= ui->treeView->selectionModel();
    connect(selectionModel,
            SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(treeSelectionChanged(const QItemSelection &, const QItemSelection &)));

    connect(ui->buttonBox,SIGNAL(rejected()),_detailsWidget,SLOT(revert()));
    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(buttonBoxClicked(QAbstractButton*)));

    ui->verticalLayout->insertWidget(0,_detailsWidget);
    //initDetailsViewWidget();
}

void PartsMainWidget::buttonBoxClicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton standardButton = ui->buttonBox->standardButton(button);
    if(standardButton & QDialogButtonBox::Apply)
        _detailsWidget->submit();
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
    QAbstractItemModel * model = ui->tableView->model();
    if(model!=NULL){
        /*
        PartRow * rowData = _tableModel->rowData(current);
        qDebug()<<"Setting data in details view";
        _detailsWidget->setData(rowData);
        */
    }
    qDebug()<<"Row changed"<<current.row();
}


void PartsMainWidget::treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    if(index.isValid()){
        QVariant nodeType = index.data(TREE_NODE_TYPE);
        if(nodeType==PartTypeNode){
            QVariant partTypeId = index.data(TREE_NODE_ID);
            qDebug()<<"Selected part type"<<partTypeId;
            _partModel.load(partTypeId.toInt());
            /*
            delete _tableModel;
            _tableModel = new PartsTableModel(&_partModel, ui->tableView);
            _tableModel->load();
            ui->tableView->setModel(_tableModel);
            */
            _tableModel.load(partTypeId.toInt());
            _detailsWidget->setModel(_tableModel.partTypeModel(), &_tableModel);
        }
    }
}

/*
void PartsMainWidget::treeCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    qDebug()<<"Node changed"<<current;
    //_treeModel->data()
}
*/
