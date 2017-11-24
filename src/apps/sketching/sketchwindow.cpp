#include <QToolBar>

#include "sketchwindow.h"


SketchWindow::SketchWindow( QWidget* parent ): QMainWindow( parent )
{
    createWindow();
    createToolBar();
}



void SketchWindow::createToolBar()
{
    QToolBar *tb_actions = new QToolBar();


    cp_color = new ColorPicker( this );
    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor& color_ ){ emit defineColorCurrent( color_ ); } );

    ac_discard = new QAction( "Discard", this );
    ac_commit = new QAction( "Commit", this );
    ac_create = new QAction( "Create", this );

    ac_edit_scene = new QAction( "Edit Scene", this );
    ac_edit_scene->setCheckable( true );


    tb_actions->addWidget( cp_color );
    tb_actions->addAction( ac_discard );
    tb_actions->addAction( ac_commit );
    tb_actions->addAction( ac_create );
    tb_actions->addAction( ac_edit_scene );

    addToolBar( tb_actions );

}


void SketchWindow::createWindow()
{
    cs = new CanvasStack();

    hb_central_widget = new QHBoxLayout( this );
    hb_central_widget->addWidget( cs );

    QWidget* central_widget = new QWidget( this );
    central_widget->setLayout( hb_central_widget );
    setCentralWidget( central_widget );

}



void SketchWindow::setupScene( SketchScene* const& scene_ )
{
    QColor color_ = cp_color->currentColor();
    scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() );


    connect( ac_discard, &QAction::triggered, [=](){ emit scene_->discard(); } );

    connect( ac_commit, &QAction::triggered, [=](){ emit scene_->commit(); } );

    connect( ac_create, &QAction::triggered, [=](){ emit scene_->create(); } );


    connect( scene_, &SketchScene::acceptVolumeDimensions, [=]( CrossSection::Direction dir_, double w, double h ){ emit updateVolume( dir_, w, h ); } );

    connect( scene_, &SketchScene::acceptCurve, [=]( const PolyCurve& curve_ ){ emit acceptCurve( curve_ ); } );

    connect( scene_, &SketchScene::commitObject, [=](){ emit commitObject(); } );

    connect( scene_, &SketchScene::objectSelected, [=]( std::size_t index_ ){ emit objectSelected( index_ ); } );

//    connect( scene_, &SketchScene::setAsCurrent, [=]( double depth_, QGraphicsView* gview_ ){  emit setAsCurrent( depth_ );
//                                                                                               /*if( gview_ == main ) highlightCanvas( -1 );
//                                                                                               else highlightCanvas( depth_ );*/ } );






//    connect( ac_edit_scene, &QAction::toggled, scene_, &SketchScene::edit );

//    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ ){ if( scene_ == nullptr ) return;  scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );


}


void SketchWindow::addMainCanvas( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;

    SketchScene* scene_ = new SketchScene( cs_ );
    scene_->setCurrent( true );
    setupScene( scene_ );

    main = new QGraphicsView();
    main->scale( 1, -1 );
    main->setScene( scene_ );

    hb_central_widget->insertWidget( 0, main );

}


void SketchWindow::addTopViewCanvas( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;

    SketchScene* scene_ = new SketchScene( cs_ );
    scene_->setCurrent( true );
    setupScene( scene_ );

    tv_main = new QGraphicsView();
    tv_main->setScene( scene_ );

    hb_central_widget->insertWidget( 0, tv_main );

}



void SketchWindow::updateCanvas()
{

    if( main != nullptr )
    {
        SketchScene* sc_main_ = ( SketchScene* )( main->scene() );
        sc_main_->updateVolume();
        sc_main_->updateCrossSection();
    }

    if( tv_main == nullptr ) return;

    SketchScene* tv_main_ = ( SketchScene* )( tv_main->scene() );
    tv_main_->updateVolume();
    tv_main_->updateCrossSection();

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->updateCrossSection();
//    }

}




void SketchWindow::addCrossSection( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;

    SketchScene* sc_ = ( SketchScene* )( tv_main->scene() );
    sc_->addCrossSection( cs_ );

}



void SketchWindow::addObject( Object* const& obj_ )
{

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( main->scene() );
        sc_->addObject( obj_ );
//    }

}


void SketchWindow::updateObject( const std::size_t& index_ )
{
//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( main->scene() );
        sc_->updateObject( index_ );
//    }


}


void SketchWindow::addTrajectory( Object* const& obj_ )
{
    SketchScene* sc_ = ( SketchScene* )( tv_main->scene() );
    sc_->addTrajectory( obj_ );
}


void SketchWindow::updateTrajectory( const std::size_t& index_ )
{
    SketchScene* sc_ = ( SketchScene* )( tv_main->scene() );
    sc_->updateTrajectory( index_ );
}





void SketchWindow::setModeSelecting()
{

    SketchScene* sc_ = ( SketchScene* )( main->scene() );
    sc_->setModeSelecting();

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->setModeSelecting();
//    }
}

void SketchWindow::setModeSketching()
{
    SketchScene* sc_ = ( SketchScene* )( main->scene() );
    sc_->setModeSketching();

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->setModeSketching();
//    }
}



void SketchWindow::setCurrentColor( int r_, int g_, int b_ )
{
    cp_color->setColor( QColor( r_, g_, b_ ) );

    if( main != nullptr )
    {
        SketchScene* sc_ = ( SketchScene* )( main->scene() );
        sc_->setCurrentColor( r_, g_, b_ );
    }
    if( tv_main != nullptr )
    {
        SketchScene* sc_ = ( SketchScene* )( tv_main->scene() );
        sc_->setCurrentColor( r_, g_, b_ );
    }
    updateCanvas();
}



//void SketchWindow::addCanvas( CrossSection* const& cs_, bool main_ )
//{

//    if( cs_ == nullptr ) return;

//    QColor color_ = cp_color->currentColor();

//    SketchScene* scene_ = new SketchScene( cs_ );
//    scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() );

//    connect( scene_, &SketchScene::acceptVolumeDimensions, [=]( double w, double h ){ emit updateVolume( cs_->getDirection(), w, h ); } );
//    connect( scene_, &SketchScene::acceptCurve, [=]( const PolyCurve& curve_ ){ emit acceptCurve( curve_ ); } );

//    connect( scene_, &SketchScene::setAsCurrent, [=]( double depth_, QGraphicsView* gview_ ){  emit setAsCurrent( depth_ );
//                                                                                               if( gview_ == main ) highlightCanvas( -1 );
//                                                                                               else highlightCanvas( depth_ ); } );
//    connect( scene_, &SketchScene::objectSelected, [=]( std::size_t index_ ){ emit objectSelected( index_ ); } );
//    connect( scene_, &SketchScene::commitObject, [=](){ emit commitObject(); } );

//    connect( ac_discard, &QAction::triggered, [=](){ emit scene_->discard(); } );
//    connect( ac_commit, &QAction::triggered, [=](){ emit scene_->commit(); } );
//    connect( ac_create, &QAction::triggered, [=](){ emit scene_->create(); } );
//    connect( ac_edit_scene, &QAction::toggled, scene_, &SketchScene::edit );

//    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ ){ if( scene_ == nullptr ) return;  scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );


//    QGraphicsView* gv_ = new QGraphicsView();
//    gv_->setScene( scene_ );

//    if( main_ == true )
//        cs->addElement( -1, gv_ );
//    else
//        cs->addElement( cs_->getDepth(), gv_ );

//    highlightCanvas( cs_ );


//    if( cs_->getDirection() == CrossSection::Direction::Y )
//        setupTopViewWindow();
//    else if( cs_->getDirection() == CrossSection::Direction::Z )
//        setupCrossSectionWindow();
//}


//void SketchWindow::removeCanvas( CrossSection* const& cs_ )
//{
//    if( cs_ == nullptr ) return;
//    if( cs->findElement( cs_->getDepth()  ) == false ) return;


//    QGraphicsView* gview_ = cs->getElement( cs_->getDepth()  );

//    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//    sc_->clear();
//    delete sc_;
//    sc_ = nullptr;

//    cs->removeElement( cs_->getDepth() );
//}


//void SketchWindow::removeCanvas( double depth_ )
//{
//    if( cs->findElement( depth_  ) == false ) return;


//    QGraphicsView* gview_ = cs->getElement( depth_ );
//    if( gview_ == nullptr ) return;

//    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//    if( sc_== nullptr ) return;

//    sc_->clear();
//    delete sc_;
//    sc_ = nullptr;

//    cs->removeElement( depth_ );

//}


//void SketchWindow::highlightCanvas( CrossSection* const& cs_ )
//{
//    if( cs_ == nullptr ) return;


//    double depth_cpy_ = cs_->getDepth();
//    highlightCanvas( depth_cpy_ );

//}


//void SketchWindow::highlightCanvas( double depth_ )
//{
//    double depth_cpy_ = depth_;
//    if( cs->findElement( depth_ ) == false )
//        depth_cpy_ = -1;

//    cs->setCurrent( depth_cpy_ );
//    setCurrentScene( depth_cpy_ );
//}


//void SketchWindow::setMainCanvas( CrossSection* const& cs_ )
//{
//    if( cs_ == nullptr ) return;
//    addCanvas( cs_, true );
//    main = cs->getElement( -1/*cs_->getDepth()*/ );

//}



//void SketchWindow::updateCanvas()
//{

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->updateCrossSection();
//    }

//}



//void SketchWindow::updateVolumes()
//{

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->updateVolume();
//    }

//}


//void SketchWindow::setModeSelecting()
//{
//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->setModeSelecting();
//    }
//}

//void SketchWindow::setModeSketching()
//{
//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->setModeSketching();
//    }
//}



//void SketchWindow::setCurrentScene( CrossSection* const& cs_ )
//{
//    if( cs_ == nullptr ) return;
//    setCurrentScene( cs_->getDepth() );


//}

//void SketchWindow::setCurrentScene( double depth_ )
//{

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->setCurrent( false );
//    }

//    QGraphicsView* gview_;

//    if( cs->findElement( depth_ ) == true )
//        gview_ = cs->getElement( depth_ );
//    else if( cs->findElement( -1 ) == true )
//        gview_ = cs->getElement( -1 );
//    else
//        return;

//    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//    sc_->setCurrent( true );

//}




//void SketchWindow::addObject( Object* const& obj_ )
//{

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->addObject( obj_ );
//    }

//}


//void SketchWindow::updateObject( const std::size_t& index_ )
//{
//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->updateObject( index_ );
//    }


//}



//void SketchWindow::addTrajectory( Object* const& obj_ )
//{
//    SketchScene* sc_ = ( SketchScene* )( main->scene() );
//    sc_->addTrajectory( obj_ );
//}


//void SketchWindow::updateTrajectory( const std::size_t& index_ )
//{
//    SketchScene* sc_ = ( SketchScene* )( main->scene() );
//    sc_->updateTrajectory( index_ );
//}

//void SketchWindow::updateTrajectories()
//{
//    SketchScene* sc_ = ( SketchScene* )( main->scene() );
//    sc_->updateTrajectories();
//}



//void SketchWindow::addCrossSection( CrossSection* const& cs_ )
//{
//    if( cs_ == nullptr ) return;

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = it->second;
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->addCrossSection( cs_ );
//    }

//}


//void SketchWindow::setCurrentCrossSection( const double& value_ )
//{
//    if( main == nullptr ) return;

//    SketchScene* sc_ = ( SketchScene* )( main->scene() );
//    if( sc_ == nullptr ) return;
//    sc_->updateCrossSection();

//    highlightCanvas( value_ );

//}



//void SketchWindow::discardSketch()
//{
//    QGraphicsView* gview_ = cs->getCurrent();
//    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//    emit sc_->discard();

//}


//void SketchWindow::commitSketch()
//{
//    QGraphicsView* gview_ = cs->getCurrent();
//    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//    emit sc_->commit();

//}



//void SketchWindow::createSurface()
//{
//    QGraphicsView* gview_ = cs->getCurrent();
//    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//    emit sc_->create();

//}



//void SketchWindow::setUpColor()
//{
//    if( cp_color->isChecked() == true )
//        return;
//    cp_color->defineRandomColor();
//}

//void SketchWindow::setupCrossSectionWindow()
//{
//    ac_edit_scene->setVisible( true );
//    cp_color->setVisible( true );

//}


//void SketchWindow::clear()
//{

//    while( cs->empty() == false )
//    {
//        CanvasContainer::Iterator it =  cs->begin();
//        removeCanvas( it->first );
//    }

//    main = nullptr;

//}


//void SketchWindow::setupTopViewWindow()
//{
//    ac_edit_scene->setVisible( false );
//    cp_color->setVisible( false );
//    cp_color->setEnabled( false );
//    update();

//}
