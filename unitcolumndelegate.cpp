#include "unitcolumndelegate.h"
#include "widgets/qunitlineedit.h"
#include "unitformatter.h"
#include "unitparser.h"
#include <qpainter.h>
#include <QDoubleSpinBox>

using namespace Widgets;

UnitColumnDelegate::UnitColumnDelegate(PartParameter::ParameterType paramType, QObject *parent) :
    QStyledItemDelegate(parent), _paramType(paramType)
{
    _unit = UnitFormatter::getUnitSymbol(_paramType);
}

QString UnitColumnDelegate::displayText ( const QVariant & value, const QLocale & locale ) const
{
    return UnitFormatter::format(value.toDouble(), _unit);
}

/*
void UnitColumnDelegate::initStyleOption ( QStyleOptionViewItem * option, const QModelIndex & index ) const
{
    option->displayAlignment=Qt::AlignRight | Qt::AlignVCenter;
    QStyledItemDelegate::initStyleOption(option, index);
}
*/

QWidget *UnitColumnDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QUnitLineEdit *editor = new QUnitLineEdit(_unit,parent);
    return editor;
}

void UnitColumnDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.data(Qt::EditRole).toDouble();
    QUnitLineEdit *lineEdit = static_cast<QUnitLineEdit*>(editor);
    QString text = UnitFormatter::format(value);
    lineEdit->setText(text);
}

void UnitColumnDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QUnitLineEdit *lineEdit = static_cast<QUnitLineEdit*>(editor);
    QString text = lineEdit->text();
    bool ok;
    double value = UnitParser::parseUnit(text,&ok);
    if(ok)
        model->setData(index, value, Qt::EditRole);

}

void UnitColumnDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


/*****************************************************************************/


DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QChar unit, double singleStep, QObject *parent)
    : QStyledItemDelegate(parent), _unit(unit), _singleStep(singleStep)
{
}

QString DoubleSpinBoxDelegate::displayText ( const QVariant & value, const QLocale & locale) const
{
    /*
    QString result;
    QTextStream out(&result);
    out.setRealNumberPrecision(2);
    out.setRealNumberNotation(QTextStream::SmartNotation);
    out<<value;
    if(unit!=0){
        out<<unit;
    }
    return result;*/
    return UnitFormatter::formatPercentage(value.toDouble(), _unit);
}

QWidget * DoubleSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
    editor->setSuffix(QString(_unit));
    editor->setDecimals(2);
    editor->setSingleStep(_singleStep);
    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.data(Qt::EditRole).toDouble();
    QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(editor);
    doubleSpinBox->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox* doubleSpinBox = static_cast<QDoubleSpinBox*>(editor);
    double value = doubleSpinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

/*****************************************************************************/

/*
PowerColumnDelegate::PowerColumnDelegate(QObject *parent)
    :DoubleSpinBoxDelegate(UnitFormatter::getUnitSymbol(PartParameter::Power),1/8f, parent)
{
}
*/
