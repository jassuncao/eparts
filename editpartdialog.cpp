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


class AttributeEditorFactory : public PartAttributeVisitor {
public:        

    void visit(FloatAttribute * attr)
    {
        _widget = new FloatAttributeEditor(attr, _parent);
    }

    void visit(IntegerAttribute * attr)
    {
        _widget = new IntegerAttributeEditor(attr, _parent);
    }

    void visit(UnitAttribute * attr)
    {
        _widget = new UnitAttributeEditor(attr, _parent);
    }

    void visit(PercentageAttribute * attr)
    {
        _widget = new PercentageAttributeEditor(attr, _parent);
    }

    void visit(TextAttribute * attr)
    {
        _widget = new TextAttributeEditor(attr, _parent);
    }
    static AbstractAttributeEditorWidget* createFor(AbstractPartAttribute* attribute, QWidget *parent=0) {
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
    _attributesRepository(attributesRepository)
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

    ui->attributesframe->setLayout(flowLayout);
    connect(_signalMapper,SIGNAL(mapped(QObject*)), this, SLOT(attributeButtonClicked(QObject*)));
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
        addAttributeEditor(attr);
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
            addAttributeEditor(attr);
        }
    }
}

void EditPartDialog::addAttributeEditor(AbstractPartAttribute* attribute)
{
    AbstractAttributeEditorWidget* attributeEditor = AttributeEditorFactory::createFor(attribute);
    connect(attributeEditor,SIGNAL(removeAttributeClicked(const AbstractPartAttribute*)),this,SLOT(removeAttributeClicked(const AbstractPartAttribute*)));
    int idx = ui->formLayout_2->rowCount();
    ui->formLayout_2->insertRow(idx, attribute->name(),attributeEditor);
    _attributeEditors[attribute->id()]=attributeEditor;
    QObject * source = _signalMapper->mapping(attribute);
    if(source){
        QPushButton* button = dynamic_cast<QPushButton*>(source);
        if(button)
            button->hide();
    }
    attributeEditor->setFocus();
}

