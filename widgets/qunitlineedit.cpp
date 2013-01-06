#include "qunitlineedit.h"
#include <QLabel>
#include <QStyle>
#include <QtGui>

using namespace Widgets;

QUnitLineEdit::QUnitLineEdit(QChar unit, QWidget *parent) :
    QLineEdit(parent), _unit(unit)
{
    QLocale locale;
    QChar decimalPoint = locale.decimalPoint();
    QString rgex("\\b[0-9]+(\\%1[0-9]+)?[k,M,G,T,P,E,Z,Y,m,u,n,p,f,a,z,y]?\\b");
    QFontMetrics fm(font());
    int padding = fm.width(_unit);
    setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(padding));
    QRegExp rx(rgex.arg(decimalPoint));
    QValidator *validator = new QRegExpValidator(rx, this);
    setAlignment(Qt::AlignTrailing);
    setValidator(validator);
}

void QUnitLineEdit::paintEvent ( QPaintEvent * ev)
{
    QLineEdit::paintEvent(ev);
    QPainter p(this);    
    int padding = p.fontMetrics().width(_unit)-4;
    QStyleOptionFrame opt;
    initStyleOption(&opt);
    QRect cr = style()->subElementRect(QStyle::SE_LineEditContents, &opt, this);
    //cr.setLeft(cr.left()+2);
    cr.setRight((cr.right() + padding));
    cr.setBottom(cr.bottom()+1);
    p.drawText(cr, Qt::AlignTrailing|Qt::AlignVCenter, _unit);    
}
