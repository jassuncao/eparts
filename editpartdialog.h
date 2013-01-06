#ifndef EDITPARTDIALOG_H
#define EDITPARTDIALOG_H

#include <QDialog>
#include "database/entities.h"
#include <dqlist.h>
#include <QMap>

namespace Ui {
class EditPartDialog;
}

class QSignalMapper;

class EditPartDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditPartDialog(const DQList<DQAttribute> & mostUsedAttributes, QWidget *parent = 0);
    ~EditPartDialog();
private slots:
    void attributeButtonClicked(int attributeId);    
    void removeAttributeClicked(int attributeId);
    void addOtherAttributeButtonClicked();
private:
    void addAttribute(int attributeId);
    Ui::EditPartDialog *ui;
    QSignalMapper * _signalMapper;
    const DQList<DQAttribute> & _mostUsedAttributes;
    QMap<int,QWidget*> _attributeEditors;
    //QMap<int,QWidget*> _attributeButtons;
};

#endif // EDITPARTDIALOG_H
