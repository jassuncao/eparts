#include "filterwidget.h"
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QTimer>

const char * FILTER_CHECK_BOX_STYLE = "QCheckBox::indicator { width: 18px; height: 18px;}"
        "QCheckBox::indicator:checked { image: url(:/images/collapse.png);}"
        "QCheckBox::indicator:unchecked{ image: url(:/images/expand.png);}";

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent)
{
    m_filterCheckbox = new QCheckBox(tr("Filters"),this);
    m_filterCheckbox->setStyleSheet(FILTER_CHECK_BOX_STYLE);

    m_filtersContainer = new QWidget(this);
    m_newFilterComboBox = new QComboBox(m_filtersContainer);
    QLabel * newFilterLabel = new QLabel(tr("Add filter"),m_filtersContainer);
    newFilterLabel->setBuddy(m_newFilterComboBox);
    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->addWidget(newFilterLabel);
    hlayout->addWidget(m_newFilterComboBox);
    QVBoxLayout * newFilterLayout = new QVBoxLayout;
    newFilterLayout->addLayout(hlayout);
    newFilterLayout->addSpacerItem(new QSpacerItem(20,4));//,QSizePolicy::Minimum, QSizePolicy::Minimum));
    //newFilterLayout->addStretch(0);

    m_gridLayout = new QGridLayout;

    QHBoxLayout * m_mainLayout = new QHBoxLayout;
    m_mainLayout->addLayout(m_gridLayout);
    m_mainLayout->addStretch();
    m_mainLayout->addLayout(newFilterLayout);
    m_filtersContainer->setLayout(m_mainLayout);

    QVBoxLayout * mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(m_filterCheckbox);
    QFrame * hFrame = new QFrame();
    hFrame->setFrameStyle(QFrame::HLine);
    mainVLayout->addWidget(hFrame);
    mainVLayout->addWidget(m_filtersContainer);
    setLayout(mainVLayout);

    connect(m_newFilterComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(newFilterSelected(int)));
    connect(m_filterCheckbox,SIGNAL(toggled(bool)), this, SLOT(toggleFilterWidget(bool)));

    QCheckBox * item1Check = new QCheckBox("Item 1",m_filtersContainer);
    QComboBox * item1Combo = new QComboBox(m_filtersContainer);
    QLineEdit * item1Edit = new QLineEdit(m_filtersContainer);
    m_gridLayout->addWidget(item1Check,0,0);
    m_gridLayout->addWidget(item1Combo,0,1);
    m_gridLayout->addWidget(item1Edit,0,2);

    QCheckBox * item2Check = new QCheckBox("Item 2",m_filtersContainer);
    QComboBox * item2Combo = new QComboBox(m_filtersContainer);
    QLineEdit * item2Edit = new QLineEdit(m_filtersContainer);
    m_gridLayout->addWidget(item2Check,1,0);
    m_gridLayout->addWidget(item2Combo,1,1);
    m_gridLayout->addWidget(item2Edit,1,2);
    m_filtersContainer->setVisible(false);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
}

void FilterWidget::toggleFilterWidget(bool checked)
{
    m_filtersContainer->setVisible(checked);
}

void FilterWidget::newFilterSelected(int index)
{
}
