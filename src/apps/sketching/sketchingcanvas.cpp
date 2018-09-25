#include "sketchingcanvas.h"

SketchingCanvas::SketchingCanvas( QWidget *parent_ ): QGraphicsView( parent_ )
{
    setInteractive( true );
    setRenderHint( QPainter::Antialiasing );
    setupScene();
}


void SketchingCanvas::setupScene()
{
    scene = std::make_shared< SketchScene >();
    setScene( scene.get() );

    connect( scene.get(), &SketchScene::ensureObjectsVisibility, [=](){  scene->setSceneRect( scene->itemsBoundingRect() ); scene->update(); } );

}


const std::shared_ptr< SketchScene >& SketchingCanvas::getScene() const
{
    return scene;
}



void SketchingCanvas::setVerticalExaggeration( double scale_ )
{
    v_exag = scale_;
    QMatrix matrix_;
    matrix_.scale( 1.0, -1*scale_ );
    setMatrix( matrix_ );

}


double SketchingCanvas::getVerticalExaggeration() const
{
   return v_exag;
}



SketchingCanvas::~SketchingCanvas()
{

//    scene.reset();
//    scene = nullptr;
}
