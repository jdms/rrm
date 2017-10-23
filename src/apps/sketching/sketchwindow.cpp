#include "sketchwindow.h"

SketchWindow::SketchWindow( QWidget* parent ): QMainWindow( parent )
{
    createWindow();
}



void SketchWindow::createWindow()
{
    cs = new CanvasStack();
    setCentralWidget( cs );

}



void SketchWindow::addCanvas( CrossSection* const& cs_ )
{
    SketchScene* scene_ = new SketchScene( cs_ );
    cs->addElement( cs_->getIndex(), new QGraphicsView( scene_ ) );

    highlightCanvas( cs_ );

    connect( scene_, &SketchScene::acceptVolumeDimensions, [=]( double w, double h ){ emit updateVolume( cs_->getDirection(), w, h ); } );
    connect( scene_, &SketchScene::acceptCurve, [=]( const PolyCurve& curve_ ){ emit acceptCurve( curve_ ); } );
}


void SketchWindow::removeCanvas( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;
    cs->removeElement( cs_->getIndex() );
}


void SketchWindow::highlightCanvas( CrossSection* const& cs_ )
{
    if( cs_ == nullptr ) return;
    cs->setCurrent( cs_->getIndex() );

    setCurrentScene( cs_ );

}


void SketchWindow::updateCanvas()
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

    QGraphicsView* gview_ = cs->getElement( cs_->getIndex() );
    SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
    sc_->setCurrent( true );

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
