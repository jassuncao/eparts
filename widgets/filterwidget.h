#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QGridLayout;
class QHBoxLayout;
QT_END_NAMESPACE

class FilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FilterWidget(QWidget *parent = 0);
    
signals:
    
public slots:
private slots:
    void toggleFilterWidget(bool checked);
    void newFilterSelected(int index);
private:
    QCheckBox * m_filterCheckbox;
    QComboBox * m_newFilterComboBox;
    QGridLayout * m_gridLayout;
    QWidget * m_filtersContainer;

};

#endif // FILTERWIDGET_H
