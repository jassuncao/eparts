#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "categoriesform.h"
#include "parttypesform.h"

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
public slots:
    virtual void accept();
    virtual void reject();
private:
    Ui::SettingsDialog *ui;
    CategoriesForm * _categoriesForm;
    PartTypesForm * _partForm;
};

#endif // SETTINGSDIALOG_H
