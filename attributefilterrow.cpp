#include "attributefilterrow.h"
#include <QtGui>
#include <models/partattribute.h>

AttributeFilterRow::AttributeFilterRow(const AbstractPartAttribute *attr, QGridLayout *layout, int row, int tag, QObject *parent) :
    QObject(parent),
    m_layout(layout),
    m_attribute(attr),
    m_tag(tag)
{
    m_checkbox = new QCheckBox(attr->name());
    m_checkbox->setChecked(true);
    layout->addWidget(m_checkbox,row,0);
    connect(m_checkbox,SIGNAL(toggled(bool)),this,SLOT(toggledCheckbox(bool)));

    if(attr->isText())
        m_opCombo = createTextOpCombo();
    else
        m_opCombo = createNumericOpCombo();
    m_opCombo->setCurrentIndex(0);
    m_selectedOp = FILTER_OP_EQUAL;
    m_arg1Edit = new QLineEdit();
    m_arg2Edit = new QLineEdit();
    m_andLabel = new QLabel("and");
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

AttributeFilterRow::~AttributeFilterRow()
{
    delete m_checkbox;
    delete m_opCombo;
    delete m_arg1Edit;
    delete m_andLabel;
    delete m_arg2Edit;
}

int AttributeFilterRow::row() const
{
    int idx = m_layout->indexOf(m_checkbox);
    int row, column, rowSpan, columnSpan;
    m_layout->getItemPosition(idx,&row,&column,&rowSpan,&columnSpan);
    return row;
}

void AttributeFilterRow::disableFilter()
{
    m_checkbox->setChecked(false);
}

void AttributeFilterRow::enableFilter()
{
    m_checkbox->setChecked(true);
    m_arg1Edit->setFocus();
}

void AttributeFilterRow::toggledCheckbox(bool checked)
{
    if(!checked)
        emit filterRemoved(m_attribute);   
    /*
    m_checkbox->setVisible(checked);
    m_opCombo->setVisible(checked);
    m_arg1Edit->setVisible(checked);
    m_andLabel->setVisible(checked);
    m_arg2Edit->setVisible(checked);
    */    
}

void AttributeFilterRow::opChanged(int index)
{
    if(index<0) return;
    QVariant itemData = m_opCombo->itemData(index);
    int op = itemData.toInt();
    if(m_selectedOp==op) return;
    if(m_selectedOp==FILTER_OP_BETWEEN){
        //we need to hide the second argument widget
        m_arg2Edit->hide();
        m_andLabel->hide();
        m_arg2Edit->clear();
    }
    if(op==FILTER_OP_BETWEEN){
        //we need to show the widget for the second argument
        m_arg2Edit->show();
        m_andLabel->show();
    }
    m_selectedOp = op;
}

QComboBox * AttributeFilterRow::createNumericOpCombo(QWidget *parent)
{
    QComboBox * comboBox = new QComboBox(parent);
    comboBox->addItem(tr("is"),FILTER_OP_EQUAL);
    comboBox->addItem(tr("is"),FILTER_OP_NOT_EQUAL);
    comboBox->addItem(tr(">="),FILTER_OP_GREATER_EQUAL_THAN);
    comboBox->addItem(tr("<="),FILTER_OP_LESS_EQUAL_THAN);
    comboBox->addItem(tr("between"),FILTER_OP_BETWEEN);
    return comboBox;
}

QComboBox * AttributeFilterRow::createTextOpCombo(QWidget *parent)
{
    QComboBox * comboBox = new QComboBox(parent);
    comboBox->addItem(tr("is"),FILTER_OP_EQUAL);
    comboBox->addItem(tr("contains"),FILTER_OP_CONTAINS);
    comboBox->addItem(tr("doesn't contains"),FILTER_OP_NOT_CONTAINS);
    return comboBox;
}
