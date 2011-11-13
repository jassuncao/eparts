#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QActionGroup;
class PartsMainWidget;

namespace Ui {
    class MainWindow;
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
    Ui::MainWindow *ui;
    QActionGroup *toolsGroup;
    PartsMainWidget * partsMainWidget;
};

#endif // MAINWINDOW_H
