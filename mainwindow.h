#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QListWidget>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void on_actionRotate_triggered();
    void on_actionDelete_triggered();
    void on_actionSave_triggered();
    void on_action3D_Preview_triggered();

private slots:
    void on_actionExit_triggered();

//    void on_actionRotate_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
