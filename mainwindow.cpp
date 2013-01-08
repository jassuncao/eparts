#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "partsmainwidget.h"
#include "settingsdialog.h"
#include "parttypedialog.h"
#include "widgets/fancytabwidget.h"
#include "widgets/stylehelper.h"
#include "editattributedialog.h"
#include "selectattributedialog.h"
#include "editpartdialog.h"
#include "models/attributesrepository.h"
#include <QList>

//using namespace Widgets;
using namespace Models;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
//    ,ui(new Ui::MainWindow)
{
    resize(800, 600);
    //ui->setupUi(this);

    /*
    partsMainWidget = new PartsMainWidget(ui->stackedWidget);
    ui->stackedWidget->insertWidget(0,partsMainWidget);
    toolsGroup = new QActionGroup(this);    
    QAction * action1 = ui->mainToolBar->addAction(QIcon(":/images/edit_add_22x22.png"),"Add",this,SLOT(selectedPartsView()));
    action1->setCheckable(true);
    QAction * action2 = ui->mainToolBar->addAction(QIcon(":/images/edit_remove_22x22.png"),"Remove",this,SLOT(selectedOtherView()));    
    action2->setCheckable(true);
    toolsGroup->addAction(action1);
    toolsGroup->addAction(action2);
    action1->setChecked(true);
    connect(ui->actionOptions,SIGNAL(triggered()),this,SLOT(showOptions()));
    ui->stackedWidget->setCurrentIndex(0);
    */
    Widgets::StyleHelper::setBaseColor(Qt::darkGray);
    _tabWidget= new Widgets::FancyTabWidget(this);

    partsMainWidget = new PartsMainWidget(this);

    _tabWidget->insertTab(0, partsMainWidget,QIcon(":/images/folder_48x48.png"),"Parts" );
    _tabWidget->insertTab(1, new QWidget(this),QIcon(":/images/list_48x48.png"),"Shopping List" );
    _tabWidget->setTabEnabled(0, true);
    _tabWidget->setTabEnabled(1, true);
    _tabWidget->setCurrentIndex(0);
    _menuBar = new QMenuBar(this);
    _menuBar->setGeometry(QRect(0, 0, 734, 22));
    _menuEdit = new QMenu("Edit",_menuBar);
    _menuBar->addAction(_menuEdit->menuAction());
    _menuEdit->addAction("Settings...",this, SLOT(showOptions()));

    setCentralWidget(_tabWidget);
    setMenuBar(_menuBar);

}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::selectedPartsView()
{
    //ui->stackedWidget->setCurrentIndex(0);
    DQQuery<PartType> query;
    query = query.filter(DQWhere("name")=="Fixed Resistors").orderBy("name");
    PartType model;
    if(query.exec()){
        if(query.next()){
            query.recordTo(model);
        }
    }
    PartTypeDialog dlg(this);
    dlg.setModel(&model);
    dlg.exec();
}

void MainWindow::selectedOtherView()
{
   // ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showOptions()
{
    //SettingsDialog dlg(this);        
    //EditAttributeDialog dlg(this);
    //SelectAttributeDialog dlg(this);
    AttributesRepository repo;
    repo.load();
    //const QList<const AbstractPartAttribute*> mostUsed = repo.listMostUsedAttributes(10);
    /*
    DQQuery<DQFloatValue> query;
    DQQuery<DQAttribute> query;
    DQList<DQAttribute> result;
    result = query.orderBy("name").all();
    */
    EditPartDialog dlg(&repo,this);
    dlg.exec();
}

