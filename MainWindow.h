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
        void createActions();


    private:

        QDockWidget *dc_2DModule;
        QDockWidget *dc_3DModule;

        QAction *ac_new;
        QAction *ac_open;
        QAction *ac_save;
        QAction *ac_export;
        QAction *ac_exit;

        QAction *ac_compute;

        QAction *ac_contents;
        QAction *ac_about;

};

#endif // MAINWINDOW_H
