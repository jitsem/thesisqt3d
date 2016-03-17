#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QQuickView>
#include <QListWidget>

#include <memory>

class DrawZone;
class Calc;

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

    void enableIcons();
signals:
    void on_actionRotate_triggered();
    void on_actionDelete_triggered();
    void on_actionSave_triggered();
    void on_actionConnect_triggered();



private slots:
    void on_actionExit_triggered();
    void on_actionOpen_File_triggered();
    void on_action3D_Preview_triggered();
    void on_actionNew_triggered();
    void on_actionEdit_Goals_triggered();


private:
    Ui::MainWindow *ui;
    QQuickView *view;
    DrawZone *drawzoneWidget;
    std::shared_ptr<Calc> calculator;

};

#endif // MAINWINDOW_H
