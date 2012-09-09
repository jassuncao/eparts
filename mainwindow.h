#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


class QActionGroup;
class PartsMainWidget;
class QMenuBar;

namespace Ui {
    class MainWindow;
}

namespace Widgets {
    class FancyTabWidget;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void selectedPartsView();
    void selectedOtherView();
    void showOptions();

private:
    //Ui::MainWindow *ui;
    Widgets::FancyTabWidget *_tabWidget;
    QMenuBar *_menuBar;
    QMenu *_menuEdit;
    QActionGroup *toolsGroup;
    PartsMainWidget * partsMainWidget;
};

#endif // MAINWINDOW_H
