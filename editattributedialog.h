#ifndef EDITATTRIBUTEDIALOG_H
#define EDITATTRIBUTEDIALOG_H

#include <QDialog>

namespace Ui {
class EditAttributeDialog;
}

namespace Models {
class PartAttribute;
}

class EditAttributeDialog : public QDialog
{
    Q_OBJECT
private slots:
    void validateInput();
public:
    explicit EditAttributeDialog(QWidget *parent = 0);
    ~EditAttributeDialog();
    QString attributeName() const;
    QString attributeDescription() const;
    int attributeType() const;
    void setAttributeName(const QString &name);
    void setAttributeDescription(const QString &description);
    void setAttributeType(int type);
private:
    void initTypesCombo();
    Ui::EditAttributeDialog *ui;
};

#endif // EDITATTRIBUTEDIALOG_H
