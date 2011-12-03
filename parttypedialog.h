#ifndef PARTTYPEDIALOG_H
#define PARTTYPEDIALOG_H

#include <QDialog>
#include "parttype.h"
#include "models/partparameterslistmodel.h"

namespace Ui {
    class PartTypeDialog;
}

class PartTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PartTypeDialog(QWidget *parent = 0);
    ~PartTypeDialog();

private slots:
    void paramsViewCurrentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
    void slotMoveFieldUp();
    void slotMoveFieldDown();
    void slotAddNewField();
    void slotFieldNameEditingFinished();
private:
    Ui::PartTypeDialog *ui;
    PartType * _model;
    PartParametersListModel * _paramsModel;
    void initCategoriesCombo();
    void initFieldTypeCombos();
    void setFieldsValues();
    void updateButtonsState(int selectedRow);
    void displayParamDetails(int selectedRow);

};

#endif // PARTTYPEDIALOG_H
