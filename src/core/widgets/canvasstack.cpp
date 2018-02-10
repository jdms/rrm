#include "canvasstack.h"
#include <QDockWidget>
#include <QBitmap>

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

    QDockWidget* dc = new QDockWidget( this );
    dc->setWindowFlags( Qt::Window );
    dc->setWindowTitle( canvas_->windowTitle() );

    connect( dc, &QDockWidget::visibilityChanged, [=]( bool status_ ) { if( dc->isHidden() == true ) emit closeSubWindow( id_ ); } );

    bool status = Container::addElement( id_, dc );
    if( status == false ) return;

    std::cout << "adicionou canvas: " <<std::endl << std::flush;

    dc->setWidget( canvas_ );
    hb_mainlayout->addWidget( dc );
    update();
}


QGraphicsView* CanvasStack::getElement( double id_ )
{
    bool status = Container::findElement( id_ );
    if( status == false ) return nullptr;

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

    QGraphicsView* gv_ = (QGraphicsView*)(Container::data[ id_ ]->widget());
    gv_->setStyleSheet( "border: 2px solid navy" );

    update();
}


QGraphicsView* CanvasStack::getCurrent()
{
    if( findElement( current ) == false ) return ( new QGraphicsView() );
    return (QGraphicsView*)(Container::data[ current ]->widget());
}

void CanvasStack::clear()
{

    while ( Container::data.begin()  != Container::data.end() )
    {
        Container::Iterator it =  Container::data.begin();
        removeElement( it->first );
    }

}
