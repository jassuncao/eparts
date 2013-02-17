#ifndef ATTRIBUTEFILTERROW_H
#define ATTRIBUTEFILTERROW_H

#include <QObject>
#include <models/partattribute.h>

using namespace Models;

class QGridLayout;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QLabel;

class AttributeFilterRow : public QObject
{
    Q_OBJECT
public:
    enum FilterOperator{
        FILTER_OP_EQUAL,
        FILTER_OP_NOT_EQUAL,
        FILTER_OP_LESS_EQUAL_THAN,
        FILTER_OP_GREATER_EQUAL_THAN,
        FILTER_OP_BETWEEN,
        FILTER_OP_CONTAINS,
        FILTER_OP_NOT_CONTAINS
    };

    explicit AttributeFilterRow(const AbstractPartAttribute *attr, QGridLayout *layout, int row, int tag=-1, QObject * parent=0);
    ~AttributeFilterRow();
    int row() const;
    inline int tag(){return m_tag;}
signals:
    void filterRemoved(const AbstractPartAttribute *attribute);
public slots:
    void disableFilter();
    void enableFilter();
private slots:
    void toggledCheckbox(bool checked);
    void opChanged(int index);
private:
    static QComboBox * createNumericOpCombo(QWidget * parent=0);
    static QComboBox * createTextOpCombo(QWidget * parent=0);

    QGridLayout *m_layout;
    QCheckBox *m_checkbox;
    QComboBox *m_opCombo;
    QLineEdit *m_arg1Edit;
    QLabel *m_andLabel;
    QLineEdit *m_arg2Edit;

    int m_selectedOp;
    const AbstractPartAttribute * m_attribute;
    const int m_tag;
    
};

#endif // ATTRIBUTEFILTERROW_H
