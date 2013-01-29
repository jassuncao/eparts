#ifndef EDITPARTDIALOG_H
#define EDITPARTDIALOG_H

#include <QDialog>
#include <models/partattribute.h>
#include <models/attributesrepository.h>
#include <QMap>
#include <QList>

namespace Ui {
class EditPartDialog;
}

class QSignalMapper;
using namespace Models;

class AbstractAttributeEditorWidget;

class EditPartDialog : public QDialog
{
    Q_OBJECT    
public:
    explicit EditPartDialog(AttributesRepository * attributesRepository, QWidget *parent = 0);
    ~EditPartDialog();
    void setPart(int partId);
    inline int part() const {return _partId;}
    void setCategory(int categoryId);
    void done(int r);
private slots:
    void attributeButtonClicked(QObject * object);
    void removeAttributeClicked(const AbstractPartAttribute* attribute);
    void addOtherAttributeButtonClicked();
private:    
    AbstractAttributeEditorWidget* addAttributeEditor(const AbstractPartAttribute *attribute);
    void savePart();

    Ui::EditPartDialog *ui;
    QSignalMapper * _signalMapper;
    QMap<int,QWidget*> _attributeEditors;
    AttributesRepository * _attributesRepository;
    int _partId;
    int _categoryId;
};

#endif // EDITPARTDIALOG_H
