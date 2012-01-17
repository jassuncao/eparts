#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    _categoriesForm = new CategoriesForm();
    _partForm = new PartTypesForm();
    ui->tabWidget->addTab(_categoriesForm,"Categories");
    ui->tabWidget->addTab(_partForm,"Parts");
}

SettingsDialog::~SettingsDialog()
{    
    delete ui;
}

void SettingsDialog::accept()
{
    _categoriesForm->accept();
    _partForm->accept();
    QDialog::accept();
}

void SettingsDialog::reject()
{
    _categoriesForm->reject();
    _partForm->reject();
    QDialog::reject();
}
