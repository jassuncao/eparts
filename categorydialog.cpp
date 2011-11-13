#include "categorydialog.h"
#include "ui_categorydialog.h"

CategoryDialog::CategoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryDialog), _model(0)
{
    ui->setupUi(this);
}

CategoryDialog::~CategoryDialog()
{
    delete ui;
}

Category * CategoryDialog::model() const
{
    return _model;
}

void CategoryDialog::setModel(Category * model)
{
    _model = model;
    if(_model)
    {
        ui->nameLineEdit->setText(_model->name);
        ui->descriptionLineEdit->setText(_model->description);
    }
}

void CategoryDialog::accept()
{

    if(!_model){
        _model = new Category();
    }
    _model->name.set(ui->nameLineEdit->text());
    _model->description.set(ui->descriptionLineEdit->text());
    QDialog::accept();
}
