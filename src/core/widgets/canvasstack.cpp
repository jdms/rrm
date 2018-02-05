#include "canvasstack.h"
#include <QDockWidget>

CanvasStack::CanvasStack()
{
    initialize();
}


void CanvasStack::initialize()
{
    hb_mainlayout = new QSplitter( this );
    hb_mainlayout->setOrientation( Qt::Vertical );

    QHBoxLayout* hb_main = new QHBoxLayout();
    hb_main->addWidget( hb_mainlayout );

    setLayout( hb_main );

    current = 0.0;
}


void CanvasStack::addElement( double id_, QGraphicsView* canvas_ )
{
//    bool status = Container::addElement( id_, canvas_ );
//    if( status == false ) return;

    std::cout << "vou adicionar canvas: " << id_ << std::endl << std::flush;

    QDockWidget* dc = new QDockWidget();
    connect( dc, &QDockWidget::visibilityChanged, [=]( bool status_ ) { if( dc->isHidden() == true ) emit closeSubWindow( id_ ); } );

    bool status = Container::addElement( id_, dc );
    if( status == false ) return;

    std::cout << "adicionou canvas: " <<std::endl << std::flush;

    dc->setWidget( canvas_ );
    hb_mainlayout->addWidget( dc );
//    hb_mainlayout->addWidget( canvas_ );
    update();
}


QGraphicsView* CanvasStack::getElement( double id_ )
{
    bool status = Container::findElement( id_ );
    if( status == false ) return nullptr;

//    return Container::getElement( id_ );
    return (QGraphicsView*)(Container::data[ id_ ]->widget());
}


void CanvasStack::removeElement( double id_ )
{


    if( findElement( id_ ) == false ) return;

    QGraphicsView* canvas_ = (QGraphicsView*)(Container::data[ id_ ]->widget() );
    if( canvas_ == nullptr ) return;

    delete canvas_;
    canvas_ = nullptr;

    bool status = Container::removeElement( id_ );
    if( status == false ) return;

    deleteElement( id_ );

    std::cout << "removeu canvas!" << std::endl;


//    QGraphicsView* canvas_ = Container::data[ id_ ];
//    if( canvas_ == nullptr ) return;

////    hb_mainlayout->removeWidget( canvas_ );

//    bool status = Container::removeElement( id_ );
//    if( status == false ) return;

    update();

}


void CanvasStack::setCurrent( double id_ )
{
    if( findElement( id_ ) == false ) return;

    int count = hb_mainlayout->count();

    for( auto index = 0; index < count; ++index )
    {
        QWidget* w0 = hb_mainlayout->widget( index );
        if( w0 == nullptr ) continue;
        w0->setStyleSheet( "" );
    }

//    QGraphicsView* gv_ = Container::data[ id_ ];
    QGraphicsView* gv_ = (QGraphicsView*)(Container::data[ id_ ]->widget());
    gv_->setStyleSheet( "border: 2px solid navy" );

    update();
}


QGraphicsView* CanvasStack::getCurrent()
{
    if( findElement( current ) == false ) return ( new QGraphicsView() );
    return (QGraphicsView*)(Container::data[ current ]->widget());

//    if( findElement( current ) == false ) return ( new QGraphicsView() );
//    return Container::data[ current ];
}

void CanvasStack::clear()
{

    while ( Container::data.begin()  != Container::data.end() )
    {
        Container::Iterator it =  Container::data.begin();
        removeElement( it->first );
    }

}
