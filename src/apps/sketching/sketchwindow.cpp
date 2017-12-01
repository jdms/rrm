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

    ac_axes = new QAction( "Axes", this );
    ac_axes->setCheckable( true );
    ac_axes->setChecked( true );

    ac_height_map = new QAction( "Map", this );
    connect( ac_height_map, &QAction::triggered, [=](){ emit getHeightMap(); } );


    tb_actions->addWidget( cp_color );
    tb_actions->addSeparator();
    tb_actions->addAction( ac_discard );
    tb_actions->addAction( ac_commit );
    tb_actions->addAction( ac_create );
    tb_actions->addSeparator();
    tb_actions->addAction( ac_edit_scene );
    tb_actions->addSeparator();
    tb_actions->addAction( ac_axes );
    tb_actions->addAction( ac_height_map );

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



    connect( ac_discard, &QAction::triggered, [=](){ if( scene_ == nullptr) return; emit scene_->discard(); } );

    connect( ac_commit, &QAction::triggered, [=](){ if( scene_ == nullptr) return; emit scene_->commit(); } );

    connect( ac_create, &QAction::triggered, [=](){ emit commitObject(); } );

    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ )
                                                    { if( scene_ == nullptr) return; scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );

    connect( ac_axes, &QAction::triggered, scene_, &SketchScene::setAxesVisible );



    connect( scene_, &SketchScene::acceptVolumeDimensions, [=]( Settings::CrossSection::CrossSectionDirections dir_, double w, double h ){ emit updateVolume( dir_, w, h ); } );

    connect( scene_, &SketchScene::acceptCurve, [=]( const PolyCurve& curve_, double depth_ ){ emit acceptCurve( curve_, depth_ ); } );

    connect( scene_, &SketchScene::commitObject, [=](){ emit commitObject(); } );

    connect( scene_, &SketchScene::objectSelected, [=]( std::size_t index_ ){ emit objectSelected( index_ ); } );

    connect( scene_, &SketchScene::setImageCrossSection, [=]( double depth_, const QString& file_, double ox_, double oy_, double scale_ )
                                                            { emit setImageCrossSection( depth_, file_, ox_, oy_, scale_ ); } );



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

    if( tv_main != nullptr )
    {

        SketchScene* tv_main_ = ( SketchScene* )( tv_main->scene() );
        tv_main_->updateVolume();
        tv_main_->updateCrossSection();
    }

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->updateCrossSection();
    }

}




void SketchWindow::addCrossSection( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;
    if( tv_main == nullptr ) return;

    SketchScene* sc_ = ( SketchScene* )( tv_main->scene() );
    sc_->addCrossSection( cs_ );

}




void SketchWindow::addObject( Object* const& obj_ )
{

    if( main == nullptr ) return;

    SketchScene* sc_ = ( SketchScene* )( main->scene() );
    sc_->addObject( obj_ );

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->addObject( obj_ );
    }

}


void SketchWindow::updateObject( const std::size_t& index_ )
{

    if( main == nullptr ) return;

    SketchScene* sc_ = ( SketchScene* )( main->scene() );
    sc_->updateObject( index_ );


    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->updateObject( index_ );
    }
}




void SketchWindow::addTrajectory( Object* const& obj_ )
{
    if( tv_main == nullptr ) return;

    SketchScene* sc_ = ( SketchScene* )( tv_main->scene() );
    sc_->addTrajectory( obj_ );
}


void SketchWindow::updateTrajectory( const std::size_t& index_ )
{
    if( tv_main == nullptr ) return;

    SketchScene* sc_ = ( SketchScene* )( tv_main->scene() );
    sc_->updateTrajectory( index_ );
}





void SketchWindow::setModeSelecting()
{
    if( main == nullptr ) return;

    SketchScene* sc_ = ( SketchScene* )( main->scene() );
    sc_->setModeSelecting();

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->setModeSelecting();
    }
}


void SketchWindow::setModeSketching()
{
    if( main == nullptr ) return;

    SketchScene* sc_ = ( SketchScene* )( main->scene() );
    sc_->setModeSketching();

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->setModeSketching();
    }
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

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->setCurrentColor( r_, g_, b_ );
    }

    updateCanvas();
}





void SketchWindow::clear()
{
    if( main != nullptr )
    {
        SketchScene* sc_main_ = ( SketchScene* )( main->scene() );
        sc_main_->clear();

        delete sc_main_;
        sc_main_ = nullptr;

        delete main;
        main = nullptr;
    }


    if( tv_main != nullptr )
    {
        SketchScene* sc_tv_main_ = ( SketchScene* )( tv_main->scene() );
        sc_tv_main_->clear();

        delete tv_main;
        tv_main = nullptr;
    }


    CanvasContainer::Iterator it =  cs->begin();
    while( it != cs->end() )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->clear();

        cs->removeElement( it->first );
        it =  cs->begin();

    }
    cs->clear();


}




void SketchWindow::addFixedCrossSectionCanvas( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;


    SketchScene* scene_ = new SketchScene( cs_ );

    QColor color_ = cp_color->currentColor();
    scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() );
    scene_->setCurrent( true );

    setupScene( scene_ );

    QGraphicsView* gv_ = new QGraphicsView();
    gv_->scale( 1, -1 );
    gv_->setScene( scene_ );
    cs->addElement( cs_->getDepth(), gv_ );


}


bool SketchWindow::removeFixedCrossSectionCanvas( double depth_ )
{
    if( cs->findElement( depth_  ) == false ) return false;

    QGraphicsView* gview_ = cs->getElement( depth_  );
    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
    sc_->clear();
    delete sc_;
    sc_ = nullptr;

   cs->removeElement( depth_ );
   return true;

}

void SketchWindow::setCurrentCrossSection( double depth_ )
{
    if( cs->findElement( depth_ ) == false ) return;
    cs->setCurrent( depth_ );
}



void SketchWindow::setTopViewImage( const std::string& image_ )
{
    if( tv_main == nullptr ) return;

    SketchScene* sc_ = ( SketchScene* )( tv_main->scene() );
    sc_->setImageToCrossSection( QString( image_.c_str() ) );

}
