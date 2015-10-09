#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit MainWindow( QWidget *parent = 0 );


    protected:

        void createWindow();
        void createMenuBar();
        void create2DModule();
        void create3DModule();

    private:

        QDockWidget *dc_2DModule;
        QDockWidget *dc_3DModule;


};

#endif // MAINWINDOW_H
