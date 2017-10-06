#include "mainwindow.h"
#include "3dview/canvas3d.h"
#include "sketching/sketchwindow.h"
#include "widgets/real_slider.h"


#include <QtWidgets>
#include <QActionGroup>
#include <QSlider>
#include <QDir>
#include <QFileInfo>
#include <QDesktopServices>



MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent )
{
    controller = nullptr;

    setupWindowProperties();
    createWindow();

    show();

    createController();
    setupController();
    run_app();

}


void MainWindow::setupWindowProperties()
{
    setAcceptDrops( true );
    setDockOptions( QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs |
                    QMainWindow::AllowTabbedDocks );

    setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    setDockNestingEnabled( true );

    QRect rect = QGuiApplication::primaryScreen()->geometry();

    app_width  = static_cast< int > (  rect.width() * 0.8f );
    app_height = static_cast< int > ( rect.height() * 0.8f );

    app_orig_x = static_cast< int > ( (  rect.width() - app_width  ) * 0.5f );
    app_orig_y = static_cast< int > ( ( rect.height() - app_height ) * 0.5f );

    setGeometry( app_orig_x, app_orig_y, app_width, app_height );
    setWindowTitle( "Rapid Reservoir Modelling" );
}


void MainWindow::createWindow()
{
    createMainInterface();
    createSketchingWindow();
}


void MainWindow::createMainInterface()
{

    canvas3d = new Canvas3d();

    sl_depth_csection = new RealSlider( Qt::Vertical );
    sl_depth_csection->setTickPosition( QSlider::TicksRight );
    sl_depth_csection->setInvertedAppearance( true );


    hb_central_widget = new QHBoxLayout( this );
    hb_central_widget->addWidget( canvas3d );
    hb_central_widget->addWidget( sl_depth_csection );


    central_widget = new QWidget( this );
    central_widget->setLayout( hb_central_widget );
    setCentralWidget( central_widget );

}



void MainWindow::createSketchingWindow()
{
    sketch_window = new SketchWindow();
    sketch_window->show();
}



void MainWindow::createController()
{
    if( controller != nullptr )
        delete controller;

    controller = new Controller();
}


void MainWindow::setupController()
{
    Scene3d* scene3d = canvas3d->getScene();
    controller->setScene3d( scene3d );
}


void MainWindow::run_app()
{
    controller->init();
    controller->addCrossSection( CrossSection::Direction::Z, 20 );
    sketch_window->addCanvas( controller->getCurrentCrossSection() );

    controller->addCrossSection( CrossSection::Direction::Z, 232 );
    sketch_window->addCanvas( controller->getCurrentCrossSection() );

    controller->addCrossSection( CrossSection::Direction::Z, 173 );
    sketch_window->addCanvas( controller->getCurrentCrossSection() );

    controller->addCrossSection( CrossSection::Direction::Z, 435 );
    sketch_window->addCanvas( controller->getCurrentCrossSection() );

//    sketch_window->addCanvas( 0 );
//    sketch_window->addCanvas( 1 );
//    sketch_window->addCanvas( 2 );
//    sketch_window->addCanvas( 0 );


//    controller->addCrossSection( CrossSection::Direction::X, 20 );
//    controller->addCrossSection( CrossSection::Direction::X, 232 );
//    controller->addCrossSection( CrossSection::Direction::X, 173 );
//    controller->addCrossSection( CrossSection::Direction::X, 435 );


//    controller->addCrossSection( CrossSection::Direction::Y, 20 );
//    controller->addCrossSection( CrossSection::Direction::Y, 232 );
//    controller->addCrossSection( CrossSection::Direction::Y, 173 );
//    controller->addCrossSection( CrossSection::Direction::Y, 435 );
}



