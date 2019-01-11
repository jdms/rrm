#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <memory>

#include <QMainWindow>
#include <QToolBar>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>

#include "rrmapplication.h"
#include "sketchinterface.h"
#include "view3dinterface.h"
#include "diagnosticsinterface.h"

#include "./core/models/controller.h"
#include "./core/widgets/objecttree.h"
#include "./core/widgets/pages_stack.h"

#include "./about/about_widget.hpp"



class MainWindow: public QMainWindow
{
    Q_OBJECT


    public:

        MainWindow( QWidget* parent_ = 0 );
        ~MainWindow();
        void run();

    public slots:

        void save();
        void load();

        void lockDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );
        void lockPreserve( const std::string& option_ );
        void lockUndoRedo( bool status_ );

        void initializeInterface();

        void activatePreserveAbove( bool status_ );
        void activatePreserveBelow( bool status_ );
        void activatePreserveRegion( bool status_ );

        bool isRegionEnabled() const;


    signals:

        void runDiagnostics( bool status_ );
        void takeScreenshot();

    protected:

        void setWindowProperties();

        void createWindow();
        void createActions();
        void createMenuBar();
        void createToolbar();
        void createSideBar();

        void createController();
        void createObjectTree();

        void plugSketchInterface();
        void plug3dInterface();
        void plug3dDiagnosticsInterface();

        void showHelp();

//        void checkButtonsStatus();


    protected:

        friend class SketchInterface;
        friend class View3dInterface;
        friend class DiagnosticsInterface;
        friend class RRMApplication;

        std::shared_ptr< SketchInterface > sketchapp = nullptr;
        std::shared_ptr< View3dInterface > view3dapp = nullptr;
        std::shared_ptr< DiagnosticsInterface > diagapp = nullptr;

        ObjectTree* object_tree = nullptr;
        QDockWidget* dw_object_tree = nullptr;

        Controller* controller = nullptr;
        RRMApplication* app = nullptr;

        QAction* ac_save = nullptr;
        QAction* ac_load = nullptr;
        QAction* ac_clear = nullptr;
        QAction *ac_export = nullptr;
        QAction* ac_exit = nullptr;
        QAction* ac_manual = nullptr;
        QAction* ac_about = nullptr;

        QAction* ac_undo = nullptr;
        QAction* ac_redo = nullptr;
        QAction* ac_sketch_above = nullptr;
        QAction* ac_sketch_region = nullptr;
        QAction* ac_sketch_below = nullptr;
        QAction* ac_remove_above = nullptr;
        QAction* ac_remove_above_int = nullptr;
        QAction* ac_remove_below = nullptr;
        QAction* ac_remove_below_int = nullptr;
        QAction* ac_screenshot = nullptr;

        QAction* ac_direction_x = nullptr;
        QAction* ac_direction_y = nullptr;
        QAction* ac_direction_z = nullptr;

        QAction* ac_stratigraphy = nullptr;
        QAction* ac_structural = nullptr;

        QAction* ac_regions = nullptr;
        QAction* ac_diagnostics = nullptr;

        QMenu* mn_file = nullptr;
        QMenu* mn_windows = nullptr;
        QMenu* mn_help = nullptr;

        QToolBar* tb_mainwindow = nullptr;

        PagesStack* ps_objectdata = nullptr;
        QDockWidget* dw_object_properties = nullptr;

        AboutWidget* about_rrm = nullptr;


};

#endif // MAINWINDOW1_H
