#include <QHBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QStyle>

#include "MainWindow.h"



MainWindow::MainWindow( QWidget *parent) : QMainWindow( parent )
{
    createWindow();
}

void MainWindow::createWindow()
{
    this->setMinimumSize( 800, 600 );

    createActions();
    createMenuBar();
    createToolbar();
    create2DModule();
    create3DModule();

    statusBar()->showMessage( "Status" );

}

void MainWindow::createActions()
{
    ac_new = new QAction( tr( "&New" ), this );
    ac_new->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/page_white.png" ) );

    ac_open = new QAction( tr( "&Open" ), this );
    ac_open->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/folder.png" ) );

    ac_save = new QAction( tr( "&Save" ), this );
    ac_save->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/diskette.png" ) );

    ac_export = new QAction( tr( "&Export" ), this );
    ac_export->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/document_export.png" ) );

    ac_exit = new QAction( tr( "E&xit" ), this );
    ac_exit->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/door_out.png" ) );

    ac_compute = new QAction( tr( "&Compute" ), this );
    ac_compute->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/sum.png" ) );

    ac_contents = new QAction( tr( "Contents" ), this );
    ac_about = new QAction( tr( "&About" ), this );

    ac_removeabove = new QAction( tr( "Remove Above" ), this );;
    ac_removeabove->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/removeabove.png" ) );

    ac_removebelow = new QAction( tr( "Remove Below" ), this );;
    ac_removebelow->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/removebelow.png" ) );

    ac_select = new QAction( tr( "Select" ), this );;
    ac_select->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/pointer.png" ) );

    ac_select1 = new QAction( tr( "Select All" ), this );;
    ac_select1->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/select.png" ) );

    ac_grab = new QAction( tr( "Grab" ), this );;
    ac_grab->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/grab.png" ) );

    cd_pickercolor = new QColorDialog();
    ac_sketchcolor = new QWidgetAction( this );
    ac_sketchcolor->setDefaultWidget( cd_pickercolor );



    connect( ac_new, SIGNAL( triggered() ), this, SLOT( newSection() ) );



}

void MainWindow::createMenuBar()
{

    QMenu *mn_file = menuBar()->addMenu( tr( "&File" ) );
    mn_file->addAction( ac_new );
    mn_file->addAction( ac_open );
    mn_file->addAction( ac_save );
    mn_file->addAction( ac_export );
    mn_file->addAction( ac_exit );

    QMenu *mn_tools = menuBar()->addMenu( tr( "&Tools" ) );
    mn_tools->addAction( ac_compute );

    QMenu *mn_help = menuBar()->addMenu( tr( "&Help" ) );
    mn_help->addAction( ac_contents );
    mn_help->addAction( ac_about );


}

void MainWindow::createToolbar()
{
    tlb_section = addToolBar( tr( "Section" ) );
    tlb_section->addAction( ac_new );
    tlb_section->addAction( ac_open );
    tlb_section->addAction( ac_save );
    tlb_section->addAction( ac_export );

    tlb_computation = addToolBar( tr( "Computation" ) );
    tlb_computation->addAction( ac_compute );


    tlb_rules = addToolBar( tr( "Rules" ) );
    tlb_rules->addAction( ac_removeabove );
    tlb_rules->addAction( ac_removebelow );


    tlb_interaction = addToolBar( tr( "Interactions" ) );
    tlb_interaction->addAction( ac_select );
    tlb_interaction->addAction( ac_select1 );
    tlb_interaction->addAction( ac_grab );


    tlb_customization = addToolBar( tr( "Customize" ) );

    tbt_colorsketch = new QToolButton;
    tbt_colorsketch->setPopupMode( QToolButton::MenuButtonPopup );
    tbt_colorsketch->setIcon( QIcon( "C:/Users/Clarissa/Dropbox/Work/Projects/RRM/Code/Interface/InterfaceRRM/images/border_color.png" ) );

    mn_pickercolor = new QMenu();
    mn_pickercolor->addAction( ac_sketchcolor );
    tbt_colorsketch->setMenu( mn_pickercolor );
    tlb_customization->addWidget( tbt_colorsketch );

}

void MainWindow::create2DModule()
{

    dc_2DModule = new QDockWidget( this );
    dc_2DModule->setAllowedAreas( Qt::LeftDockWidgetArea );

    canvas2D = new Canvas2D( this );

    dc_2DModule->setWidget( canvas2D );
    addDockWidget( Qt::LeftDockWidgetArea, dc_2DModule );

}

void MainWindow::create3DModule()
{

    dc_3DModule = new QDockWidget( this );
    dc_3DModule->setAllowedAreas( Qt::RightDockWidgetArea );

    QGLFormat fmt;
    fmt.setVersion( 4, 1 );
    fmt.setProfile( QGLFormat::CoreProfile);

    Canvas3D *canvas3d = new Canvas3D( fmt );

    dc_3DModule->setWidget( canvas3d );
    addDockWidget( Qt::RightDockWidgetArea, dc_3DModule );

}

void MainWindow::newSection()
{
    clearCanvas2D();
//    clearCanvas3D();
//    clearComputation();
}

void MainWindow::clearCanvas2D()
{
    canvas2D->clear();
//    resetToolBar();
}
