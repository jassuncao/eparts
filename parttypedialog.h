#ifndef PARTTYPEDIALOG_H
#define PARTTYPEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QItemDelegate>
#include "parttype.h"
#include "models/partparameterslistmodel.h"
#include "models/partparameterstablemodel.h"
#include "models/categorieslistmodel.h"

namespace Ui {
    class PartTypeDialog;
}

class ParamTypeDelegate;

class PartTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PartTypeDialog(QWidget *parent = 0);
    ~PartTypeDialog();
    void setModel(PartType * partType);
    PartType * model() const;
private slots:
    void paramsViewCurrentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
    void slotMoveFieldUp();
    void slotMoveFieldDown();
    void slotAddNewField();
    void slotRemoveField();    

public slots:
    void accept();
private:
    Ui::PartTypeDialog *ui;
    PartType * _model;
    PartParametersTableModel _paramsModel;    
    QDataWidgetMapper _fieldsMapper;
    QStringList _typeModel;    
    void initCategoriesCombo();
    void initFieldTypeCombos();
    void setFieldsValues();
    void updateButtonsState(int selectedRow);    

};

class ParamTypeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ParamTypeDelegate(QObject *parent = 0);
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // PARTTYPEDIALOG_H
