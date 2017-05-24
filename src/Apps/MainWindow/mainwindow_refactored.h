#ifndef MAINWINDOW_REFACTORED_H
#define MAINWINDOW_REFACTORED_H

#include <QMainWindow>

class QActionGroup;
class QAction;
class QToolbar;
class QSlider;

class Canvas3d_Refactored;
class SketchWindow;
class ObjectTree;


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
        void setDefaultValues();


    private:

        void createWindow();
        void setupWindowProperties();
        void createMainInterface();
        void createCrossSectionInterface();
        void createTopViewInterface();
        void createSidebar();
        void createToolbarActions();

        inline void setupController(){}
        inline void initController(){}


    private:

        int app_height;
        int app_width;

        int app_orig_x;
        int app_orig_y;

        QSlider* sl_depth_csection;

        Canvas3d_Refactored *canvas3d;
        SketchWindow* sketch_window;
        ObjectTree* object_tree;

        QDockWidget* dw_object_tree;


        const bool OBJECTTREE_VISIBLE = true;


};

#endif // MAINWINDOW_REFACTORED_H
