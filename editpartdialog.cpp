#include "editpartdialog.h"
#include "ui_editpartdialog.h"
#include <QPushButton>
#include "widgets/flowlayout.h"
#include "widgets/attributeeditorwidget.h"
#include "models/partattribute.h"
#include <QDesktopWidget>
#include "widgets/qlineedit2.h"
#include "selectattributedialog.h"
#include <QList>


EditPartDialog::EditPartDialog(const DQList<DQAttribute> & mostUsedAttributes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditPartDialog),
    _mostUsedAttributes(mostUsedAttributes)
{
    ui->setupUi(this);    

    FlowLayout *flowLayout = new FlowLayout();
    QPushButton * otherAttributeButton = new QPushButton(tr("<other>"));
    connect(otherAttributeButton,SIGNAL(clicked()),this,SLOT(addOtherAttributeButtonClicked()));
    flowLayout->addWidget(otherAttributeButton);

    _signalMapper = new QSignalMapper(this);
    for(int i=0; i<mostUsedAttributes.size();++i) {
        DQAttribute * attr = mostUsedAttributes.at(i);
        QPushButton * button = new QPushButton(attr->name.get().toString());
        button->setToolTip(attr->description.get().toString());
        int attributeId = attr->id.get().toInt();
        _signalMapper->setMapping(button, attributeId);
        connect(button,SIGNAL(clicked()),_signalMapper,SLOT(map()));
        flowLayout->addWidget(button);
    }
    ui->attributesframe->setLayout(flowLayout);
    connect(_signalMapper,SIGNAL(mapped(int)), this, SLOT(attributeButtonClicked(int)));
}

EditPartDialog::~EditPartDialog()
{
    delete ui;
}

void EditPartDialog::attributeButtonClicked(int attributeId)
{
    qDebug()<<"Attribute button "<<attributeId<<" pushed";
    addAttribute(attributeId);
}

void EditPartDialog::removeAttributeClicked(int attributeId)
{
    QWidget * editor = _attributeEditors[attributeId];
    QWidget * editorLabel = ui->formLayout_2->labelForField(editor);
    int idx = ui->formLayout_2->indexOf(editor);
    if(idx>=0){
        QLayoutItem * item = ui->formLayout_2->takeAt(idx);
        delete item->widget();
        delete item;
    }
    idx = ui->formLayout_2->indexOf(editorLabel);
    if(idx>=0){
        QLayoutItem * item = ui->formLayout_2->takeAt(idx);
        delete item->widget();
        delete item;
    }
    _attributeEditors.remove(attributeId);
    QObject * source = _signalMapper->mapping(attributeId);
    if(source){
        QPushButton* button = dynamic_cast<QPushButton*>(source);
        if(button)
            button->show();
    }
}

void EditPartDialog::addOtherAttributeButtonClicked()
{
    QList<int> keys = _attributeEditors.keys();
    SelectAttributeDialog dlg(keys, this);
    if(dlg.exec()){
        int attrId = dlg.getSelectedAttribute();
        addAttribute(attrId);
    }
}

void EditPartDialog::addAttribute(int attributeId)
{
    DQAttribute attr;
    DQQuery query;
    query = query.filter(DQWhere("id","=",QVariant(attributeId)));
    if(query.exec() && query.next()){
        query.recordTo(attr);
    }
    AbstractAttributeEditorWidget * attributeEditor;
    switch(attr.type.get().toInt())
    {
    case Models::ATTRIBUTE_TEXT:
        break;
    case Models::ATTRIBUTE_RESISTANCE:
        break;
    }

    UnitAttributeEditor  * attributeEditor = new UnitAttributeEditor(attributeId,'F');
    connect(attributeEditor,SIGNAL(removeAttributeClicked(int)),this,SLOT(removeAttributeClicked(int)));
    int idx = ui->formLayout_2->rowCount();
    ui->formLayout_2->insertRow(idx, QString(attr.name.get().toString()),attributeEditor);
    _attributeEditors[attributeId]=attributeEditor;
    QObject * source = _signalMapper->mapping(attributeId);
    if(source){
        QPushButton* button = dynamic_cast<QPushButton*>(source);
        if(button)
            button->hide();
    }
}

