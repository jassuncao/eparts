#ifndef PARTFILTERROW_H
#define PARTFILTERROW_H

#include <QObject>
class QGridLayout;
class QCheckBox;
class QComboBox;
class QLineEdit;
class QLabel;

class PartFilterRow : public QObject
{
    Q_OBJECT
public:
    explicit PartFilterRow(const QString &label, int type, QGridLayout * layout, int tag, QObject *parent = 0);
    ~PartFilterRow();
    int row() const;
signals:
    void filterRemoved(int tag);
public slots:
    void focus();
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

    int m_selectedOperand;
    const int m_tag;
};

#endif // PARTFILTERROW_H
