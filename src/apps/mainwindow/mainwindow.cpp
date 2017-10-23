#include "mainwindow.h"
#include "3dview/canvas3d.h"
#include "sketching/sketchwindow.h"
#include "widgets/realfeaturedslider.h"


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
    sl_depth_csection = new RealFeaturedSlider( Qt::Vertical );
    sl_depth_csection->setDiscretization( 500 );
    sl_depth_csection->setRange( 0, 500 );


    hb_central_widget = new QHBoxLayout( this );
    hb_central_widget->addWidget( canvas3d );
    hb_central_widget->addWidget( sl_depth_csection );


    central_widget = new QWidget( this );
    central_widget->setLayout( hb_central_widget );
    setCentralWidget( central_widget );


    connect( this, &MainWindow::updateVolume, [=](){ double width_ = 0, height_ = 0, length_ = 0;
                                                     controller->getVolumeDimensions( width_, height_, length_ );
                                                     sl_depth_csection->setRange( 0, length_ ); } );

}


void MainWindow::createSketchingWindow()
{

    sketch_window = new SketchWindow();

    dw_sketchwindow = new QDockWidget( "Cross-Section" );
    dw_sketchwindow->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketchwindow->setWidget( sketch_window );
    addDockWidget( Qt::BottomDockWidgetArea, dw_sketchwindow );



    connect( sl_depth_csection, &RealFeaturedSlider::markValue, [=]( const double& v ){ controller->addCrossSection( CrossSection::Direction::Z, v );
                                                                                        sketch_window->addCanvas( controller->getCurrentCrossSection() ); } );

    connect( sl_depth_csection, &RealFeaturedSlider::unmarkValue, [=]( double v ){   controller->removeCrossSection( CrossSection::Direction::Z, v );
                                                                                     sketch_window->removeCanvas( controller->getCrossection( v ) ); } );

    connect( sl_depth_csection, &RealFeaturedSlider::hightlightValue, [=]( double v ){  controller->setCurrentCrossSection( v );
                                                                                        sketch_window->highlightCanvas( controller->getCurrentCrossSection() ); }  );


    connect( this, &MainWindow::updateVolume, sketch_window, &SketchWindow::updateCanvas );


    connect( sketch_window, &SketchWindow::updateVolume, [=]( CrossSection::Direction dir_, double w, double l ){ controller->acceptVolumeDimensions( dir_, w, l );
                                                                                                                  emit updateVolume(); } );

    connect( sketch_window, &SketchWindow::acceptCurve, [=]( const PolyCurve& curve_ ){ controller->addObjectCurve( curve_ );
                                                                                        emit updateObject( controller->getIndexCurrentObject() ); } );

    connect( this, &MainWindow::updateObject, sketch_window, &SketchWindow::updateObject );

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

    emit sl_depth_csection->markValue( sl_depth_csection->maximumValue() );

}

