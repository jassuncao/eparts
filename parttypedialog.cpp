#include "parttypedialog.h"
#include "ui_parttypedialog.h"
#include "category.h"
#include "partparameter.h"
#include "models/partparameterslistmodel.h"

PartTypeDialog::PartTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PartTypeDialog)
{
    ui->setupUi(this);
    ui->addFieldButton->setEnabled(true);
    ui->removeFieldButton->setEnabled(false);
    ui->fieldUpButton->setEnabled(false);
    ui->fieldDownButton->setEnabled(false);
    _paramsModel = new PartParametersListModel();

    QListView * fieldListView = ui->fieldListView;
    fieldListView->setModel(_paramsModel);

    connect(fieldListView->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(paramsViewCurrentRowChanged(QModelIndex,QModelIndex)));
    connect(ui->fieldUpButton,SIGNAL(clicked()), this, SLOT(slotMoveFieldUp()));
    connect(ui->fieldDownButton,SIGNAL(clicked()), this, SLOT(slotMoveFieldDown()));
    connect(ui->addFieldButton, SIGNAL(clicked()),this, SLOT(slotAddNewField()));
    connect(ui->fieldNameEdit, SIGNAL(editingFinished()), this, SLOT(slotFieldNameEditingFinished()));
    initCategoriesCombo();
    initFieldTypeCombos();

    DQQuery<PartType> query;
    query = query.filter(DQWhere("name")=="Fixed Resistors").orderBy("name");
    if(query.exec()){
        _model = new PartType();
        if(query.next())
            query.recordTo(*_model);
    }    
    _paramsModel->load(_model->id.get());

    setFieldsValues();
}

PartTypeDialog::~PartTypeDialog()
{
    delete _paramsModel;
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

void PartTypeDialog::paramsViewCurrentRowChanged ( const QModelIndex & current, const QModelIndex & previous )
{
    updateButtonsState(current.row());
    if(current.isValid())
        displayParamDetails(current.row());
}

void PartTypeDialog::updateButtonsState(int selectedRow)
{
    int maxRow = ui->fieldListView->model()->rowCount()-1;
    bool validRow = selectedRow>=0 && selectedRow<=maxRow;
    ui->removeFieldButton->setEnabled(validRow);
    ui->fieldUpButton->setEnabled(selectedRow>0);
    ui->fieldDownButton->setEnabled(selectedRow<maxRow);
}

void PartTypeDialog::displayParamDetails(int selectedRow)
{
    PartParameter param = _paramsModel->getParameter(selectedRow);
    ui->fieldNameEdit->setText(param.name);
    ui->fieldDescriptionEdit->setText(param.description);
    int index = ui->fieldCombo->findData(param.type);
    ui->fieldCombo->setCurrentIndex(index);
    ui->fixedValuesCheckBox->setChecked(param.fixedValues);
}

void PartTypeDialog::slotMoveFieldUp()
{
    QModelIndex index = ui->fieldListView->selectionModel()->currentIndex();
    if(!index.isValid())
        return;
    int row = index.row();
    _paramsModel->moveUp(row);
    updateButtonsState(ui->fieldListView->selectionModel()->currentIndex().row());
}

void PartTypeDialog::slotMoveFieldDown()
{
    QModelIndex index = ui->fieldListView->selectionModel()->currentIndex();
    if(!index.isValid())
        return;
    int row = index.row();
    _paramsModel->moveDown(row);
    updateButtonsState(ui->fieldListView->selectionModel()->currentIndex().row());
}


void PartTypeDialog::initFieldTypeCombos()
{
    ui->fieldCombo->addItem(tr("Text"),PartParameter::Text);
    ui->fieldCombo->addItem(tr("Generic Number"),PartParameter::GenericNumber);
    ui->fieldCombo->addItem(tr("Resistance"),PartParameter::Resistance);
    ui->fieldCombo->addItem(tr("Capacitance"),PartParameter::Capacitance);
    ui->fieldCombo->addItem(tr("Inductance"),PartParameter::Inductance);
    ui->fieldCombo->addItem(tr("Power"),PartParameter::Power);
    ui->fieldCombo->addItem(tr("Percentage"),PartParameter::Percentage);
    for(int i =0; i<ui->fieldCombo->count();++i)
    {
        QString text = ui->fieldCombo->itemText(i);
        QVariant userData = ui->fieldCombo->itemData(i);
        ui->partMainFieldCombo->addItem(text, userData);
    }
    ui->fieldCombo->setCurrentIndex(-1);
}

void PartTypeDialog::setFieldsValues()
{
    ui->partNameEdit->setText(_model->name);
    ui->partDescriptionEdit->setText(_model->description);
    int index = ui->partCategoryCombo->findData(_model->category);
    ui->partCategoryCombo->setCurrentIndex(index);
    index = ui->partMainFieldCombo->findData(_model->valueType);
    ui->partMainFieldCombo->setCurrentIndex(index);
}

void PartTypeDialog::slotAddNewField()
{
    PartParameter newParam;
    newParam.name="New Field";
    QModelIndex index = _paramsModel->add(newParam);
    ui->fieldListView->setCurrentIndex(index);
    /*
    ui->fieldNameEdit->setText(newParam.name);
    ui->fieldDescriptionEdit->setText("");
    ui->fieldCombo->setCurrentIndex(-1);
    ui->fixedValuesCheckBox->setChecked(false);
    */
    ui->fieldNameEdit->setFocus();
    ui->fieldNameEdit->selectAll();

}

void PartTypeDialog::slotFieldNameEditingFinished()
{
    QModelIndex index = ui->fieldListView->currentIndex();
    _paramsModel->setData(index,ui->fieldNameEdit->text());
}
