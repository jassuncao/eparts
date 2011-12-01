#ifndef PARTTYPEDIALOG_H
#define PARTTYPEDIALOG_H

#include <QDialog>

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
    void initCategoriesCombo();
    void initFieldTypeCombos();
};

#endif // PARTTYPEDIALOG_H
