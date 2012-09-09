#ifndef QUNITLINEEDIT_H
#define QUNITLINEEDIT_H

#include <QLineEdit>


class QLabel;
class QChar;

namespace Widgets {

class QUnitLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QUnitLineEdit(QChar unit, QWidget *parent = 0);

protected:
    //void resizeEvent(QResizeEvent *);
    void paintEvent ( QPaintEvent * );
private:
    QChar _unit;
    //QLabel * _unitLabel;
signals:

public slots:

};

}

#endif // QUNITLINEEDIT_H
