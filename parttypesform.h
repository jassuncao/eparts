#ifndef PARTTYPESFORM_H
#define PARTTYPESFORM_H

#include <QWidget>
#include "parttypetablemodel.h"

namespace Ui {
    class PartTypesForm;
}

class PartTypesForm : public QWidget
{
    Q_OBJECT

public:
    explicit PartTypesForm(QWidget *parent = 0);
    ~PartTypesForm();
public slots:
    void accept();
    void reject();
private slots:
    void currentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
    void addButtonClicked();
    void editButtonClicked();
    void removeButtonClicked();
    void doubleClicked(const QModelIndex & index);
private:
    Ui::PartTypesForm *ui;
    PartTypeTableModel * _model;
};

#endif // PARTTYPESFORM_H
