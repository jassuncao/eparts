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
#include "models/attributestablemodel.h"
#include <models/attributesrepository.h>

using namespace Models;

SelectAttributeDialog::SelectAttributeDialog(AttributesRepository * attributesRepository, const QList<int> & attributesToExclude, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectAttributeDialog),
    _selectedAttribute(0),
    _attributesRepository(attributesRepository)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    _timer = new QTimer(this);
    _timer->setSingleShot(true);
    initAttributesModel(attributesToExclude);
    QShortcut * shortcut = new QShortcut(QKeySequence(QKeySequence::Find),this);
    connect(shortcut,SIGNAL( activated() ), ui->lineEdit, SLOT( setFocus() ) );
    connect( _timer, SIGNAL( timeout() ), this, SLOT( slotSearchLineActivate() ) );
    connect( ui->lineEdit, SIGNAL( textChanged( const QString& ) ), SLOT( slotSearchLineChange( const QString& ) ) );
}

SelectAttributeDialog::~SelectAttributeDialog()
{
    delete ui;
}

AbstractPartAttribute *SelectAttributeDialog::getSelectedAttribute() const
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

void SelectAttributeDialog::slotCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{

    QStandardItem *item = _sourceModel->itemFromIndex(_proxyModel->mapToSource(current));
    if(item) {
        QObject * obj = qvariant_cast<QObject *>(item->data());
        AbstractPartAttribute * attr= qobject_cast<AbstractPartAttribute*>(obj);
        if(attr){
            qDebug()<<"Item selected:"<<attr->name();
            _selectedAttribute = attr;
        }
    }
    else{
        _selectedAttribute=0;
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(_selectedAttribute>=0);   
}

void SelectAttributeDialog::newAttribute()
{
    qDebug()<<"New attribute";
    EditAttributeDialog dlg(this);
    if(dlg.exec()){        
        _selectedAttribute = _attributesRepository->addAttribute(
                    dlg.attributeName(),
                    dlg.attributeDescription(),
                    dlg.attributeType(),
                    dlg.attributeUnit()
                    );
        done(Accepted);
    }
}


void SelectAttributeDialog::initAttributesModel(const QList<int> & attributesToExclude)
{
    QStandardItemModel * model = new QStandardItemModel(this);
    model->setColumnCount(2);
    QStringList columnNames;
    columnNames<<tr("Attribute")<<tr("Description");
    model->setHorizontalHeaderLabels(columnNames);
    QList<AbstractPartAttribute*> attributes = _attributesRepository->attributes();
    AbstractPartAttribute* attr;
    foreach(attr, attributes){
        if(attributesToExclude.contains(attr->id()))
            continue;
        QStandardItem * nameItem = new QStandardItem(attr->name());
        QVariant v = qVariantFromValue((QObject*)attr);
        nameItem->setData(v);
        nameItem->setEditable(false);
        QStandardItem * descriptionItem = new QStandardItem(attr->description());
        descriptionItem->setEditable(false);
        descriptionItem->setData(v);
        QList<QStandardItem*> row;
        row.append(nameItem);
        row.append(descriptionItem);
        model->appendRow(row);
    }

    _sourceModel = model;
    _proxyModel = new QSortFilterProxyModel(this);
    _proxyModel->setSourceModel(_sourceModel);
    ui->treeView->setModel(_proxyModel);
    QItemSelectionModel * selectModel = ui->treeView->selectionModel();    
    connect(selectModel,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this, SLOT(slotCurrentRowChanged(QModelIndex,QModelIndex)));    
}
