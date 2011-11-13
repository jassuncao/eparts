#include "categoriesform.h"
#include "ui_categoriesform.h"
#include "categorieslistmodel.h"
#include "categorydialog.h"

CategoriesForm::CategoriesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CategoriesForm)
{
    ui->setupUi(this);
    _model = new CategoriesListModel();
    _model->load();
    QListView * listView = ui->listView;
    listView->setModel(_model);
    connect(listView->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    connect(ui->addButton,SIGNAL(clicked()),this, SLOT(addButtonClicked()));
    connect(ui->removeButton,SIGNAL(clicked()),this, SLOT(removeButtonClicked()));
    connect(ui->editButton,SIGNAL(clicked()),this, SLOT(editButtonClicked()));
    ui->removeButton->setEnabled(false);
    ui->editButton->setEnabled(false);
}

CategoriesForm::~CategoriesForm()
{
    delete ui;
}

void CategoriesForm::currentRowChanged ( const QModelIndex & current, const QModelIndex & previous )
{
    bool validRow = current.isValid();
    ui->removeButton->setEnabled(validRow);
    ui->editButton->setEnabled(validRow);
}

void CategoriesForm::addButtonClicked()
{
    CategoryDialog dlg;
    if(dlg.exec()==QDialog::Accepted){
        qDebug()<<"AADDIINNGG";
        if(dlg.model())
            _model->add(dlg.model());
    }
}

void CategoriesForm::editButtonClicked()
{

}

void CategoriesForm::removeButtonClicked()
{
    QModelIndexList selectedRows = ui->listView->selectionModel()->selectedRows();
    int row = selectedRows.at(0).row();
    _model->removeRow(row);

}

void CategoriesForm::accept()
{
    _model->saveChanges();
}

void CategoriesForm::reject()
{

}
