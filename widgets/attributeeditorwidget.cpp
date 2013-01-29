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
    _remButton = new QToolButton(this);
    QIcon icon(QString::fromUtf8(":/images/edit_clear_16x16.png"));
    _remButton->setIcon(icon);
    _boxLayout->addWidget(_remButton);
    setLayout(_boxLayout);
    connect(_remButton,SIGNAL(clicked()),this,SLOT(removeButtonClicked()));
}

void AbstractAttributeEditorWidget::removeButtonClicked()
{
    emit removeAttributeClicked(_attribute);
}

void AbstractAttributeEditorWidget::valueChanged()
{
    bool oldValue = _modified;
    _modified = checkModified();
    if(_modified && !oldValue){
        emit modified(_attribute);
    }
}

TextAttributeEditor::TextAttributeEditor(const TextAttribute *attribute, QWidget *parent) :
    AbstractAttributeEditorWidget(attribute,parent)
{
    _lineEdit = new QLineEdit(this);
    boxLayout()->insertWidget(0,_lineEdit);
    if(_lineEdit->focusPolicy() != Qt::NoFocus) {
        setFocusProxy(_lineEdit);
    }
    connect(_lineEdit,SIGNAL(textEdited(QString)),this,SLOT(valueChanged()));
    setTabOrder(_lineEdit,_remButton);
}

QVariant TextAttributeEditor::value() const
{
    return QVariant(_lineEdit->text());
}

void TextAttributeEditor::setValue(QVariant value)
{
    _originalValue = value.toString();
    _lineEdit->setText(_originalValue);
}

bool TextAttributeEditor::checkModified() const
{
    return _originalValue!=_lineEdit->text();
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
    if(_spinbox->focusPolicy() != Qt::NoFocus) {
        setFocusProxy(_spinbox);
    }
    connect(_spinbox,SIGNAL(valueChanged(double)), this, SLOT(valueChanged()));
    setTabOrder(_spinbox,_remButton);
}

QVariant PercentageAttributeEditor::value() const
{
    return QVariant(_spinbox->value());
}

void PercentageAttributeEditor::setValue(QVariant value)
{
    _originalValue = value.toDouble();
    if(_originalValue>=0.01 || _originalValue<100)
        _spinbox->setValue(_originalValue);
}

bool PercentageAttributeEditor::checkModified() const
{
    return _originalValue!=_spinbox->value();
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
    if(_lineEdit->focusPolicy() != Qt::NoFocus) {
        setFocusProxy(_lineEdit);
    }
    connect(_lineEdit,SIGNAL(textEdited(QString)),this,SLOT(valueChanged()));
    setTabOrder(_lineEdit,_remButton);
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
    if(ok){
        _originalValue = QString::number(d);
        _lineEdit->setText(_originalValue);
    }
}

bool FloatAttributeEditor::checkModified() const
{
    return _originalValue!=_lineEdit->text();
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
    if(_lineEdit->focusPolicy() != Qt::NoFocus) {
        setFocusProxy(_lineEdit);
    }
    connect(_lineEdit,SIGNAL(textEdited(QString)),this,SLOT(valueChanged()));
    setTabOrder(_lineEdit,_remButton);
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
    if(ok){
        _originalValue = QString::number(i);
        _lineEdit->setText(_originalValue);
    }
}

bool IntegerAttributeEditor::checkModified() const
{
    return _originalValue!=_lineEdit->text();
}

UnitAttributeEditor::UnitAttributeEditor(const UnitAttribute *attribute, QWidget *parent) :
    AbstractAttributeEditorWidget(attribute,parent)
{
    _lineEdit = new Widgets::QUnitLineEdit(attribute->unitSymbol(),this);
    if(_lineEdit->focusPolicy() != Qt::NoFocus) {
        setFocusProxy(_lineEdit);
    }
    boxLayout()->insertWidget(0,_lineEdit);
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(_lineEdit->sizePolicy().hasHeightForWidth());
    _lineEdit->setSizePolicy(sizePolicy1);
    boxLayout()->insertSpacerItem(1, new  QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    connect(_lineEdit,SIGNAL(editingFinished()),this,SLOT(prettyPrint()));
    connect(_lineEdit,SIGNAL(textEdited(QString)),this,SLOT(valueChanged()));
}

QVariant UnitAttributeEditor::value() const
{
    bool ok;
    QString text = _lineEdit->text();
    qDebug()<<"Text to parse:"<<text;
    double d = UnitParser::parseUnit(text,&ok);
    qDebug()<<"Parse result:"<<d;
    if(ok)
        return QVariant(d);
    return QVariant();
}

void UnitAttributeEditor::setValue(QVariant value)
{
    bool ok;
    double d = value.toDouble(&ok);
    if(ok){
        _originalValue = UnitFormatter::format(d);
        _lineEdit->setText(_originalValue);
    }
}

void UnitAttributeEditor::prettyPrint()
{
    bool ok;
    QString text = _lineEdit->text();
    double value = UnitParser::parseUnit(text,&ok);
    if(ok){
        _lineEdit->setText(UnitFormatter::format(value));
    }
}

bool UnitAttributeEditor::checkModified() const
{
    return _originalValue!=_lineEdit->text();
}
