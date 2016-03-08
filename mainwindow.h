#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QQuickView>
#include <QListWidget>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Q_INVOKABLE void delete3D();
    ~MainWindow();

signals:
    void on_actionRotate_triggered();
    void on_actionDelete_triggered();
    void on_actionSave_triggered();
    void on_action3D_Preview_triggered();

private slots:
    void on_actionExit_triggered();
    void slotTriggered3D_Preview();

//    void on_actionRotate_triggered();

private:
    Ui::MainWindow *ui;
    QQuickView *view;

};

#endif // MAINWINDOW_H
