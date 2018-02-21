#include "canvasstack.h"
#include <QDockWidget>
#include <QBitmap>
#include <QStyleOptionDockWidget>
#include <QAction>

CanvasStack::CanvasStack( QWidget* parent ): QWidget( parent )
{
    initialize();
}


void CanvasStack::initialize()
{

    setWindowFlags(Qt::Widget);

    hb_mainlayout = new QSplitter( this );
    hb_mainlayout->setOrientation( Qt::Vertical );

    QHBoxLayout* hb_main = new QHBoxLayout();
    hb_main->addWidget( hb_mainlayout );

    setLayout( hb_main );

    current = 0.0;
}


void CanvasStack::addElement( double id_, QGraphicsView* canvas_ )
{

    DockWidget* dc = new DockWidget( this );
    dc->setAllowedAreas(Qt::AllDockWidgetAreas);
    dc->setGeometry( 100, 100, canvas_->rect().width(), canvas_->rect().height() );
    dc->setWindowTitle( canvas_->windowTitle() );
    dc->setWidget( canvas_ );
    dc->updateGeometry();

    bool status = Container::addElement( id_, dc );
    if( status == false ) return;

    hb_mainlayout->addWidget( dc );

    connect( dc, &DockWidget::closeDockWidget, [=]() { emit closeSubWindow( id_ ); } );



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

    update();

}


void CanvasStack::setCurrent( double id_ )
{
    if( findElement( id_ ) == false ) return;

    Container::Iterator it =  Container::data.begin();
    while( it != Container::data.end() )
    {
        QGraphicsView* gv_ = (QGraphicsView*)(Container::data[ it->first ]->widget());
        gv_->setStyleSheet( "" );
        it++;
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

void CanvasStack::mousePressEvent(QMouseEvent *event)
{

    QWidget::mousePressEvent( event );
}

void CanvasStack::mouseReleaseEvent(QMouseEvent *event)
{

    QWidget::mouseReleaseEvent( event );
}

void CanvasStack::mouseDoubleClickEvent(QMouseEvent *event)
{

    QWidget::mouseDoubleClickEvent( event );
}
