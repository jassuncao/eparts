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
    initTypesCombo();    
    connect(ui->nameEditor,SIGNAL(textChanged(QString)),this, SLOT(validateInput()));
    connect(ui->typeCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(validateInput()));
    ui->typeCombo->setCurrentIndex(-1);
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

void EditAttributeDialog::initTypesCombo()
{
    QComboBox *combo = ui->typeCombo;
    combo->clear();
    combo->addItem(tr("Text"),QVariant(Models::ATTRIBUTE_TEXT));
    combo->addItem(tr("Generic Number"),QVariant(Models::ATTRIBUTE_GENERIC_NUMBER));
    combo->addItem(tr("Resistance"),QVariant(Models::ATTRIBUTE_RESISTANCE));
    combo->addItem(tr("Capacitance"),QVariant(Models::ATTRIBUTE_CAPACITANCE));
    combo->addItem(tr("Inductance"),QVariant(Models::ATTRIBUTE_INDUCTANCE));
    combo->addItem(tr("Power"),QVariant(Models::ATTRIBUTE_POWER));
    combo->addItem(tr("Percentage"),QVariant(Models::ATTRIBUTE_PERCENTAGE));
}

void EditAttributeDialog::validateInput()
{
    bool valid = true;
    valid=valid && !ui->nameEditor->text().isEmpty();
    valid=valid && ui->typeCombo->currentIndex()>=0;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}
