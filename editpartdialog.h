#ifndef EDITPARTDIALOG_H
#define EDITPARTDIALOG_H

#include <QDialog>
//#include "database/entities.h"
//#include <dqlist.h>
#include <models/partattribute.h>
#include <models/attributesrepository.h>
#include <QMap>
#include <QList>

namespace Ui {
class EditPartDialog;
}

class QSignalMapper;
using namespace Models;

class EditPartDialog : public QDialog
{
    Q_OBJECT    
public:
    explicit EditPartDialog(AttributesRepository * attributesRepository, QWidget *parent = 0);
    ~EditPartDialog();
private slots:
    void attributeButtonClicked(QObject * object);
    void removeAttributeClicked(const AbstractPartAttribute* attribute);
    void addOtherAttributeButtonClicked();
private:
    void addAttributeEditor(AbstractPartAttribute* attribute);
    Ui::EditPartDialog *ui;
    QSignalMapper * _signalMapper;
    //const QList<AbstractPartAttribute*> & _mostUsedAttributes;
    QMap<int,QWidget*> _attributeEditors;
    //QMap<int,QWidget*> _attributeButtons;
    AttributesRepository * _attributesRepository;
};

#endif // EDITPARTDIALOG_H
