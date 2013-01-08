#ifndef SELECTATTRIBUTEDIALOG_H
#define SELECTATTRIBUTEDIALOG_H

#include <QDialog>
#include <QList>
#include <models/partattribute.h>

using namespace Models;

namespace Ui {
class SelectAttributeDialog;
}

namespace Models {
class AttributesRepository;
}

class QStandardItemModel;
class QSortFilterProxyModel;
class QTimer;
class QModelIndex;

class SelectAttributeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectAttributeDialog(AttributesRepository * attributesRepository, const QList<int> & attributesToExclude, QWidget *parent = 0);
    ~SelectAttributeDialog();
    AbstractPartAttribute * getSelectedAttribute() const;
    //void setAttributes(const QList<AbstractPartAttribute*> &attributes, const QList<int> &attributesToExclude);
private slots:
    void slotSearchLineChange( const QString& newText );
    void slotSearchLineActivate();
    void slotCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void newAttribute();
private:
    void initAttributesModel(const QList<int> &attributesToExclude);
    Ui::SelectAttributeDialog *ui;
    QStandardItemModel *_sourceModel;
    QSortFilterProxyModel *_proxyModel;
    QTimer* _timer;
    AbstractPartAttribute * _selectedAttribute;
    AttributesRepository * _attributesRepository;
};

#endif // SELECTATTRIBUTEDIALOG_H
