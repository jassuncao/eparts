#include "categoriesform.h"
#include "ui_categoriesform.h"
#include "models/categorieslistmodel.h"
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
    connect(ui->listView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doubleClicked(QModelIndex)));
    ui->removeButton->setEnabled(false);
    ui->editButton->setEnabled(false);
}

CategoriesForm::~CategoriesForm()
{
    delete ui;
    delete _model;
}

void CategoriesForm::currentRowChanged ( const QModelIndex & current, const QModelIndex & previous )
{
    bool validRow = current.isValid();
    ui->removeButton->setEnabled(validRow);
    ui->editButton->setEnabled(validRow);
    qDebug()<<"Row Changed:"<<current.row();
}

void CategoriesForm::doubleClicked(const QModelIndex & index)
{
    if(!index.isValid())
        return;
    int row = index.row();
    Category cat = _model->getCategory(row);
    CategoryDialog dlg(parentWidget());
    dlg.setModel(&cat);
    if(dlg.exec()==QDialog::Accepted){
        qDebug()<<"Editing";
        if(dlg.model()){
            _model->updateCategory(row, *dlg.model());
            _model->saveChanges();
        }
    }
}

void CategoriesForm::addButtonClicked()
{
    CategoryDialog dlg(parentWidget());
    if(dlg.exec()==QDialog::Accepted){
        qDebug()<<"Adding";
        if(dlg.model()){
            _model->add(dlg.model());
            _model->saveChanges();
        }
    }
}

void CategoriesForm::editButtonClicked()
{

    QModelIndex index = ui->listView->selectionModel()->currentIndex();
    doubleClicked(index);
}

void CategoriesForm::removeButtonClicked()
{
    QModelIndex index = ui->listView->selectionModel()->currentIndex();
    if(!index.isValid())
        return;
    int row = index.row();
    _model->removeRow(row);
    _model->saveChanges();

}

void CategoriesForm::accept()
{
    _model->saveChanges();
}

void CategoriesForm::reject()
{

}
