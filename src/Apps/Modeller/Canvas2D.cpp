#include "Modeller/Canvas2D.h"

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

//    this->setGeometry( this->parentWidget()->geometry() );
    this->setMinimumSize( 600, 600 );


    connect( parent, SIGNAL( applyremoveabove() ), scene, SLOT( applyRemoveAbove() ) );
    connect( parent, SIGNAL( applyremovebelow() ), scene, SLOT( applyRemoveBelow() ) );
    connect( parent, SIGNAL( setColor( int, int, int ) ), scene, SLOT( setColor( int, int, int ) ) );
    connect( parent, SIGNAL( selectMode() ), scene, SLOT( setModeSelect() ) );
    connect( parent, SIGNAL( sketchingMode() ), scene, SLOT( setModeSketching() ) );

}

Canvas2D::~Canvas2D()
{
    scene->clear();
    delete scene;
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
