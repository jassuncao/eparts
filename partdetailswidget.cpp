#include "partdetailswidget.h"
#include "partmodel.h"
#include "unitformatter.h"
#include "float.h"
#include <QtGui/QLayoutIterator>
const double GENERIC_NUMBER_MIN= -DBL_MAX;
const double GENERIC_NUMBER_MAX = DBL_MAX;

static QPlainTextEdit * createFixedRowsTextEdit(int visibleRows, QWidget *parent=0)
{
    QPlainTextEdit * editor = new QPlainTextEdit(parent);
    QFontMetrics m(editor->font());
    int rowHeight = m.lineSpacing();
    editor->setFixedHeight(visibleRows * rowHeight+2);
    editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    return editor;
}

static QWidget * createFieldEditor(PartParameter::ParameterType paramType, QWidget *parent)
{
    switch(paramType){
    case PartParameter::Text:
        return new QLineEdit(parent);
    case PartParameter::LongText:
        return createFixedRowsTextEdit(4, parent);
    case PartParameter::GenericNumber:
    {
        QDoubleSpinBox * spinBox = new QDoubleSpinBox(parent);
        spinBox->setRange(GENERIC_NUMBER_MIN, GENERIC_NUMBER_MAX);
        return spinBox;
    }
    case PartParameter::Resistance:
    case PartParameter::Capacitance:
    case PartParameter::Inductance:
    case PartParameter::Power:
    case PartParameter::Percentage:
    {
        QUnitLineEdit * editor = new QUnitLineEdit(UnitFormatter::getUnitSymbol(paramType), parent);
        editor->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        return editor;
    }
    default:
        //TODO: Show error;
        return NULL;
    }
}

static void clearLayout(QLayout *l) {
    QLayoutItem *child;
    while ((child = l->takeAt(0)) != 0) {
        if (child->layout()) {
                clearLayout((QLayout *)child->layout());
        }
        delete child;
    }
}

PartDetailsWidget::PartDetailsWidget(QWidget *parent ) :
    QWidget(parent), _formLayout(0)
{      
    /*
    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(notesTextEdit->sizePolicy().hasHeightForWidth());
    notesTextEdit->setSizePolicy(sizePolicy3);
    formLayout->addRow(notesLabel, notesTextEdit);
    */
    _dataMapper = new QDataWidgetMapper(this);

}

void PartDetailsWidget::retranslateUi(QWidget *widget)
{    
    quantityLabel->setText(QApplication::translate("PartDetailsWidget", "Quantity:", 0, QApplication::UnicodeUTF8));
    minimumQuantityLabel->setText(QApplication::translate("PartDetailsWidget", "Minimum Quantity:", 0, QApplication::UnicodeUTF8));
}

/*
static void setFieldValue(QWidget * editor, QString value){
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
    if(lineEdit){
        lineEdit->setText(value);
    }
    else {
        QPlainTextEdit * textEdit = qobject_cast<QPlainTextEdit*>(editor);
        if(textEdit){
            textEdit->setPlainText(value);
        }
    }
}

void PartDetailsWidget::setData(PartRow * data)
{
    if(data==NULL){
        qDebug()<<"Data is null";
    }
    else{

       quantitySpinBox->setValue(data->part.quantity);
       minimumQuantitySpinBox->setValue(data->part.minimumQuantity);      
       const QList<PartParameter>  * parameters = _partModel->parameters();
       QList<PartParameter>::const_iterator it;
       for(it = parameters->constBegin();it!=parameters->constEnd();++it)
       {
           int id = it->id.get().toInt();
           ParameterValue paramValue = data->paramValues[id];
           QWidget * paramEditor = _params[id];
           PartParameter::ParameterType valueType = (PartParameter::ParameterType)it->type.get().toUInt();
           QString valueText;
           if(valueType==PartParameter::Text)
           {
               valueText = paramValue.textValue;

           }
           else{
               valueText = UnitFormatter::format(paramValue.numericValue);
           }
           setFieldValue(paramEditor,valueText);
       }
    }
}
*/

void PartDetailsWidget::createWidgets(){
    if(_formLayout){
        clearLayout(_formLayout);
        _dataMapper->clearMapping();
    }
    else{
        _formLayout = new QFormLayout(this);
    }
    int fieldIndex = 0;
    quantityLabel = new QLabel(this);
    quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setMaximum(65535);
    quantitySpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    _formLayout->addRow(quantityLabel, quantitySpinBox);
    _dataMapper->addMapping(quantitySpinBox,fieldIndex++);

    minimumQuantityLabel = new QLabel(this);
    minimumQuantitySpinBox = new QSpinBox(this);
    minimumQuantitySpinBox->setMaximum(65535);
    minimumQuantitySpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    _formLayout->addRow(minimumQuantityLabel, minimumQuantitySpinBox);
    _dataMapper->addMapping(minimumQuantitySpinBox,fieldIndex++);

    const QStringList & fieldNames = _partTypeModel->fields();
    for(;fieldIndex<fieldNames.count();++fieldIndex)
    {

        PartParameter::ParameterType fieldType = _partTypeModel->fieldType(fieldIndex);
        QString fieldName = fieldNames[fieldIndex];

        qDebug()<<"Creating field "<<fieldName;
        QLabel * fieldLabel = new QLabel(fieldName+":", this);
        QWidget * fieldEditor = createFieldEditor(fieldType, this);
        _formLayout->addRow(fieldLabel, fieldEditor);
        _dataMapper->addMapping(fieldEditor,fieldIndex);
    }
    retranslateUi(this);
}

void PartDetailsWidget::setModel(const PartTypeModel * partTypeModel, QAbstractItemModel * model)
{
    _partTypeModel = partTypeModel;    
    _dataMapper->setModel(model);
    createWidgets();
    _dataMapper->toFirst();
}

void PartDetailsWidget::setCurrentModelIndex(const QModelIndex & index)
{
    _dataMapper->setCurrentModelIndex(index);
}
