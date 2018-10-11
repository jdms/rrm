#include "sketchingcanvas.h"

SketchingCanvas::SketchingCanvas( QWidget *parent_ ): QGraphicsView( parent_ )
{
    setInteractive( true );
    setRenderHint( QPainter::Antialiasing );
    setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );
    setSizeAdjustPolicy( QAbstractScrollArea::AdjustToContents );
    setupScene();
}


void SketchingCanvas::setupScene()
{
    scene = std::make_shared< SketchScene >();
    setScene( scene.get() );

    connect( scene.get(), &SketchScene::ensureObjectsVisibility, [=]()
    {
        QRectF rect_ = scene->itemsBoundingRect();
//        scene->setSceneRect(  );
        scene->setSceneRect( rect_.x() - 50, rect_.y() - 50, rect_.width() + 100, rect_.height() + 100 );
        scene->update();
        this->ensureVisible( scene->sceneRect() );
//        this->fitInView( scene->sceneRect() );
    } );

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
