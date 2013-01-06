#ifndef QLINEEDIT2_H
#define QLINEEDIT2_H

#include <QLineEdit>

class QLineEdit2 : public QLineEdit
{
    Q_OBJECT
public:
    explicit QLineEdit2(QWidget *parent = 0);    
signals:
    void focus();
protected:
    void focusInEvent(QFocusEvent * e);
    void focusOutEvent(QFocusEvent * e);
    double _value;
    
};

#endif // QLINEEDIT2_H
