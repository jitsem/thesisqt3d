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

/**Class for the main Application window*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**Make sure there is only one instance of the Calc*/
    static std::shared_ptr<MainWindow> Instance();
    /**Destructor of MainWindow*/
    ~MainWindow();
    /**Stop 3D simulation, invokable from Qml*/
    Q_INVOKABLE void delete3D();
    /**Enable some icons and edit plane*/
    void enableInterface();
    /**Toggle some icons*/
    void toggleIcons(bool b);
    /**Returns ui*/
    Ui::MainWindow *getUi() const;
    /**Getter for GridSize*/
    Q_INVOKABLE int getGridSize() const;
    /**Getter and Setter for GridSize*/
    void setGridSize(int value);

signals:

    /**Signal for saving*/
    void on_actionSave_triggered();
    /**Signal for setting ground*/
    void on_actionGround_triggered();

private slots:

    /**Exit slot for application*/
    void on_actionExit_triggered();
    /**Slot to open File*/
    void on_actionOpen_File_triggered();
    /**Slot to launch 3D simulation*/
    void on_action3D_Preview_triggered();
    /**Slot to Create new file*/
    void on_actionNew_triggered();
    /**Slot to Edit Level Goals*/
    void on_actionEdit_Goals_triggered();
    /**Slot to Rotate a component*/
    void on_actionRotate_triggered();
    /**Slot to Delete a component*/
    void on_actionDelete_triggered();
    /**Slot to Copy functionality for components*/
    void on_action_Copy_triggered();
    /**Slot to Paste Functionality for components*/
    void on_action_Paste_triggered();
    /**Slot to Show an about message*/
    void on_actionAbout_triggered();
    /**Show info message*/
    void on_actionInfo_triggered();

private:


    /**Private Constructor for MainWindow*/
    explicit MainWindow(QWidget *parent = 0);
    /**Seperate fucntion to setup Ui*/
    void setUpUi();
    /**Instance variable, makes sure only one instance exists*/
    static std::shared_ptr<MainWindow> instance;
    /**Ui form*/
    Ui::MainWindow *ui;
    /**3D view, Contains all Qml*/
    QQuickView * view;
    /**Drawzone for circuit drawing*/
    DrawZone *drawzoneWidget;
    /**Calc class for calculations and i/o*/
    std::shared_ptr<Calc> calculator;
    /**Clipboard*/
    QList<Component_lb*> copied;
    /**Global Size Value*/
    int gridSize;
};

#endif // MAINWINDOW_H
