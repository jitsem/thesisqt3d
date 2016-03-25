#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>


class QWidget;
class QQuickView;
class Component_lb;
class DrawZone;
class Calc;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    //Make sure there is only one instance of the Calc
    static std::shared_ptr<MainWindow> Instance();

    ~MainWindow();

    //Stop 3D simulation
    Q_INVOKABLE void delete3D();

    //Enable edit icons
    void enableIcons();

    //Return ui
    Ui::MainWindow *getUi() const;

    //Getter and Setter for GridSize
    Q_INVOKABLE int getGridSize() const;
    void setGridSize(int value);

signals:

    //Signal for saving
    void on_actionSave_triggered();

    //Signal for setting ground
    void on_actionGround_triggered();

private slots:
    //Exit
    void on_actionExit_triggered();

    //Open File
    void on_actionOpen_File_triggered();

    //Launch 3D simulation
    void on_action3D_Preview_triggered();

    //Create new file
    void on_actionNew_triggered();

    //Edit Level Goals
    void on_actionEdit_Goals_triggered();

    //Rotate a component
    void on_actionRotate_triggered();

    //Delete a component
    void on_actionDelete_triggered();

    //Copy functionality for components
    void on_action_Copy_triggered();

    //Paste Functionality for components
    void on_action_Paste_triggered();

    //Show an about message
    void on_actionAbout_triggered();

    //Show info message
    void on_actionInfo_triggered();

private:


    //Private Constructor
    explicit MainWindow(QWidget *parent = 0);
    //Seperate fucntion to setup Ui
    void setUpUi();
    //Instance
    static std::shared_ptr<MainWindow> instance;
    //Ui
    Ui::MainWindow *ui;
    //3D view
    QQuickView * view;
    //Drawzone for circuit drawing
    DrawZone *drawzoneWidget;
    //Calculator class for calculations and i/o
    std::shared_ptr<Calc> calculator;
    //Clipboard
    QList<Component_lb*> copied;
    //Global Size Value
    int gridSize;
};

#endif // MAINWINDOW_H
