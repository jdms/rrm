#ifndef MAINWINDOW_REFACTORED_H
#define MAINWINDOW_REFACTORED_H

#include <QMainWindow>

class MainWindow_Refactored : public QMainWindow
{
    Q_OBJECT

   public:

        explicit MainWindow_Refactored( QWidget *parent = 0 );


    protected slots:

        inline void showSidebar( bool status_ ){}
        inline void showObjectTree( bool status_ ){}
        inline void showObjectProperties( bool status_ ){}

        inline void clear(){}
        inline void resetInterface(){}
        inline void setDefaultValues(){}


    private:

        inline void createMainInterface(){}
        inline void createCrossSectionInterface(){}
        inline void createTopViewInterface(){}
        inline void createSidebar(){}

        inline void setupController(){}
        inline void initController(){}


};

#endif // MAINWINDOW_REFACTORED_H
