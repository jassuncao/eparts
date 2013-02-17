#include "qcloseableheaderview.h"
#include <QtDebug>

#include <QtGui/QMouseEvent>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionViewItem>

QCloseableHeaderView::QCloseableHeaderView(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(orientation, parent)
{
}

void QCloseableHeaderView::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug()<<"Context menu ";
    QHeaderView::contextMenuEvent(event);
}

