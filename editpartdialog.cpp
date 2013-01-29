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
#include <QSignalMapper>
#include <QDebug>
#include <dquest.h>
#include <database/entities.h>


class AttributeEditorFactory : public PartAttributeVisitor {
public:        

    void visit(const FloatAttribute * attr)
    {
        _widget = new FloatAttributeEditor(attr, _parent);
    }

    void visit(const IntegerAttribute * attr)
    {
        _widget = new IntegerAttributeEditor(attr, _parent);
    }

    void visit(const UnitAttribute * attr)
    {
        _widget = new UnitAttributeEditor(attr, _parent);
    }

    void visit(const PercentageAttribute * attr)
    {
        _widget = new PercentageAttributeEditor(attr, _parent);
    }

    void visit(const TextAttribute * attr)
    {
        _widget = new TextAttributeEditor(attr, _parent);
    }
    static AbstractAttributeEditorWidget* createFor(const AbstractPartAttribute* attribute, QWidget *parent=0) {
        AttributeEditorFactory visitor(parent);
        attribute->accept(visitor);
        return visitor._widget;
    }
private:
    AttributeEditorFactory(QWidget *parent) : _parent(parent) {}
    QWidget * _parent;
    AbstractAttributeEditorWidget* _widget;
};


EditPartDialog::EditPartDialog(AttributesRepository * attributesRepository, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditPartDialog),
    _attributesRepository(attributesRepository),
    _partId(-1)
{
    ui->setupUi(this);    

    FlowLayout *flowLayout = new FlowLayout();
    QPushButton * otherAttributeButton = new QPushButton(tr("<other>"));
    connect(otherAttributeButton,SIGNAL(clicked()),this,SLOT(addOtherAttributeButtonClicked()));
    flowLayout->addWidget(otherAttributeButton);

    _signalMapper = new QSignalMapper(this);
    const AbstractPartAttribute* attr;
    foreach(attr,attributesRepository->listMostUsedAttributes(10)){
        QPushButton * button = new QPushButton(attr->name());
        button->setToolTip(attr->description());
        _signalMapper->setMapping(button, (QObject*)attr);
        connect(button,SIGNAL(clicked()),_signalMapper,SLOT(map()));
        flowLayout->addWidget(button);
    }

    ui->attributesgroupBox->setLayout(flowLayout);
    connect(_signalMapper,SIGNAL(mapped(QObject*)), this, SLOT(attributeButtonClicked(QObject*)));

    QPushButton * cloneButton = new QPushButton(QIcon(":/images/edit-copy_16x16.png"),"Clone");
    ui->buttonBox->addButton(cloneButton,QDialogButtonBox::ActionRole);
}

EditPartDialog::~EditPartDialog()
{
    delete ui;
}



void EditPartDialog::attributeButtonClicked(QObject *object)
{
    AbstractPartAttribute* attr = qobject_cast<AbstractPartAttribute*>(object);
    if(attr){
        qDebug()<<"Attribute button "<<attr->name()<<" pushed";
        addAttributeEditor(attr)->setFocus();
    }
}

void EditPartDialog::removeAttributeClicked(const AbstractPartAttribute *attribute)
{
    int attributeId = attribute->id();
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
    QObject * source = _signalMapper->mapping((QObject*)attribute);
    if(source){
        QPushButton* button = dynamic_cast<QPushButton*>(source);
        if(button)
            button->show();
    }
}

void EditPartDialog::addOtherAttributeButtonClicked()
{
    QList<int> attributesToExclude = _attributeEditors.keys();
    SelectAttributeDialog dlg(_attributesRepository, attributesToExclude, this);   
    if(dlg.exec()){
        AbstractPartAttribute * attr = dlg.getSelectedAttribute();
        if(attr){
            addAttributeEditor(attr)->setFocus();
        }
    }
}

void EditPartDialog::setPart(int partId)
{
    _partId = partId;
    DQPart part;
    if(part.load(DQWhere("id","=",partId)))
    {
        _categoryId = part.category.get().toInt();
        ui->quantitySpinBox_2->setValue(part.quantity.get().toInt());
        ui->minQuantitySpinBox_2->setValue(part.minimumQuantity.get().toInt());
        ui->partNumberLineEdit_2->setText(part.partNumber.get().toString());
        ui->descriptionLineEdit_2->setText(part.description.get().toString());
        QWidget * previousWidget = ui->descriptionLineEdit_2;
        DQQuery<DQFloatValue> floatsQuery;
        floatsQuery = floatsQuery.filter(DQWhere("part","=",partId));
        if(floatsQuery.exec()){
            DQFloatValue floatValue;
            while(floatsQuery.next()){
                floatsQuery.recordTo(floatValue);
                const AbstractPartAttribute* attr = _attributesRepository->findById(floatValue.attribute.get().toInt());
                AbstractAttributeEditorWidget * editor = addAttributeEditor(attr);
                editor->setValue(floatValue.value.get());
                QWidget::setTabOrder(previousWidget,editor);
                previousWidget = editor;
            }
        }
        DQQuery<DQTextValue> textAttrQuery;
        textAttrQuery = textAttrQuery.filter(DQWhere("part","=",partId));
        if(textAttrQuery.exec()){
            DQTextValue textValue;
            while(textAttrQuery.next()){
                textAttrQuery.recordTo(textValue);
                const AbstractPartAttribute* attr = _attributesRepository->findById(textValue.attribute.get().toInt());
                AbstractAttributeEditorWidget * editor = addAttributeEditor(attr);
                editor->setValue(textValue.value.get());
                QWidget::setTabOrder(previousWidget,editor);
                previousWidget = editor;
            }
        }
        QWidget::setTabOrder(previousWidget,ui->buttonBox);
    }
    else{
        //TODO: SHow some error
    }
}

void EditPartDialog::setCategory(int categoryId)
{
    _categoryId = categoryId;
}

void EditPartDialog::done(int r)
{
    if(QDialog::Accepted == r){
        savePart();
    }
    QDialog::done(r);
}


AbstractAttributeEditorWidget *EditPartDialog::addAttributeEditor(const AbstractPartAttribute* attribute)
{
    AbstractAttributeEditorWidget* attributeEditor = AttributeEditorFactory::createFor(attribute);
    connect(attributeEditor,SIGNAL(removeAttributeClicked(const AbstractPartAttribute*)),this,SLOT(removeAttributeClicked(const AbstractPartAttribute*)));
    int idx = ui->formLayout_2->rowCount();
    ui->formLayout_2->insertRow(idx, attribute->name(),attributeEditor);
    _attributeEditors[attribute->id()]=attributeEditor;
    QObject * source = _signalMapper->mapping((QObject *)attribute);
    if(source){
        QPushButton* button = dynamic_cast<QPushButton*>(source);
        if(button)
            button->hide();
    }    
    return attributeEditor;
}

void EditPartDialog::savePart()
{
    DQPart part;
    if(_partId!=-1){
        part.id.set(_partId);
    }
    part.category.set(_categoryId);
    part.description.set(ui->descriptionLineEdit_2->text());
    part.minimumQuantity.set(ui->minQuantitySpinBox_2->value());
    part.quantity.set(ui->quantitySpinBox_2->value());
    part.partNumber.set(ui->partNumberLineEdit_2->text());    
    if(part.save()) {
        _partId = part.id.get().toInt();
        DQQuery<DQTextValue> textAttrQuery;
        DQQuery<DQFloatValue> floatsQuery;
        textAttrQuery.filter(DQWhere("part","=",_partId)).remove();
        floatsQuery.filter(DQWhere("part","=",_partId)).remove();
        QWidget* widget;
        foreach(widget, _attributeEditors.values()){
            AbstractAttributeEditorWidget * editor = dynamic_cast<AbstractAttributeEditorWidget*>(widget);
            if(editor->attribute()->isText()){
                DQTextValue textValue;
                textValue.value.set(editor->value());
                textValue.part.set(part.id);
                textValue.attribute.set(editor->attribute()->id());
                textValue.save();
            }
            else{
                DQFloatValue floatValue;
                floatValue.value.set(editor->value());
                floatValue.part.set(part.id);
                floatValue.attribute.set(editor->attribute()->id());
                floatValue.save();
            }
        }
    }
    else{
        //TODO: Show some error
    }

}

