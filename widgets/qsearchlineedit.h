#ifndef QSEARCHLINEEDIT_H
#define QSEARCHLINEEDIT_H

#include <QLineEdit>

class QToolButton;
class QLabel;

namespace Widgets {

class QSearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QSearchLineEdit(QWidget *parent = 0);
    
protected:
    void resizeEvent(QResizeEvent *);
private slots:
    void updateCloseButton(const QString &text);
private:
    QToolButton *clearButton;
    QLabel *imageLabel;
    
};

}

#endif // QSEARCHLINEEDIT_H
