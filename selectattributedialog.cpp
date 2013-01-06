#include "selectattributedialog.h"
#include "ui_selectattributedialog.h"
#include <dquest.h>
#include <database/entities.h>
#include <models/partattribute.h>

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QShortcut>
#include "editattributedialog.h"

using Models::PartAttribute;

SelectAttributeDialog::SelectAttributeDialog(const QList<int> attributesToFilter, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectAttributeDialog),
    _selectedAttribute(-1)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    _timer = new QTimer(this);
    _timer->setSingleShot(true);
    initAttributesTree(attributesToFilter);
    QShortcut * shortcut = new QShortcut(QKeySequence(QKeySequence::Find),this);
    connect(shortcut,SIGNAL( activated() ), ui->lineEdit, SLOT( setFocus() ) );
    connect( _timer, SIGNAL( timeout() ), this, SLOT( slotSearchLineActivate() ) );
    connect( ui->lineEdit, SIGNAL( textChanged( const QString& ) ), SLOT( slotSearchLineChange( const QString& ) ) );
}

SelectAttributeDialog::~SelectAttributeDialog()
{
    delete ui;
}

int SelectAttributeDialog::getSelectedAttribute() const
{
    return _selectedAttribute;
}

void SelectAttributeDialog::slotSearchLineChange(const QString &newText)
{
    _timer->start( 300 );
}

void SelectAttributeDialog::slotSearchLineActivate()
{
    if ( !_proxyModel )
        return;
    _proxyModel->setFilterKeyColumn(0);
    _proxyModel->setFilterCaseSensitivity( Qt::CaseInsensitive );
    _proxyModel->setFilterFixedString( ui->lineEdit->text() );
}

void SelectAttributeDialog::slotClicked(const QModelIndex &index)
{
    if(!_sourceModel || !_proxyModel)
        return;
    QStandardItem *item = _sourceModel->itemFromIndex(_proxyModel->mapToSource(index));
    if(item) {
        qDebug()<<"Item selected:"<<item->data();
        _selectedAttribute = item->data().toInt();
    }
    else{
        _selectedAttribute=-1;
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(_selectedAttribute>=0);
}

void SelectAttributeDialog::slotCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    QStandardItem *item = _sourceModel->itemFromIndex(_proxyModel->mapToSource(current));
    if(item) {
        qDebug()<<"Item selected:"<<item->data();
        _selectedAttribute = item->data().toInt();
    }
    else{
        _selectedAttribute=-1;
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(_selectedAttribute>=0);
}

void SelectAttributeDialog::newAttribute()
{
    qDebug()<<"New attribute";
    EditAttributeDialog dlg(this);
    if(dlg.exec()){
        DQAttribute newAttr;
        newAttr.name.set(dlg.attributeName());
        newAttr.description.set(dlg.attributeDescription());
        newAttr.type.set(dlg.attributeType());
        qDebug()<<"Saving new attribute";
        if(newAttr.save()){
            _selectedAttribute = newAttr.id.get().toInt();
        }
        else{
            qErrnoWarning("Unable to save new attribute");
            //TODO: SHow some error msg
        }
    }
}

void SelectAttributeDialog::initAttributesTree(const QList<int> &attributesToFilter)
{
    QStandardItemModel * model = new QStandardItemModel(this);
    model->setColumnCount(2);
    QStringList columnNames;
    columnNames<<tr("Attribute")<<tr("Description");
    model->setHorizontalHeaderLabels(columnNames);
    DQQuery<DQAttribute> query;    
    if(query.exec()){
        DQAttribute attr;
        while(query.next()){
            query.recordTo(attr);
            int attributeId = attr.id.get().toInt();
            if(attributesToFilter.contains(attributeId))
                continue;
            QStandardItem * nameItem = new QStandardItem(attr.name.get().toString());
            nameItem->setData(attr.id.get());
            nameItem->setEditable(false);
            QStandardItem * descriptionItem = new QStandardItem(attr.description.get().toString());
            descriptionItem->setEditable(false);
            descriptionItem->setData(attr.id.get());
            QList<QStandardItem*> row;
            row.append(nameItem);
            row.append(descriptionItem);
            model->appendRow(row);
        }
    }
    else{
        //TODO: Show some Error message
    }

    _sourceModel = model;
    _proxyModel = new QSortFilterProxyModel(this);
    _proxyModel->setSourceModel(_sourceModel);
    ui->treeView->setModel(_proxyModel);
    QItemSelectionModel * selectModel = ui->treeView->selectionModel();
    connect(selectModel,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this, SLOT(slotCurrentRowChanged(QModelIndex,QModelIndex)));
}
