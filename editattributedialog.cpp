#include "editattributedialog.h"
#include "ui_editattributedialog.h"
#include "models/partattribute.h"
#include <QPushButton>
#include "database/entities.h"
#include <dquest.h>

EditAttributeDialog::EditAttributeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAttributeDialog)
{
    ui->setupUi(this);    
    initCombos();
    connect(ui->nameEditor,SIGNAL(textChanged(QString)),this, SLOT(validateInput()));
    connect(ui->typeCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(typeComboChanged(int)));
    connect(ui->unitCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(validateInput()));
    ui->typeCombo->setCurrentIndex(-1);
    ui->unitCombo->setCurrentIndex(-1);
    validateInput();
}

EditAttributeDialog::~EditAttributeDialog()
{
    delete ui;
}

QString EditAttributeDialog::attributeName() const
{
    return ui->nameEditor->text();
}

QString EditAttributeDialog::attributeDescription() const
{
    return ui->descriptionEditor->text();
}

int EditAttributeDialog::attributeType() const
{
    int typeIdx = ui->typeCombo->currentIndex();
    if(typeIdx>=0){
        QVariant tmp = ui->typeCombo->itemData(typeIdx);
        return tmp.toInt();
    }
    return -1;
}

int EditAttributeDialog::attributeUnit() const
{
    int idx = ui->unitCombo->currentIndex();
    if(idx>=0){
        QVariant tmp = ui->unitCombo->itemData(idx);
        return tmp.toInt();
    }
    return -1;
}

void EditAttributeDialog::setAttributeName(const QString &name)
{
    ui->nameEditor->setText(name);
}

void EditAttributeDialog::setAttributeDescription(const QString &description)
{
    ui->descriptionEditor->setText(description);
}

void EditAttributeDialog::setAttributeType(int type)
{
    int idx=ui->typeCombo->findData(QVariant(type));
    ui->typeCombo->setCurrentIndex(idx);
}

void EditAttributeDialog::initCombos()
{
    QComboBox *typeCombo = ui->typeCombo;
    typeCombo->clear();
    typeCombo->addItem(tr("Text"),QVariant(Models::ATTRIBUTE_TEXT));
    typeCombo->addItem(tr("Float"),QVariant(Models::ATTRIBUTE_GENERIC_FLOAT));
    typeCombo->addItem(tr("Integer"),QVariant(Models::ATTRIBUTE_GENERIC_INTEGER));
    typeCombo->addItem(tr("Unit"),QVariant(Models::ATTRIBUTE_UNIT));
    typeCombo->addItem(tr("Percentage"),QVariant(Models::ATTRIBUTE_PERCENTAGE));

    QComboBox *unitCombo = ui->unitCombo;
    unitCombo->clear();
    DQQuery<DQUnit> query;
    if(query.exec()){
        DQUnit unit;
        while(query.next()){
            query.recordTo(unit);
            QString text("%1 (%2)");
            text=text.arg(unit.name.get().toString(),unit.symbol.get().toString());
            unitCombo->addItem(text,unit.id);
        }
    }
    unitCombo->setEnabled(false);
}

void EditAttributeDialog::validateInput()
{
    bool valid = true;
    valid=valid && !ui->nameEditor->text().isEmpty();
    int typeIdx = ui->typeCombo->currentIndex();
    valid=valid && typeIdx>=0;
    QVariant userData = ui->typeCombo->itemData(typeIdx);
    if(userData.isValid() && userData.toInt()==Models::ATTRIBUTE_UNIT){
        valid=valid&&ui->unitCombo->currentIndex()>=0;
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}

void EditAttributeDialog::typeComboChanged(int index)
{
    QVariant userData = ui->typeCombo->itemData(index);
    if(userData.isValid() && userData.toInt()==Models::ATTRIBUTE_UNIT)
        ui->unitCombo->setEnabled(true);
    else{
        ui->unitCombo->setEnabled(false);
        ui->unitCombo->setCurrentIndex(-1);
    }
    validateInput();
}
