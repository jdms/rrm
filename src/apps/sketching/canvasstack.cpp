#include "canvasstack.h"

CanvasStack::CanvasStack()
{
    initialize();
}


void CanvasStack::initialize()
{
    hb_mainlayout = new QHBoxLayout( this );
    setLayout( hb_mainlayout );
}


void CanvasStack::addElement( std::size_t id_, QGraphicsView* canvas_ )
{
    bool status = Container::addElement( id_, canvas_ );
    if( status == false ) return;

    hb_mainlayout->addWidget( canvas_ );
    hb_mainlayout->addSpacing( 1 );

    update();
}


void CanvasStack::removeElement( std::size_t id_ )
{
    hb_mainlayout->removeWidget( Container::data[ id_ ] );
    bool status = Container::removeElement( id_ );
    if( status == false ) return;

    update();

}


void CanvasStack::setCurrent( std::size_t id_ )
{
    if( findElement( id_ ) == false ) return;

    int count = hb_mainlayout->count();

    for( auto index = 0; index < count; ++index )
    {
        QWidget* w0 = hb_mainlayout->itemAt( index )->widget();
        if( w0 == nullptr ) continue;
        w0->setStyleSheet( "" );
    }

    QGraphicsView* gv_ = Container::data[ id_ ];
    gv_->setStyleSheet( "border: 2px solid navy" );

    update();
}


QGraphicsView* CanvasStack::getCurrent()
{
    if( findElement( current ) == false ) return ( new QGraphicsView() );
    return Container::data[ current ];
}
