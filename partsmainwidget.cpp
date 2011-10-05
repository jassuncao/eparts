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
    //query = query.filter(DQWhere("partType") == _partTypeId);
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

    /*
    QSqlQuery categoriesQuery("SELECT id,name,description FROM category");
    QSqlQuery partsQuery;
    partsQuery.prepare("SELECT id,name,description FROM part_definition WHERE fk_category=:category");
    while(categoriesQuery.next()){
        QVariant id = categoriesQuery.value(0);
        QString name = categoriesQuery.value(1).toString();
        QString description = categoriesQuery.value(2).toString();
        qDebug()<<"Creating category "<<name;
        QStandardItem * item = createCategoryItem(id,name,description);
        rootItem->appendRow(item);

        partsQuery.bindValue(0,id);
        partsQuery.exec();
        while(partsQuery.next()){
            QVariant partId = partsQuery.value(0);
            QString partName = partsQuery.value(1).toString();
            QString partDescription = partsQuery.value(2).toString();
            qDebug()<<"Creating part "<<partName;
            item->appendRow(createPartsItem(partId, partName,partDescription));
        }
    }
    */
    ui->treeView->setModel(model);
    PartsTableModel * tableModel = new PartsTableModel(1, ui->tableView);
    tableModel->load();
    ui->tableView->setModel(tableModel);
    initDetailsViewWidget();
}

//static QWidget* createValueWi

void PartsMainWidget::initDetailsViewWidget()
{
    PartType partType;
    partType.load(DQWhere("id")==1);
    QVBoxLayout *verticalLayout = new QVBoxLayout();

    QLabel * valueFieldLabel = new QLabel();
    valueFieldLabel->setText(tr("Value:"));
    QLineEdit * valueFieldValue = new QLineEdit();
    QRegExp rx("\\b[0-9]+(\\.[0-9]+)?[k,M,G,T,P,E,Z,Y,m,u,n,p,f,a,z,y]?\\b");
    QValidator *validator = new QRegExpValidator(rx, this);
    valueFieldValue->setValidator(validator);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(valueFieldLabel);
    layout->addWidget(valueFieldValue);    
    PartParameter::ParameterType valueType = (PartParameter::ParameterType)partType.valueType.get().toUInt();
    layout->addWidget(new QLabel(UnitFormatter::getUnitSymbol(valueType)));
    verticalLayout->addLayout(layout);

    DQQuery<PartParameter> parametersQuery;
    parametersQuery = parametersQuery.filter(DQWhere("partType") == partType.id).orderBy("orderIndex");
    if(parametersQuery.exec()){
        while(parametersQuery.next()){
            PartParameter param = parametersQuery.record();

            QLabel * fieldLabel = new QLabel();
            fieldLabel->setText(param.name);
            QLineEdit * fieldValue = new QLineEdit();

            QHBoxLayout *fieldLayout = new QHBoxLayout();
            fieldLayout->setObjectName("fieldLayout2");
            fieldLayout->addWidget(fieldLabel);
            fieldLayout->addWidget(fieldValue);
            PartParameter::ParameterType valueType = (PartParameter::ParameterType)param.type.get().toUInt();
            fieldLayout->addWidget(new QLabel(UnitFormatter::getUnitSymbol(valueType)));
            verticalLayout->addLayout(fieldLayout);
        }
    }
    ui->frame1->setLayout(verticalLayout);

}
