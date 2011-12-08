#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "partsmainwidget.h"
#include "settingsdialog.h"
#include "parttypedialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showOptions()
{
    SettingsDialog dlg(this);
    dlg.exec();
}

