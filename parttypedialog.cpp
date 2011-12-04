#include "parttypedialog.h"
#include "ui_parttypedialog.h"
#include "category.h"
#include "partparameter.h"
#include "models/partparameterslistmodel.h"

PartTypeDialog::PartTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PartTypeDialog)//,
    //_paramsModel(0,4)
{
    ui->setupUi(this);
    ui->addFieldButton->setEnabled(true);
    ui->removeFieldButton->setEnabled(false);
    ui->fieldUpButton->setEnabled(false);
    ui->fieldDownButton->setEnabled(false);

    //_paramsModel = new PartParametersListModel();
    QListView * fieldListView = ui->fieldListView;
    fieldListView->setModel(&_paramsModel);
    _fieldsMapper.setModel(&_paramsModel);
    _fieldsMapper.setItemDelegate(new ParamTypeDelegate(this));
    _fieldsMapper.addMapping(ui->fieldNameEdit,0);
    _fieldsMapper.addMapping(ui->fieldCombo,1);
    _fieldsMapper.addMapping(ui->fieldDescriptionEdit,2);
    _fieldsMapper.addMapping(ui->fixedValuesCheckBox,3);

    connect(fieldListView->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(paramsViewCurrentRowChanged(QModelIndex,QModelIndex)));
    connect(ui->fieldUpButton,SIGNAL(clicked()), this, SLOT(slotMoveFieldUp()));
    connect(ui->fieldDownButton,SIGNAL(clicked()), this, SLOT(slotMoveFieldDown()));
    connect(ui->addFieldButton, SIGNAL(clicked()),this, SLOT(slotAddNewField()));
    connect(ui->removeFieldButton, SIGNAL(clicked()), this, SLOT(slotRemoveField()));

    initCategoriesCombo();
    initFieldTypeCombos();

    DQQuery<PartType> query;
    query = query.filter(DQWhere("name")=="Fixed Resistors").orderBy("name");
    if(query.exec()){
        _model = new PartType();
        if(query.next())
            query.recordTo(*_model);
    }
    /*
    QStringList columnNames;
    columnNames<<tr("Name")<<tr("Type")<<tr("Description")<<tr("Fixed values");
    _paramsModel.setHorizontalHeaderLabels(columnNames);
    */
    //_paramsModel->load(_model->id.get());

    /*
    DQQuery<PartParameter> query2;
    query2 = query2.filter(DQWhere("partType")==_model->id.get()).orderBy("orderIndex");
    if(query2.exec()){
        PartParameter param;
        QList<QStandardItem*> items;
        while(query2.next()){
            query2.recordTo(param);

            QStandardItem * item = new QStandardItem(param.name);
            item->setData(param.id);
            items<<item;

            item = new QStandardItem();
            item->setData(param.type);
            items<<item;

            items<<new QStandardItem(param.description);
            items<<new QStandardItem(param.fixedValues);

            _paramsModel.appendRow(items);
            items.clear();
        }
    }
    */
    _paramsModel.load(_model->id.get());
    setFieldsValues();
}

PartTypeDialog::~PartTypeDialog()
{
    //delete _paramsModel;
    delete _model;
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
        displayParamDetails(current);
}

void PartTypeDialog::updateButtonsState(int selectedRow)
{
    int maxRow = ui->fieldListView->model()->rowCount()-1;
    bool validRow = selectedRow>=0 && selectedRow<=maxRow;
    ui->removeFieldButton->setEnabled(validRow);
    ui->fieldUpButton->setEnabled(selectedRow>0);
    ui->fieldDownButton->setEnabled(selectedRow<maxRow);
    ui->fieldNameEdit->setEnabled(validRow);
    ui->fieldDescriptionEdit->setEnabled(validRow);
    ui->fieldCombo->setEnabled(validRow);
}

void PartTypeDialog::displayParamDetails(const QModelIndex &index)
{
    _fieldsMapper.setCurrentModelIndex(index);
    /*
    PartParameter param = _paramsModel->getParameter(selectedRow);
    ui->fieldNameEdit->setText(param.name);
    ui->fieldDescriptionEdit->setText(param.description);
    int index = ui->fieldCombo->findData(param.type);
    ui->fieldCombo->setCurrentIndex(index);
    ui->fixedValuesCheckBox->setChecked(param.fixedValues);
    */
}

void PartTypeDialog::slotMoveFieldUp()
{
    QModelIndex index = ui->fieldListView->selectionModel()->currentIndex();
    if(!index.isValid())
        return;
    int row = index.row();
    if(row==0) return;
    _paramsModel.moveUp(row);

    //QList<QStandardItem *> rowItems = _paramsModel.takeRow(row);
    //_paramsModel.insertRow(row-1,rowItems);
    index = _paramsModel.index(row-1,0);
    //ui->fieldListView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    ui->fieldListView->setCurrentIndex( index);

    //_paramsModel->moveUp(row);
    updateButtonsState(ui->fieldListView->selectionModel()->currentIndex().row());
}

void PartTypeDialog::slotMoveFieldDown()
{
    QModelIndex index = ui->fieldListView->selectionModel()->currentIndex();
    if(!index.isValid())
        return;
    int row = index.row();
    if(row>=_paramsModel.rowCount()-1) return;
    _paramsModel.moveDown(row);
    //QList<QStandardItem *> rowItems = _paramsModel.takeRow(row);
    //_paramsModel.insertRow(row+1,rowItems);
    index = _paramsModel.index(row+1,0);
    //ui->fieldListView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::SelectCurrent);
    ui->fieldListView->setCurrentIndex( index);
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
    /*
    PartParameter newParam;
    newParam.name="New Field";
    //QModelIndex index = _paramsModel->add(newParam);
    //ui->fieldListView->setCurrentIndex(index);
    /*
    ui->fieldNameEdit->setText(newParam.name);
    ui->fieldDescriptionEdit->setText("");
    ui->fieldCombo->setCurrentIndex(-1);
    ui->fixedValuesCheckBox->setChecked(false);
    */

    //QStandardItem * newItem = new QStandardItem("New Field");
    //newItem->setData(-1);//Set id as undefined
    //_paramsModel.appendRow(newItem);
    //QModelIndex index = _paramsModel.indexFromItem(newItem);
    PartParameter newParam;
    newParam.name ="New Field";
    newParam.type = -1;
    newParam.id = -1;
    QModelIndex index = _paramsModel.appendRow(newParam);


    ui->fieldListView->setCurrentIndex(index);
    _fieldsMapper.setCurrentModelIndex(index);
    ui->fieldNameEdit->setFocus();
    ui->fieldNameEdit->selectAll();

}

void PartTypeDialog::slotRemoveField()
{
    QModelIndex index = ui->fieldListView->currentIndex();
    if(!index.isValid())
        return;
    _paramsModel.removeRow(index.row());
    /*
    QList<QStandardItem *> rowItems = _paramsModel.takeRow(index.row());
    QStandardItem * item;
    int paramId = rowItems[0]->data().toInt();
    if(paramId>=0)
        _removedParams.append(paramId);
    foreach(item, rowItems){
        delete item;
    }
    rowItems.clear();
    */
    index = ui->fieldListView->currentIndex();
    updateButtonsState(index.row());
    _fieldsMapper.setCurrentModelIndex(index);
    _fieldsMapper.revert();
}

void PartTypeDialog::slotFieldNameEditingFinished()
{
    QModelIndex index = ui->fieldListView->currentIndex();
    //_paramsModel->setData(index,ui->fieldNameEdit->text());
}

void PartTypeDialog::accept()
{
    qDebug()<<"Accept";
    _model->name = ui->partNameEdit->text();
    _model->description = ui->partDescriptionEdit->text();
    int index = ui->partCategoryCombo->currentIndex();
    if(index>0)
        _model->category = ui->partCategoryCombo->itemData(index);
    index = ui->partMainFieldCombo->currentIndex();
    if(index>0)
        _model->valueType = ui->partMainFieldCombo->itemData(index);
    _paramsModel.saveChanges();
    /*
    int removedParamId;
    DQQuery<PartParameter> query;
    foreach(removedParamId, _removedParams){
        query = query.filter(DQWhere("id")==removedParamId);
        query.remove();
    }
    int rows = _paramsModel.rowCount();
    while(rows>0){
        _paramsModel.takeRow(0);
    }
    */
    QDialog::accept();
}


ParamTypeDelegate::ParamTypeDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

void ParamTypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
    if(comboBox){
        int comboIndex = comboBox->findData(index.data());
        comboBox->setCurrentIndex(comboIndex);
        return;
    }
    QItemDelegate::setEditorData(editor, index);
}

void ParamTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
    if(comboBox){
        int comboIndex = comboBox->currentIndex();
        if(comboIndex>=0){
            QVariant value  = comboBox->itemData(comboIndex);
            model->setData(index, value);
        }
        return;
    }
    QItemDelegate::setModelData(editor, model, index);
}
