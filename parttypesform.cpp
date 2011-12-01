#include "parttypesform.h"
#include "ui_parttypesform.h"

PartTypesForm::PartTypesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PartTypesForm)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setVisible(true);
    _model = new PartTypeTableModel();
    _model->load();
    QTableView * tableView = ui->tableView;
    tableView->setModel(_model);
}

PartTypesForm::~PartTypesForm()
{
    delete ui;
    delete _model;
}

void PartTypesForm::accept()
{

}

void PartTypesForm::reject()
{

}
