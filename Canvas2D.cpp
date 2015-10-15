#include "Canvas2D.h"

Canvas2D::Canvas2D( QWidget *parent ): QGraphicsView( parent )
{

    scene = new SketchingScene();
    this->setScene( scene );

    this->setRenderHint( QPainter::Antialiasing, true );
    this->setOptimizationFlags( QGraphicsView::DontSavePainterState );
    this->setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
    this->setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
    this->setFocusPolicy( Qt::StrongFocus );
    this->setInteractive( true );
    this->setBackgroundRole( QPalette::Base );
    this->setAutoFillBackground( true );


    this->setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    this->viewport()->grabGesture( Qt::PinchGesture );
    this->viewport()->grabGesture( Qt::SwipeGesture );
    this->viewport()->grabGesture( Qt::PanGesture );

    this->setGeometry( this->parentWidget()->geometry() );

}

void Canvas2D::resizeEvent( QResizeEvent* re )
{
    QGraphicsView::resizeEvent( re );
}

void Canvas2D::clear()
{
    scene->clear();
    delete scene;

    scene = new SketchingScene();
    this->setScene( scene );
}
