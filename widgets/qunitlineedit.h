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
    explicit QUnitLineEdit(const QString & unit, QWidget *parent = 0);

protected:
    //void resizeEvent(QResizeEvent *);
    void paintEvent ( QPaintEvent * );
private:
    const QString _unit;
signals:

public slots:

};

}

#endif // QUNITLINEEDIT_H
