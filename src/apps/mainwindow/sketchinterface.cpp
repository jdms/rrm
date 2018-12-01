#include "sketchinterface.h"

#include "mainwindow.h"

SketchInterface::SketchInterface( MainWindow* const& window_ )
{
    window = window_;
}


void SketchInterface::createInterface()
{
    if( window == nullptr ) return;

    createSketchingWindow();
    createSketchingActions();
}


void SketchInterface::createSketchingWindow()
{

    scontroller = new SketchingController();

    sketch_window = new SketchWindow();
    sketch_window->setToolTip( "Cross-Section Canvas" );

    dw_sketchwindow = new QDockWidget( "Cross-Section" );
    dw_sketchwindow->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketchwindow->setWidget( sketch_window );

    window->addDockWidget( Qt::TopDockWidgetArea, dw_sketchwindow );

    sketch_topview_window = new SketchWindow();
    sketch_topview_window->setToolTip( "Top-View Canvas" );
    dw_topview_window = new QDockWidget( "Top-View" );
    dw_topview_window->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_topview_window->setWidget( sketch_topview_window );

    window->addDockWidget( Qt::RightDockWidgetArea, dw_topview_window );

    scontroller->setMainWindow( std::shared_ptr< SketchWindow > ( sketch_window ) );
    scontroller->setTopViewWindow( std::shared_ptr< SketchWindow > ( sketch_topview_window ) );

    scontroller->setController( std::shared_ptr< Controller > ( window->controller ) );

}


void SketchInterface::createSketchingActions()
{

    ac_csection = new QAction ( "Cross-Section Window", window );
    ac_csection->setCheckable ( true );
    ac_csection->setChecked ( Settings::Application::DEFAULT_CSECTION_VISIBILITY );

    ac_topview = new QAction ( "Top-View Window", window );
    ac_topview->setCheckable ( true );
    ac_topview->setChecked ( Settings::Application::DEFAULT_TOPVIEW_VISIBILITY );

    window->mn_windows->addAction( ac_csection );
    window->mn_windows->addAction( ac_topview );


    ////

    connect( window, &MainWindow::runDiagnostics, this, &SketchInterface::showOnlyMainCanvas );


    connect( ac_csection, &QAction::toggled, dw_sketchwindow, &QDockWidget::setVisible );


    connect( ac_topview, &QAction::toggled, dw_topview_window, &QDockWidget::setVisible );


    connect( sketch_window, &SketchWindow::removeMarkerFromSlider, [=]( double id_ )
    {  window->app->removeMarkerFromSlider( id_ ); } );


    connect( sketch_window, &SketchWindow::defineColorCurrent, [=]( int red_, int green_, int blue_ )
    {
        window->app->defineCurrentColor( red_, green_, blue_ );
        if( sketch_topview_window == nullptr) return;
        sketch_topview_window->updateColorWidget( red_, green_, blue_ );
    } );


    connect( sketch_window, &SketchWindow::addCurve, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
    { window->app->addCurveToObject( curve_, dir_, depth_ ); } );


    connect( sketch_window, &SketchWindow::removeLastCurve, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
    { window->app->removeLastCurve( dir_, depth_ ); } );



    connect( sketch_window, &SketchWindow::createObject, [=]()
    { window->app->createObjectSurface(); } );


    connect( sketch_window, &SketchWindow::updateVolumeDimensions, [=]( const  Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_  )
    {
        window->app->setVolumeDimensions( dir_, width_, height_ );
    } );


    connect( sketch_window, &SketchWindow::setImageToCrossSection, [=]( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_  )
    {
        window->app->setImageToCrossSection( file_, dir_, depth_, ox_, oy_, w_, h_ );
    } );


    connect( sketch_window, &SketchWindow::removeImageFromCrossSection, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){ window->app->clearImageInCrossSection( dir_, depth_ ); } );


    connect( sketch_window, &SketchWindow::objectSelected, [=]( const std::size_t& id_  ){ window->app->setObjectSelectedAsBoundering( id_ ); } );


    connect( sketch_window, &SketchWindow::getRegionByPoint, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_  ){ window->app->getRegionByPointAsBoundering( px_, py_, depth_, dir_ ); } );


    connect( sketch_window, &SketchWindow::sendSketchOfSelection, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_  ){ window->app->selectBounderingBySketch( curve_, dir_, depth_  ); } );


    connect( sketch_window, &SketchWindow::setAreaChoosed, [=](){ /*window->app->setSketchRegion( false );*/ } );


     connect( sketch_window, &SketchWindow::sendPointGuidedExtrusion, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ )
     {
//         window->app->setPointGuidedExtrusion( px_, py_, depth_, dir_ );
         scontroller->setPointGuidedExtrusionInPath( px_, py_, depth_, dir_ );
     } );

     connect( sketch_window, &SketchWindow::stopSketchesOfSelection, [=](){ scontroller->enableSketching( true ); } );

//     connect( sketch_window, &SketchWindow::updatePointGuidedExtrusion, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ )
//     {
//         window->app->setPointGuidedExtrusion( px_, py_, depth_, dir_ );
//         scontroller->setPointGuidedExtrusionInPath( px_, py_, depth_, dir_ );
//     } );



    connect( sketch_topview_window, &SketchWindow::getRegionByPoint, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_  ){ window->app->getRegionByPointAsBoundering( px_, py_, depth_, dir_ ); } );



    connect( sketch_topview_window, &SketchWindow::defineColorCurrent, [=]( int red_, int green_, int blue_ )
    {
        window->app->defineCurrentColor( red_, green_, blue_ );
        if( sketch_window == nullptr) return;
        sketch_window->updateColorWidget( red_, green_, blue_ );
    } );

    connect( sketch_topview_window, &SketchWindow::addTrajectory, [=]( const PolyCurve& curve_ )
    { window->app->addTrajectoryToObject( curve_ ); } );

    connect( sketch_topview_window, &SketchWindow::useLastTrajectory, [=]()
    { window->app->previewLastTrajectory(); } );


    connect( sketch_topview_window, &SketchWindow::createObject, [=]()
    { window->app->createObjectSurface(); } );


    connect( sketch_topview_window, &SketchWindow::updateVolumeDimensions, [=]( const  Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_  )
    {
        window->app->setVolumeDimensions( dir_, width_, height_ );
    } );


    connect( sketch_topview_window, &SketchWindow::setImageToCrossSection, [=]( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_  )
    {
        Settings::CrossSection::CrossSectionDirections dir1_ = Settings::CrossSection::CrossSectionDirections::Y;
        window->app->setImageToCrossSection( file_, dir1_, depth_, ox_, oy_, w_, h_ );
    } );

    connect( sketch_topview_window, &SketchWindow::removeImageFromCrossSection, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){ window->app->clearImageInCrossSection( dir_, depth_ ); } );

    connect( sketch_topview_window, &SketchWindow::sketchDoneGuidedExtrusion, [=]( const PolyCurve& curve_  )
    {
        float px_, py_, pz_;
        scontroller->getGuidedExtrusionPoint( px_, py_, pz_ );
//        window->app->setGuidedExtrusion( px_, py_, pz_, curve_ );
    } );

    connect( sketch_topview_window, &SketchWindow::stopSketchesOfSelection, [=](){ scontroller->enableSketching( true ); } );


    connect( sketch_window, &SketchWindow::objectSelected, [=]( const std::size_t& id_  ){ window->app->setObjectSelectedAsBoundering( id_ ); } );


    connect( sketch_window, &SketchWindow::regionSelected, [=]( const std::size_t& id_, bool status_  ){ window->app->setRegionSelected( id_, status_ ); } );

        connect( sketch_window, &SketchWindow::setVerticalExaggeration, [=]( double scale_  ){ window->app->setVerticalExaggeration( scale_ ); } );


    connect( window->app, &RRMApplication::updateVolume, [=]()
    { scontroller->updateVolume(); } );


    connect( window->app, &RRMApplication::updateMainCrossSection, [=]()
    { scontroller->updateMainCrossSection(); } );


    connect( window->app, &RRMApplication::updateTopViewCrossSection, [=]()
    {  scontroller->updateObjects();  scontroller->updateTopViewCrossSection(); } );


    connect( window->app, &RRMApplication::changeToCrossSectionDirection, [=]()
    {   sketch_window->removeAllCanvas();
        scontroller->updateObjects(); scontroller->updateMainCrossSection(); scontroller->updateTopViewCrossSection();} );


    connect( window->app, &RRMApplication::changeToTopViewDirection, [=]()
    {   sketch_window->removeAllCanvas();
        scontroller->updateTopViewCrossSection(); scontroller->updateMainCrossSection();} );


    connect( window->app, &RRMApplication::addFixedCrossSectionWindow, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, QColor color_ )
    { scontroller->viewCrossSection( dir_, depth_, color_ ); } );


    connect( window->app, &RRMApplication::removeFixedCrossSectionWindow, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
    { scontroller->removeWindow( dir_, depth_ ); } );


    connect( window->app, &RRMApplication::addObject, [=]( const ObjectPtr& obj_ )
    { scontroller->addStratigraphy( obj_ ); } );


    connect( window->app, &RRMApplication::updateObjects, [=]()
    { scontroller->updateObjects(); } );


    connect( window->app, &RRMApplication::updateTrajectories, [=]()
    { scontroller->updateObjectsTrajectories(); } );


    connect( window->app, &RRMApplication::resetApplication, [=]()
    { scontroller->clear(); } );


    connect( window->app, &RRMApplication::startApplication, this, &SketchInterface::init );


    connect( window->app, &RRMApplication::disableVolumeResizing, [=](){ sketch_window->disableResizeVolume( true ); } );


    connect( window->app, &RRMApplication::enableVolumeResizing, [=](){ sketch_window->disableResizeVolume( false ); } );


    connect( window->app, &RRMApplication::disableVolumeResizing, [=](){ sketch_topview_window->disableResizeVolume( true ); } );


    connect( window->app, &RRMApplication::enableVolumeResizing, [=](){ sketch_topview_window->disableResizeVolume( false ); } );


    connect( window->app, &RRMApplication::selectEnabled, [=]( const std::string& option_, bool status_ )
    {
        if( option_.compare( "REGION") == 0 )
        {
            sketch_window->setModeRegionSelecting( status_ );
            sketch_topview_window->setModeRegionSelecting( status_ );
        }
        else if( option_.compare( "NONE") == 0 )
        {
            sketch_window->setModeSelecting( false );
            sketch_topview_window->setModeSelecting( false );
        }
        else
        {
            sketch_window->setModeSelecting( status_ );
            sketch_topview_window->setModeSelecting( status_ );
        }

    } );


    connect( window->app, &RRMApplication::enablePreserve, [=]( const std::string& option_, bool status_ )
    {
        if( option_.compare( "REGION") == 0 )
        {
            sketch_window->setModeRegionSelecting( status_ );
            sketch_topview_window->setModeRegionSelecting( status_ );
        }
        else
        {
            sketch_window->setModeSelectingStratigraphies( status_ );
            sketch_topview_window->setModeSelectingStratigraphies( status_ );
        }



    } );


    connect( window->app, &RRMApplication::setCurrentColor, [=]( int red_, int green_, int blue_ ){ sketch_window->updateColorWidget( red_, green_, blue_ ); } );


    connect( window->app, &RRMApplication::setCurrentColor, [=]( int red_, int green_, int blue_ ){ sketch_topview_window->updateColorWidget( red_, green_, blue_ ); } );



    connect( window->app, &RRMApplication::setCurveAsBoundering, [=]( const PolyCurve& boundary_ ){ scontroller->setCurveAsBoundering( boundary_ ); } );


    connect( window->app, &RRMApplication::clearBounderingArea, [=](){ scontroller->clearCurveAsBoundering(); } );


    connect( window->app, &RRMApplication::addRegions, [=](){ scontroller->enableSketching( false );  } );

    connect( window->app, &RRMApplication::addRegionCrossSectionBoundary, [=]( const RegionsPtr& reg_ ){ scontroller->addRegion( reg_ );  } );


    connect( window->app, &RRMApplication::updateRegions, [=](){ scontroller->updateRegions();  } );


    connect( window->app, &RRMApplication::clearRegions, [=](){ scontroller->clearRegions(); scontroller->enableSketching( true ); } );


    connect( window->app, &RRMApplication::updateBoundary, [=](){ scontroller->updateBoundering();  } );


    connect( window->app, &RRMApplication::updateImageInCrossSection, [=]()
    { scontroller->updateImageInScene(); } );


}




void SketchInterface::init()
{
    if( sketch_window != nullptr )
    {
        sketch_window->disableResizeVolume( false );
//        sketch_window->usingVerticalExaggeration( 0.2 );
        sketch_window->resetVerticalExaggeration();
        sketch_window->setDipAngle( 0.0 );
    }

    if( sketch_topview_window != nullptr )
        sketch_topview_window->disableResizeVolume( false );



    scontroller->init();
    showOnlyMainCanvas( false );

}


void SketchInterface::showOnlyMainCanvas( bool status_ )
{
    if( dw_sketchwindow != nullptr )
        dw_sketchwindow->setVisible( !status_ );

    if( dw_topview_window != nullptr )
        dw_topview_window->setVisible( !status_ );
}

