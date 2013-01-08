#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QDoubleValidator>
#include <QSpinBox>
#include "qunitlineedit.h"
#include "attributeeditorwidget.h"
#include "models/partattribute.h"
#include <unitparser.h>
#include <unitformatter.h>

static const QChar SYMBOL_RESISTANCE(0x2126);
static const QChar SYMBOL_CAPACITANCE('F');
static const QChar SYMBOL_INDUCTANCE('H');
static const QChar SYMBOL_POWER('W');
static const QChar SYMBOL_PERCENTAGE('%');

/*

AttributeEditorWidget::AttributeEditorWidget(int attributeId, Models::AttributeType attributeType, QWidget *parent) :
    _attributeId(attributeId),
    _attributeType(attributeType),
    QWidget(parent)
{
    QHBoxLayout * hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->setSpacing(0);
    hBoxLayout->setMargin(0);

    switch(attributeType)
    {
        case Models::ATTRIBUTE_GENERIC_NUMBER:
            _lineEdit = new QLineEdit(this);
            _lineEdit->setValidator(new QDoubleValidator(this));
            break;
        case Models::ATTRIBUTE_RESISTANCE:
            _lineEdit = new Widgets::QUnitLineEdit(SYMBOL_RESISTANCE,this);
            break;
        case Models::ATTRIBUTE_CAPACITANCE:
            _lineEdit = new Widgets::QUnitLineEdit(SYMBOL_CAPACITANCE,this);
            break;
        case Models::ATTRIBUTE_INDUCTANCE:
            _lineEdit = new Widgets::QUnitLineEdit(SYMBOL_INDUCTANCE,this);
            break;
        case Models::ATTRIBUTE_POWER:
            _lineEdit = new Widgets::QUnitLineEdit(SYMBOL_POWER,this);
            break;
        case Models::ATTRIBUTE_PERCENTAGE:
            _lineEdit = new QLineEdit(this);
            break;
        case Models::ATTRIBUTE_TEXT:
            _lineEdit = new QLineEdit(this);
            break;
    }    
    hBoxLayout->addWidget(_lineEdit);
    if(Models::PartAttribute::isTextAttribute(attributeType)==false){
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(_lineEdit->sizePolicy().hasHeightForWidth());
        _lineEdit->setSizePolicy(sizePolicy1);
        hBoxLayout->addSpacerItem(new  QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    }
    QToolButton * remButton = new QToolButton(this);
    QIcon icon(QString::fromUtf8(":/images/edit_clear_16x16.png"));    
    remButton->setIcon(icon);
    hBoxLayout->addWidget(remButton);
    setLayout(hBoxLayout);
    connect(remButton,SIGNAL(clicked()),this,SLOT(removeButtonClicked()));
    connect(_lineEdit,SIGNAL(editingFinished()),this,SLOT(prettyPrint()));
}

QVariant AttributeEditorWidget::value() const
{

}

QVariant AttributeEditorWidget::setValue(QVariant value)
{
}

void AttributeEditorWidget::removeButtonClicked()
{
    emit removeAttributeClicked(_attributeId);
}

void AttributeEditorWidget::prettyPrint()
{
    bool ok;
    double value = UnitParser::parseUnit(_lineEdit->text(),&ok);
    if(ok){
        QString text = UnitFormatter::format(value);
        _lineEdit->setText(text);
    }
}

*/
AbstractAttributeEditorWidget::AbstractAttributeEditorWidget(const AbstractPartAttribute * attribute, QWidget *parent) :
    QWidget(parent),
    _attribute(attribute)
{
    _boxLayout = new QHBoxLayout(this);
    _boxLayout->setSpacing(0);
    _boxLayout->setMargin(0);
    QToolButton * remButton = new QToolButton(this);
    QIcon icon(QString::fromUtf8(":/images/edit_clear_16x16.png"));
    remButton->setIcon(icon);
    _boxLayout->addWidget(remButton);
    setLayout(_boxLayout);
    connect(remButton,SIGNAL(clicked()),this,SLOT(removeButtonClicked()));
}

void AbstractAttributeEditorWidget::removeButtonClicked()
{
    emit removeAttributeClicked(_attribute);
}

TextAttributeEditor::TextAttributeEditor(const TextAttribute *attribute, QWidget *parent) :
    AbstractAttributeEditorWidget(attribute,parent)
{
    _lineEdit = new QLineEdit(this);
    boxLayout()->insertWidget(0,_lineEdit);
}

QVariant TextAttributeEditor::value() const
{
    return QVariant(_lineEdit->text());
}

void TextAttributeEditor::setValue(QVariant value)
{
    _lineEdit->setText(value.toString());
}

PercentageAttributeEditor::PercentageAttributeEditor(const PercentageAttribute *attribute, QWidget *parent) :
    AbstractAttributeEditorWidget(attribute,parent)
{
    _spinbox = new QDoubleSpinBox(this);
    _spinbox->setMinimum(0.01);
    _spinbox->setMaximum(100);
    _spinbox->setValue(0);
    _spinbox->setSuffix("%");
    _spinbox->setDecimals(2);
    boxLayout()->insertWidget(0,_spinbox);    
    boxLayout()->insertSpacerItem(1, new  QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

QVariant PercentageAttributeEditor::value() const
{
    return QVariant(_spinbox->value());
}

void PercentageAttributeEditor::setValue(QVariant value)
{
    double d = value.toDouble();
    if(d>=0.01 || d<100)
        _spinbox->setValue(d);
}

FloatAttributeEditor::FloatAttributeEditor(const FloatAttribute *attribute, QWidget *parent) :
     AbstractAttributeEditorWidget(attribute,parent)
{
    _lineEdit = new QLineEdit(this);
    _lineEdit->setValidator(new QDoubleValidator(this));
    boxLayout()->insertWidget(0,_lineEdit);
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(_lineEdit->sizePolicy().hasHeightForWidth());
    _lineEdit->setSizePolicy(sizePolicy1);
    boxLayout()->insertSpacerItem(1, new  QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

QVariant FloatAttributeEditor::value() const
{
    bool ok;
    double d = _lineEdit->text().toDouble(&ok);
    if(ok)
        return QVariant(d);
    return QVariant();
}

void FloatAttributeEditor::setValue(QVariant value)
{
    bool ok;
    double d = value.toDouble(&ok);
    if(ok)
        _lineEdit->setText(QString::number(d));
}

IntegerAttributeEditor::IntegerAttributeEditor(const IntegerAttribute *attribute, QWidget *parent) :
     AbstractAttributeEditorWidget(attribute,parent)
{
    _lineEdit = new QLineEdit(this);
    _lineEdit->setValidator(new QIntValidator(this));
    boxLayout()->insertWidget(0,_lineEdit);
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(_lineEdit->sizePolicy().hasHeightForWidth());
    _lineEdit->setSizePolicy(sizePolicy1);
    boxLayout()->insertSpacerItem(1, new  QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

QVariant IntegerAttributeEditor::value() const
{
    bool ok;
    int i = _lineEdit->text().toInt(&ok);
    if(ok)
        return QVariant(i);
    return QVariant();
}

void IntegerAttributeEditor::setValue(QVariant value)
{
    bool ok;
    double i = value.toInt(&ok);
    if(ok)
        _lineEdit->setText(QString::number(i));
}

UnitAttributeEditor::UnitAttributeEditor(const UnitAttribute *attribute, QWidget *parent) :
    AbstractAttributeEditorWidget(attribute,parent)
{
    _lineEdit = new Widgets::QUnitLineEdit(attribute->unitSymbol(),this);
    boxLayout()->insertWidget(0,_lineEdit);
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(_lineEdit->sizePolicy().hasHeightForWidth());
    _lineEdit->setSizePolicy(sizePolicy1);
    boxLayout()->insertSpacerItem(1, new  QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    connect(_lineEdit,SIGNAL(editingFinished()),this,SLOT(prettyPrint()));
}

QVariant UnitAttributeEditor::value() const
{
    bool ok;
    double d = UnitParser::parseUnit(_lineEdit->text(),&ok);
    if(ok)
        return QVariant(d);
    return QVariant();
}

void UnitAttributeEditor::setValue(QVariant value)
{
    bool ok;
    double d = value.toDouble(&ok);
    if(ok)
        _lineEdit->setText(UnitFormatter::format(d));
}

void UnitAttributeEditor::prettyPrint()
{
    bool ok;
    QString text = _lineEdit->text();
    double value = UnitParser::parseUnit(text,&ok);
    if(ok){
        QString text = UnitFormatter::format(value);
        _lineEdit->setText(text);
    }
}
