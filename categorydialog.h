#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>
#include "database/category.h"

namespace Ui {
    class CategoryDialog;
}

class CategoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryDialog(QWidget *parent = 0);
    ~CategoryDialog();
    Category * model() const;
    void setModel(Category * model);
public slots:
    virtual void accept();
private:
    Ui::CategoryDialog *ui;
    Category *_model;
};

#endif // CATEGORYDIALOG_H
