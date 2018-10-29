#include <QDir>

#include "view3dinterface.h"

#include "mainwindow.h"

View3dInterface::View3dInterface( MainWindow* const& window_ )
{
    window = window_;
}


void View3dInterface::createInterface()
{
    if( window == nullptr ) return;

    createView3dWindow();
    createView3dActions();



//    QString arg_( QDir::separator() );
//    QString path_ = QDir::currentPath().append( arg_ ).append( "shaders" );
//    std::cout << "Binary dir: " << path_.toStdString().c_str() << std::endl << std::flush;

//    QDir dir_;
//    bool status_ = dir_.cd( path_ );
//    if( status_ == true )
//        std::cout << "Path exists " << std::endl << std::flush;

//    else
//        std::cout << "Path not exists " << std::endl << std::flush;


}


void View3dInterface::createView3dWindow()
{

    controller3d = new View3dController();

    canvas3d = new Canvas3d();
    canvas3d->setToolTip( "Canvas 3D" );
//    canvas3d->show();

    sl_depth_csection = new RealFeaturedSlider( Qt::Vertical );
    sl_depth_csection->setToolTip( "Move Cross-section" );

    QHBoxLayout* hb_central_widget = new QHBoxLayout();
    hb_central_widget->addWidget( canvas3d );
    hb_central_widget->addWidget( sl_depth_csection );


    QFrame* frame = new QFrame();
    frame->setFrameStyle(QFrame::Box | QFrame::Raised );
    frame->setLineWidth( 0 );
    frame->setMidLineWidth( 1 );
    frame->setLayout( hb_central_widget );

    QHBoxLayout* hb_layout = new QHBoxLayout();
    hb_layout->addWidget( frame );

    QWidget* central_widget = new QWidget();
    central_widget->setLayout( hb_layout );

    window->setCentralWidget( central_widget );
    window->show();


    controller3d->setMainWindow( std::shared_ptr< Canvas3d > ( canvas3d ) );
    controller3d->setController( std::shared_ptr< Controller > ( window->controller ) );



}


void View3dInterface::createView3dActions()
{

    connect( sl_depth_csection, &RealFeaturedSlider::sliderMoved, [=]( double depth_ )
    { window->app->moveMainCrossSection( depth_ ); } );

    connect( sl_depth_csection, &RealFeaturedSlider::sliderReleased, [=]()
    { window->app->updateRegionBoundary(); } );

    connect( sl_depth_csection, &RealFeaturedSlider::markValue, [=]( const double& depth_, QColor color_ )
    { window->app->addFixedCrossSection( depth_, color_ );  } );

    connect( sl_depth_csection, &RealFeaturedSlider::unmarkValue, [=]( const double& depth_ )
    { window->app->removeFixedCrossSection( depth_ );  } );

    connect( window->app, &RRMApplication::updateVolume, [=]()
    { controller3d->updateVolume(); } );

    connect( window->app, &RRMApplication::updateRange, [=]( double min_, double max_, bool inverted_  )
    {
        sl_depth_csection->setRange( min_, max_ );
        sl_depth_csection->setInvertedControls( !inverted_ );
        sl_depth_csection->setInvertedAppearance( !inverted_ );
    } );

    connect( window->app, &RRMApplication::updateDiscretization, [=]( const std::size_t& disc_ )
    { sl_depth_csection->setDiscretization( disc_ ); } );

    connect( window->app, &RRMApplication::addObject, [=]( const ObjectPtr& obj_ )
    { controller3d->addStratigraphy( obj_ ); } );

    connect( window->app, &RRMApplication::updateObjects, [=]()
    { controller3d->updateObjects(); } );


    connect( window->app, &RRMApplication::changeToTopViewDirection, [=]()
    { controller3d->setTopViewCrossSection(); } );


    connect( window->app, &RRMApplication::changeToCrossSectionDirection, [=]()
    { controller3d->setMainViewCrossSection(); } );


    connect( window->app, &RRMApplication::updateMainCrossSection, [=]()
    { controller3d->updateMainCrossSection(); } );

    connect( window->app, &RRMApplication::updateTopViewCrossSection, [=]()
    { controller3d->updateMainCrossSection(); } );


    connect( window->app, &RRMApplication::resetApplication, [=]()
    { controller3d->clear(); } );

    connect( window->app, &RRMApplication::startApplication, [=]{ controller3d->init(); }
    );

    connect( window->app, &RRMApplication::addRegions, [=]{ controller3d->addRegions(); }
    );

    connect( window->app, &RRMApplication::updateRegions, [=]{ controller3d->updateRegions(); } );

    connect( window->app, &RRMApplication::clearRegions, [=]{ controller3d->clearRegions(); } );

    connect( window->app, &RRMApplication::setVerticalExaggerationScale, [=]( double scale_ ){ controller3d->setVerticalExaggeration( scale_ ); } );

    connect( window, &MainWindow::takeScreenshot, [=](){ canvas3d->screenshot(); } );

}


void View3dInterface::init()
{
    controller3d->init();
}
