#ifndef SELECTATTRIBUTEDIALOG_H
#define SELECTATTRIBUTEDIALOG_H

#include <QDialog>
#include <QList>

namespace Ui {
class SelectAttributeDialog;
}

class QStandardItemModel;
class QSortFilterProxyModel;
class QTimer;
class QModelIndex;

class SelectAttributeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectAttributeDialog(const QList<int> attributesToFilter, QWidget *parent = 0);
    ~SelectAttributeDialog();
    int getSelectedAttribute() const;
private slots:
    void slotSearchLineChange( const QString& newText );
    void slotSearchLineActivate();
    void slotClicked(const QModelIndex &index);
    void slotCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void newAttribute();
private:
    void initAttributesTree(const QList<int> & attributesToFilter);
    Ui::SelectAttributeDialog *ui;
    QStandardItemModel *_sourceModel;
    QSortFilterProxyModel *_proxyModel;
    QTimer* _timer;
    int _selectedAttribute;
};

#endif // SELECTATTRIBUTEDIALOG_H
