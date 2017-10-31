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
    setCentralWidget( cs );

}



void SketchWindow::addCanvas( CrossSection* const& cs_ )
{
    SketchScene* scene_ = new SketchScene( cs_ );
    cs->addElement( cs_->getDepth(), new QGraphicsView( scene_ ) );

    highlightCanvas( cs_ );

    connect( scene_, &SketchScene::acceptVolumeDimensions, [=]( double w, double h ){ emit updateVolume( cs_->getDirection(), w, h ); } );
    connect( scene_, &SketchScene::acceptCurve, [=]( const PolyCurve& curve_ ){ emit acceptCurve( curve_ ); } );

    connect( ac_discard, &QAction::triggered, [=](){ emit scene_->discard(); } );
    connect( ac_commit, &QAction::triggered, [=](){ emit scene_->commit(); } );
    connect( ac_create, &QAction::triggered, [=](){ emit scene_->create(); } );
    connect( ac_edit_scene, &QAction::toggled, scene_, &SketchScene::edit );

    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ ){ scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );


}


void SketchWindow::removeCanvas( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;
    cs->removeElement( cs_->getDepth() );
}


void SketchWindow::highlightCanvas( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;
    cs->setCurrent( cs_->getDepth() );

    setCurrentScene( cs_ );

}


void SketchWindow::setMainCanvas( CrossSection* const& cs_ )
{
    addCanvas( cs_ );
    main = cs->getElement( cs_->getDepth() );

}



void SketchWindow::updateCanvas()
{

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->updateCrossSection();
    }

}




void SketchWindow::updateVolumes()
{

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->updateVolume();
    }

}




void SketchWindow::setCurrentScene( CrossSection* const& cs_ )
{
    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->setCurrent( false );
    }

    QGraphicsView* gview_ = cs->getElement( cs_->getDepth() );
    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
    sc_->setCurrent( true );

}



void SketchWindow::addObject( Object* const& obj_ )
{

    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->addObject( obj_ );
    }

}


void SketchWindow::updateObject( const std::size_t& index_ )
{
    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
    {
        QGraphicsView* gview_ = it->second;
        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
        sc_->updateObject( index_ );
    }


}


void SketchWindow::setCurrentCrossSection( const double& value_ )
{
    SketchScene* sc_ = ( SketchScene* )( main->scene() );
    sc_->updateCrossSection();
}


void SketchWindow::discardSketch()
{
    QGraphicsView* gview_ = cs->getCurrent();
    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
    emit sc_->discard();

}


void SketchWindow::commitSketch()
{
    QGraphicsView* gview_ = cs->getCurrent();
    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
    emit sc_->commit();

}

void SketchWindow::createSurface()
{
    QGraphicsView* gview_ = cs->getCurrent();
    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
    emit sc_->create();

}


void SketchWindow::setUpColor()
{
    if( cp_color->isChecked() == true )
        return;
    cp_color->defineRandomColor();
}
