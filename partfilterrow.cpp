#include "partfilterrow.h"
#include "constants.h"
#include <QtGui>


PartFilterRow::PartFilterRow(const QString &label, int type, QGridLayout *layout, int tag, QObject *parent) :
    QObject(parent),
    m_layout(layout),
    m_tag(tag)
{
    int row = layout->rowCount();
    m_checkbox = new QCheckBox(label);
    m_checkbox->setChecked(true);
    layout->addWidget(m_checkbox,row,0);
    connect(m_checkbox,SIGNAL(toggled(bool)),this,SLOT(toggledCheckbox(bool)));

    if(type==EParts::ATTRIBUTE_TEXT)
        m_opCombo = createTextOpCombo();
    else
        m_opCombo = createNumericOpCombo();
    m_opCombo->setCurrentIndex(0);
    m_selectedOperand = EParts::FILTER_OP_EQUAL;
    m_arg1Edit = new QLineEdit();
    m_arg2Edit = new QLineEdit();
    m_andLabel = new QLabel(tr("and"));
    m_arg2Edit->hide();
    m_andLabel->hide();
    QHBoxLayout * boxLayout = new QHBoxLayout;
    boxLayout->setMargin(0);
    boxLayout->addWidget(m_arg1Edit);
    boxLayout->addWidget(m_andLabel);
    boxLayout->addWidget(m_arg2Edit);


    layout->addWidget(m_opCombo,row,1);
    layout->addLayout(boxLayout,row,2);
    connect(m_opCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(opChanged(int)));
}

PartFilterRow::~PartFilterRow()
{
    delete m_checkbox;
    delete m_opCombo;
    delete m_arg1Edit;
    delete m_andLabel;
    delete m_arg2Edit;
}

int PartFilterRow::row() const
{
    int idx = m_layout->indexOf(m_checkbox);
    int row, column, rowSpan, columnSpan;
    m_layout->getItemPosition(idx,&row,&column,&rowSpan,&columnSpan);
    return row;
}

/*
void PartFilterRow::disableFilter()
{
    m_checkbox->setChecked(false);
}

void PartFilterRow::enableFilter()
{
    m_checkbox->setChecked(true);
    m_arg1Edit->setFocus();
}
*/

void PartFilterRow::toggledCheckbox(bool checked)
{
    if(!checked)
        emit filterRemoved(m_tag);
}

void PartFilterRow::opChanged(int index)
{
    if(index<0) return;
    QVariant itemData = m_opCombo->itemData(index);
    int op = itemData.toInt();
    if(m_selectedOperand==op) return;
    if(m_selectedOperand==EParts::FILTER_OP_BETWEEN){
        //we need to hide the second argument widget
        m_arg2Edit->hide();
        m_andLabel->hide();
        m_arg2Edit->clear();
    }
    if(op==EParts::FILTER_OP_BETWEEN){
        //we need to show the widget for the second argument
        m_arg2Edit->show();
        m_andLabel->show();
    }
    m_selectedOperand = op;
}

QComboBox * PartFilterRow::createNumericOpCombo(QWidget *parent)
{
    QComboBox * comboBox = new QComboBox(parent);
    comboBox->addItem(tr("is"),EParts::FILTER_OP_EQUAL);
    comboBox->addItem(tr("is"),EParts::FILTER_OP_NOT_EQUAL);
    comboBox->addItem(tr(">="),EParts::FILTER_OP_GREATER_EQUAL_THAN);
    comboBox->addItem(tr("<="),EParts::FILTER_OP_LESS_EQUAL_THAN);
    comboBox->addItem(tr("between"),EParts::FILTER_OP_BETWEEN);
    return comboBox;
}

QComboBox * PartFilterRow::createTextOpCombo(QWidget *parent)
{
    QComboBox * comboBox = new QComboBox(parent);
    comboBox->addItem(tr("is"),EParts::FILTER_OP_EQUAL);
    comboBox->addItem(tr("contains"),EParts::FILTER_OP_CONTAINS);
    comboBox->addItem(tr("doesn't contains"),EParts::FILTER_OP_NOT_CONTAINS);
    return comboBox;
}


void PartFilterRow::focus()
{
    m_arg1Edit->setFocus();
}
