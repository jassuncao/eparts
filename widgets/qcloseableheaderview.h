#ifndef QCLOSEABLEHEADERVIEW_H
#define QCLOSEABLEHEADERVIEW_H

#include <QHeaderView>

class QCloseableHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit QCloseableHeaderView(Qt::Orientation orientation, QWidget *parent = 0);
protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    
public slots:
private:
};

#endif // QCLOSEABLEHEADERVIEW_H
