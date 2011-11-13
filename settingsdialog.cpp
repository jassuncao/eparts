#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    _categoriesForm = new CategoriesForm();
    ui->tabWidget->addTab(_categoriesForm,"Categories");
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    _categoriesForm->accept();
    QDialog::accept();
}

void SettingsDialog::reject()
{
    _categoriesForm->reject();
    QDialog::reject();
}
