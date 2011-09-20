#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "partsmainwidget.h"

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
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectedPartsView()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::selectedOtherView()
{
    ui->stackedWidget->setCurrentIndex(1);
}

