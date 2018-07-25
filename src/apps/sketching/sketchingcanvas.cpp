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

SketchingCanvas::~SketchingCanvas()
{

//    scene.reset();
//    scene = nullptr;
}
