#include "qlineedit2.h"
#include <QMouseEvent>
#include <unitformatter.h>
#include <unitparser.h>

QLineEdit2::QLineEdit2(QWidget *parent) :
    QLineEdit(parent),_value(120)
{
}

void QLineEdit2::focusInEvent(QFocusEvent *e)
{
    setText(UnitFormatter::format(_value,0));
    QLineEdit::focusInEvent(e);
    //emit focus();
}

void QLineEdit2::focusOutEvent(QFocusEvent *e)
{
    _value=UnitParser::parseUnit(text());
    setText(UnitFormatter::format(_value,QChar('F')));
    QLineEdit::focusOutEvent(e);
}
