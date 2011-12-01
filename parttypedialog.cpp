#include "parttypedialog.h"
#include "ui_parttypedialog.h"
#include "category.h"
#include "partparameter.h"

PartTypeDialog::PartTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PartTypeDialog)
{
    ui->setupUi(this);
    initCategoriesCombo();
    initFieldTypeCombos();
}

PartTypeDialog::~PartTypeDialog()
{
    delete ui;
}

void PartTypeDialog::initCategoriesCombo()
{
    DQQuery<Category> query;
    query = query.orderBy("name");
    if(query.exec()){
        while(query.next()){
            Category cat;
            query.recordTo(cat);
            ui->partCategoryCombo->addItem(cat.name,cat.id);
        }
    }
}

void PartTypeDialog::initFieldTypeCombos()
{
    ui->fieldCombo->addItem(tr("Text"),PartParameter::Text);
    ui->fieldCombo->addItem(tr("Generic Number"),PartParameter::GenericNumber);
    ui->fieldCombo->addItem(tr("Resistance"),PartParameter::Resistance);
    ui->fieldCombo->addItem(tr("Capacitance"),PartParameter::Capacitance);
    ui->fieldCombo->addItem(tr("Inductance"),PartParameter::Power);
    ui->fieldCombo->addItem(tr("Percentage"),PartParameter::Percentage);
    for(int i =0; i<ui->fieldCombo->count();++i)
    {
        QString text = ui->fieldCombo->itemText(i);
        QVariant userData = ui->fieldCombo->itemData(i);
        ui->partMainFieldCombo->addItem(text, userData);
    }
}
