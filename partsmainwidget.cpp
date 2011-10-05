#include "partsmainwidget.h"
#include "ui_partsmainwidget.h"
#include "partstablemodel.h"
//#include <QtSql>
#include <QStandardItem>
#include <dquest.h>
#include "category.h"
#include "parttype.h"
#include "partparameter.h"
#include <QLineEdit>
#include <QLabel>
#include "unitformatter.h"


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
    PartsTableModel * tableModel = new PartsTableModel(&_partModel, ui->tableView);
    tableModel->load();
    ui->tableView->setModel(tableModel);
    initDetailsViewWidget();
}

void PartsMainWidget::initDetailsViewWidget()
{
    QRegExp rx("\\b[0-9]+(\\.[0-9]+)?[k,M,G,T,P,E,Z,Y,m,u,n,p,f,a,z,y]?\\b");
    QValidator *validator = new QRegExpValidator(rx, this);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setMargin(0);

    //Build a row of widgets for the parts value
    QLabel * valueFieldLabel = new QLabel(tr("Value:"));
    QLineEdit * valueFieldValue = new QLineEdit(); 
    valueFieldValue->setValidator(validator);
    QLabel * unitLabel = new QLabel(UnitFormatter::getUnitSymbol(_partModel.valueType()));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(valueFieldLabel);
    layout->addWidget(valueFieldValue);        
    layout->addWidget(unitLabel);
    layout->addStretch(0);
    verticalLayout->addLayout(layout);

    //Create a row of widgets for each parameter
    const QList<PartParameter>  * parameters = _partModel.parameters();
    for (int i = 0; i < parameters->size(); ++i) {
        PartParameter param = parameters->at(i);
        PartParameter::ParameterType valueType = (PartParameter::ParameterType)param.type.get().toUInt();

        QLabel * fieldLabel = new QLabel(param.name+":");
        QLineEdit * fieldValue = new QLineEdit();
        QLabel * fieldUnitLabel = new QLabel(UnitFormatter::getUnitSymbol(valueType));

        QHBoxLayout *fieldLayout = new QHBoxLayout();
        fieldLayout->setObjectName("fieldLayout2");
        fieldLayout->addWidget(fieldLabel);
        fieldLayout->addWidget(fieldValue);
        fieldLayout->addWidget(fieldUnitLabel);
        fieldLayout->addStretch(0);
        verticalLayout->addLayout(fieldLayout);
    }
    ui->frame1->setLayout(verticalLayout);
}


