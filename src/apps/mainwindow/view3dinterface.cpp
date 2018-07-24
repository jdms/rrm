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

}


void View3dInterface::createView3dWindow()
{

    controller3d = new View3dController();

    canvas3d = new Canvas3d();
    sl_depth_csection = new RealFeaturedSlider( Qt::Vertical );

    QHBoxLayout* hb_central_widget = new QHBoxLayout();
    hb_central_widget->addWidget( canvas3d );
    hb_central_widget->addWidget( sl_depth_csection );

    QWidget* central_widget = new QWidget();
    central_widget->setLayout( hb_central_widget );

    window->setCentralWidget( central_widget );
    window->show();

}


void View3dInterface::createView3dActions()
{

    connect( sl_depth_csection, &RealFeaturedSlider::sliderMoved, [=]( double depth_ )
    { window->app->moveMainCrossSection( depth_ ); } );


    connect( window->app, &RRMApplication::updateVolume, [=]()
    {
    } );


    connect( window->app, &RRMApplication::updateRange, [=]( double min_, double max_ )
    {
        sl_depth_csection->setRange( min_, max_ );
    } );


    connect( window->app, &RRMApplication::updateDiscretization, [=]( const std::size_t& disc_ )
    { sl_depth_csection->setDiscretization( disc_ ); } );


    connect( window->app, &RRMApplication::addObject, [=]( const ObjectPtr& obj_ )
    { controller3d->addStratigraphy( obj_ ); } );

    connect( window->app, &RRMApplication::updateObjects, [=]()
    { controller3d->updateObjects(); } );

    connect( window->app, &RRMApplication::updateMainCrossSection, [=]()
    { controller3d->updateMainCrossSection(); } );

}


void View3dInterface::init()
{
    controller3d->setMainWindow( std::shared_ptr< Canvas3d > ( canvas3d ) );
    controller3d->setController( std::shared_ptr< Controller > ( window->controller ) );
    controller3d->init();
}
