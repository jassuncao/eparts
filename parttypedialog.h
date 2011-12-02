#ifndef PARTTYPEDIALOG_H
#define PARTTYPEDIALOG_H

#include <QDialog>
#include "parttype.h"

namespace Ui {
    class PartTypeDialog;
}

class PartTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PartTypeDialog(QWidget *parent = 0);
    ~PartTypeDialog();

private:
    Ui::PartTypeDialog *ui;
    PartType * _model;
    void initCategoriesCombo();
    void initFieldTypeCombos();
    void setFieldsValues();
};

#endif // PARTTYPEDIALOG_H
