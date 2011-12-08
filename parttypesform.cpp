#include "parttypesform.h"
#include "ui_parttypesform.h"
#include "parttypedialog.h"

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
    connect(tableView->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    connect(ui->addButton,SIGNAL(clicked()),this, SLOT(addButtonClicked()));
    connect(ui->removeButton,SIGNAL(clicked()),this, SLOT(removeButtonClicked()));
    connect(ui->editButton,SIGNAL(clicked()),this, SLOT(editButtonClicked()));
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));
    ui->removeButton->setEnabled(false);
    ui->editButton->setEnabled(false);
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

void PartTypesForm::currentRowChanged ( const QModelIndex & current, const QModelIndex & previous )
{
    bool validRow = current.isValid();
    ui->removeButton->setEnabled(validRow);
    ui->editButton->setEnabled(validRow);
    qDebug()<<"Row Changed:"<<current.row();
}

void PartTypesForm::doubleClicked(const QModelIndex & index)
{
    if(!index.isValid())
        return;
    int row = index.row();
    PartType part = _model->rowData(row);

    PartTypeDialog dlg(parentWidget());
    dlg.setModel(&part);
    if(dlg.exec()==QDialog::Accepted){
        qDebug()<<"Editing";
        if(dlg.model()){
            _model->setRowData(row, *dlg.model());            
        }
    }
}

void PartTypesForm::addButtonClicked()
{
    PartTypeDialog dlg(parentWidget());
    PartType newPart;
    newPart.name = "New Part";
    dlg.setModel(&newPart);
    if(dlg.exec()==QDialog::Accepted){
        qDebug()<<"Adding";
        if(dlg.model())
            _model->add(*dlg.model());
    }
}

void PartTypesForm::editButtonClicked()
{

    QModelIndex index = ui->tableView->selectionModel()->currentIndex();
    doubleClicked(index);
}

void PartTypesForm::removeButtonClicked()
{
    QModelIndex index = ui->tableView->selectionModel()->currentIndex();
    if(!index.isValid())
        return;
    int row = index.row();
    _model->removeRow(row);

}
