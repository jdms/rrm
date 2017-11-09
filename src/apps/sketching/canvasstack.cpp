#include "canvasstack.h"

CanvasStack::CanvasStack()
{
    initialize();
}


void CanvasStack::initialize()
{
    hb_mainlayout = new QHBoxLayout( this );
    setLayout( hb_mainlayout );

    current = 0.0;
}


void CanvasStack::addElement( double id_, QGraphicsView* canvas_ )
{
    bool status = Container::addElement( id_, canvas_ );
    if( status == false ) return;

    hb_mainlayout->addWidget( canvas_ );
    hb_mainlayout->addSpacing( 1 );

    update();
}


QGraphicsView* CanvasStack::getElement( double id_ )
{
    bool status = Container::findElement( id_ );
    if( status == false ) return nullptr;

    return Container::getElement( id_ );
}


void CanvasStack::removeElement( double id_ )
{

    if( findElement( id_ ) == false ) return;


    QGraphicsView* canvas_ = Container::data[ id_ ];
    if( canvas_ == nullptr ) return;

    hb_mainlayout->removeWidget( canvas_ );

    bool status = Container::removeElement( id_ );
    if( status == false ) return;

    delete canvas_;
    canvas_ = nullptr;
    update();

}


void CanvasStack::setCurrent( double id_ )
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

void CanvasStack::clear()
{

    while ( Container::data.begin()  != Container::data.end() )
    {
        Container::Iterator it =  Container::data.begin();
        removeElement( it->first );
    }

}
