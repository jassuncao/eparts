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


PartsMainWidget::PartsMainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PartsMainWidget)
{
    ui->setupUi(this);
    buildPartsModel();
}

PartsMainWidget::~PartsMainWidget()
{
    delete ui;
}

static QStandardItem* createCategoryItem(const QVariant &id, const QString &name, const QString &description)
{
    QStandardItem * item = new QStandardItem(QIcon(":/images/drawer-close_16x16.png"),name);
    item->setToolTip(description);
    item->setData(id);
    item->setEditable(false);
    return item;
}

static QStandardItem* createPartsItem(const QVariant &id, const QString &name, const QString &description)
{
    QStandardItem * item = new QStandardItem(QIcon(":/images/single-drawer_16x16.png"),name);
    item->setToolTip(description);
    item->setData(id);
    item->setEditable(false);
    return item;
}

void PartsMainWidget::buildPartsModel()
{
    qDebug("Building parts tree model");
    QStandardItemModel * model = new QStandardItemModel(ui->treeView);
    QStandardItem *rootItem = model->invisibleRootItem();
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

    ui->treeView->setModel(model);
    _partModel.load(1);
    _tableModel = new PartsTableModel(&_partModel, ui->tableView);
    _tableModel->load();
    ui->tableView->setModel(_tableModel);
    connect(ui->tableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    _detailsWidget = new PartDetailsWidget(&_partModel, ui->frame);

    ui->verticalLayout->insertWidget(0,_detailsWidget);
    //initDetailsViewWidget();
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
        PartRow * rowData = _tableModel->rowData(current);
        qDebug()<<"Setting data in details view";
        _detailsWidget->setData(rowData);
    }
    qDebug()<<"Row changed"<<current.row();
}
