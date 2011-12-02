#ifndef CATEGORIESFORM_H
#define CATEGORIESFORM_H

#include <QWidget>
//#include <QModelIndex>

class QModelIndex;
class CategoriesListModel;

namespace Ui {
    class CategoriesForm;
}

class CategoriesForm : public QWidget
{
    Q_OBJECT

public:
    explicit CategoriesForm(QWidget *parent = 0);
    ~CategoriesForm();
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
    Ui::CategoriesForm *ui;
    CategoriesListModel * _model;

};

#endif // CATEGORIESFORM_H
