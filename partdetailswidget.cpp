#include "partdetailswidget.h"
#include "partmodel.h"
#include "unitformatter.h"

static QLineEdit * createLineEdit(PartParameter::ParameterType paramType, QWidget *parent)
{
    if(paramType==PartParameter::Text)
    {
        return new QLineEdit(parent);
    }
    else{
        return new QUnitLineEdit(UnitFormatter::getUnitSymbol(paramType), parent);
    }
}

PartDetailsWidget::PartDetailsWidget(PartModel *partModel, QWidget *parent ) :
    _partModel(partModel), QWidget(parent)
{    
    formLayout = new QFormLayout(this);

    valueLabel = new QLabel(this);
    valueField = createLineEdit(_partModel->valueType(), this);
    valueField->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    formLayout->addRow(valueLabel, valueField);


    //Create a row of widgets for each parameter
    const QList<PartParameter>  * parameters = _partModel->parameters();
    for (int i = 0; i < parameters->size(); ++i) {
        PartParameter param = parameters->at(i);
        PartParameter::ParameterType valueType = (PartParameter::ParameterType)param.type.get().toUInt();

        QLabel * fieldLabel = new QLabel(param.name+":");
        QLineEdit * fieldValue = createLineEdit(valueType, this);
        formLayout->addRow(fieldLabel, fieldValue);
        _params[param.id.get().toInt()]=fieldValue;
    }

    quantityLabel = new QLabel(this);
    quantitySpinBox = new QSpinBox(this);
    quantitySpinBox->setMaximum(65535);
    quantitySpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    formLayout->addRow(quantityLabel, quantitySpinBox);

    minimumQuantityLabel = new QLabel(this);
    minimumQuantitySpinBox = new QSpinBox(this);
    minimumQuantitySpinBox->setMaximum(65535);
    minimumQuantitySpinBox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    formLayout->addRow(minimumQuantityLabel, minimumQuantitySpinBox);

    notesLabel = new QLabel(this);
    notesTextEdit = new QTextEdit(this);
    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(notesTextEdit->sizePolicy().hasHeightForWidth());
    notesTextEdit->setSizePolicy(sizePolicy3);
    formLayout->addRow(notesLabel, notesTextEdit);

    retranslateUi(this);
}

void PartDetailsWidget::retranslateUi(QWidget *widget)
{
    valueLabel->setText(QApplication::translate("PartDetailsWidget", "Value:", 0, QApplication::UnicodeUTF8));
    quantityLabel->setText(QApplication::translate("PartDetailsWidget", "Quantity:", 0, QApplication::UnicodeUTF8));
    minimumQuantityLabel->setText(QApplication::translate("PartDetailsWidget", "Minimum Quantity:", 0, QApplication::UnicodeUTF8));
    notesLabel->setText(QApplication::translate("PartDetailsWidget", "Notes:", 0, QApplication::UnicodeUTF8));
}

void PartDetailsWidget::setData(PartRow * data)
{
    if(data==NULL){
        qDebug()<<"Data is null";
    }
    else{

       quantitySpinBox->setValue(data->part.quantity);
       minimumQuantitySpinBox->setValue(data->part.minimumQuantity);
       PartParameter::ParameterType type = _partModel->valueType();
       QString valueText;
          if(type==PartParameter::Text){
              valueText = data->part.textValue;
          }
          else{
              valueText = UnitFormatter::format(data->part.numericValue);
          }
       valueField->setText(valueText);
       const QList<PartParameter>  * parameters = _partModel->parameters();
       QList<PartParameter>::const_iterator it;
       for(it = parameters->constBegin();it!=parameters->constEnd();++it)
       {
           int id = it->id.get().toInt();
           ParameterValue paramValue = data->paramValues[id];
           QLineEdit * paramEditor = _params[id];
           PartParameter::ParameterType valueType = (PartParameter::ParameterType)it->type.get().toUInt();
           if(valueType==PartParameter::Text)
           {
               paramEditor->setText(paramValue.textValue);
           }
           else{
               paramEditor->setText(UnitFormatter::format(paramValue.numericValue));
           }
       }
    }
}
