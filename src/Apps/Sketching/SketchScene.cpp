#include <QMimeData>
#include <QUrl>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QSvgGenerator>
#include <QFileDialog>

#include "SketchScene.h"

SketchScene::SketchScene( QObject* parent )
{

    csection_image = new QGraphicsPixmapItem();
    current_interaction = UserInteraction::SKETCHING;
    sketch = nullptr;

    surface_selected = 10000;
    current_color = QColor( 255, 0, 0 );
}


void SketchScene::resetData()
{
    for( auto &it: items() )
        removeItem( it );

    if( sketch != nullptr )
    {
        sketch->clear();
        delete sketch;
        sketch = nullptr;
    }


    addItem( csection_image );
    addItem( &volume );
    setSceneRect( volume.boundingRect());

}

void SketchScene::finishSketch()
{
    if ( sketch == nullptr ) return;
    if ( sketch->isEmpty() == true ) return;


    Curve2D curve_;
    if( acceptSketch( curve_ ) == true  )
    {
        Curve2D cpy_curve_;
        curve_.douglasPeuckerSimplify( cpy_curve_, 1.0 );
        emit curveAccepted( cpy_curve_ );
    }

    clearSketch();
}


bool SketchScene::acceptSketch( Curve2D& curve_ )
{
    if ( sketch == nullptr ) return false;

    QPolygonF c_ = sketch->getSketchFunctionGraph();
    curve_ = PolyQtUtils::qPolyginFToCurve2D( c_ );

    return true;
}


void SketchScene::clearSketch()
{
    removeItem( sketch );
    delete sketch;

    sketch = nullptr;
}


void SketchScene::clearScene()
{
    current_interaction = UserInteraction::SKETCHING;


    for( auto &it: items() )
        removeItem( it );


    if( sketch != nullptr )
    {
        sketch->clear();
        delete sketch;
        sketch = nullptr;
    }


    if( csection_image != nullptr )
    {
        delete csection_image;
        csection_image = nullptr;
    }


    for( auto &it: object_list )
    {
        if( (it.second) != nullptr )
        {
            (it.second)->clear();
            delete it.second;
        }
    }
    object_list.clear();


    allowed_objects.clear();
    volume.clear();

    csection_depth = 0.0f;
    surface_selected = 10000;
    boundary_anchor = QPointF( 0.0f, 0.0f );

}


void SketchScene::addObject( Object* obj_ )
{
    ObjectItemWrap* obj_wrapper_;

    auto search = object_list.find( obj_->getId() );
    if( search == object_list.end() )
    {
        obj_wrapper_ = new ObjectItemWrap();        
        obj_wrapper_->setObjectRaw( obj_, csection_depth );
        obj_wrapper_->setState( ObjectItemWrap::State::NONE );

        object_list[ obj_->getId() ] = obj_wrapper_;
    }
    else{
        obj_wrapper_ = object_list[ obj_->getId() ];
        obj_wrapper_->setObjectRaw( obj_, csection_depth );
    }


    addItem( obj_wrapper_ );
    update();
}


void SketchScene::setImagetoCrossSection( const QString& url_ )
{

    QPixmap image_ = QPixmap( url_ );    

    QTransform myTransform;
    myTransform.scale( 1, -1 );
    image_ = image_.transformed( myTransform );

    csection_image->setPixmap( image_ );

//    emit updateVolumeWidthHeight( image_.rect().width(), image_.rect().height() );

    update();
}


void SketchScene::updateScene()
{
    volume.update();
    setSceneRect( volume.boundingRect() );

    for( auto &it_: object_list )
    {
        ObjectItemWrap* obj_= it_.second;
        obj_->updateCrossSection( csection_depth );
    }

    update();
}



void SketchScene::screenshot()
{
    QString selectedFilter;
    QString name_of_file = QFileDialog::getSaveFileName( nullptr, tr( "Save Image" ), "./screenshots/",
                                                         tr( "PNG (*.png);;SVG (*.svg)" ),
                                                         &selectedFilter );

    if( selectedFilter == "PNG (*.png)" )
    {
        savetoRasterImage( name_of_file );
    }
    else if ( selectedFilter == "SVG (*.svg)" )
    {
        savetoVectorImage( name_of_file );
    }
}



void SketchScene::savetoRasterImage( const QString& filename )
{

    QStringList name_and_extension = filename.split('.', QString::SkipEmptyParts );

    QString filename_csection;
    if( name_and_extension.size() > 1 ){
        filename_csection = name_and_extension[ 0 ].append( "_csection." );
        filename_csection.append( name_and_extension[1] );
    }
    else
        filename_csection = filename;


    QImage image( sceneRect().size().toSize(), QImage::Format_ARGB32 );
    image.fill( Qt::transparent );

    QPainter painter( &image );
    render( &painter );

    image = image.mirrored( false, true );
    image.save( filename_csection );

}

void SketchScene::savetoVectorImage( const QString& filename )
{


    QStringList name_and_extension = filename.split('.', QString::SkipEmptyParts );

    QString filename_csection;
    if( name_and_extension.size() > 1 ){
        filename_csection = name_and_extension[ 0 ].append( "_csection." );
        filename_csection.append( name_and_extension[1] );
    }
    else
        filename_csection = filename;



    QSvgGenerator svgGen;

    svgGen.setFileName( filename_csection );
    svgGen.setSize( QSize( width(), height() ) );

    svgGen.setViewBox( sceneRect() );
    svgGen.setTitle( tr( "Rapid Reservoir Modelling - SVG generated by Qt5" ) );
    svgGen.setDescription( tr( "SVG output of Rapid Reservoir Modelling Software" ) );

    QPainter painter( &svgGen );
    painter.scale( 1.0, -1.0 );
    painter.translate( QPointF( 0.0, -height() ) );

    render( &painter );
}





void SketchScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{

    if ( event->buttons() & Qt::LeftButton )
    {
        if( current_interaction == UserInteraction::SKETCHING )
        {
            if( sketch == nullptr )
            {
                sketch = new InputSketch( current_color );
                sketch->create( event->scenePos() );
                addItem( sketch );
            }
        }

        else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
        {
            boundary_anchor = event->buttonDownScenePos( Qt::LeftButton );
        }
    }
    else if ( event->buttons() & Qt::RightButton )
    {
        if( current_interaction == UserInteraction::SKETCHING )
        {
            finishSketch();
        }
    }

    QGraphicsScene::mousePressEvent( event );
    update();
}


void SketchScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

    if ( event->buttons() & Qt::LeftButton )
    {

        if( current_interaction == UserInteraction::SKETCHING )
        {
            if ( sketch == nullptr ) return;
            sketch->add( event->scenePos() );
        }
        else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
        {
            int w_ = event->scenePos().x() - boundary_anchor.x() ;
            int h_ = event->scenePos().y() - boundary_anchor.y() ;

            volume.editGeometry( boundary_anchor.x(), boundary_anchor.y(), std::abs( w_),
                                 std::abs( h_) ) ;
        }

    }

    QGraphicsScene::mouseMoveEvent( event );
    update();


}


void SketchScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if( current_interaction == UserInteraction::SKETCHING )
    {
        if ( sketch == nullptr ) return;
        sketch->process( event->scenePos() );               
    }
    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        double w_ = ( double )std::abs( event->scenePos().x() - boundary_anchor.x() );
        double h_ = ( double )std::abs( event->scenePos().y() - boundary_anchor.y() );

        emit updateVolumeWidthHeight( w_, h_ );
        setModeSketching();
    }
    else if( current_interaction == UserInteraction::SELECTING )
    {

        QList< QGraphicsItem* > items = selectedItems();
        if( items.empty() == false )
        {
            ObjectItemWrap* s = (ObjectItemWrap*) ( items.at( 0 ) );
            s->setState( ObjectItemWrap::State::SELECTED );
            surface_selected = s->getId();

            disallowObjects();

            emit selectedSurface( surface_selected );
            setModeSketching();

        }

    }



    QGraphicsScene::mouseReleaseEvent( event );
    update();
}


void SketchScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
    if( current_interaction == UserInteraction::SKETCHING )
    {
         emit interpolateObject();
        clearSketch();
    }

    QGraphicsScene::mouseDoubleClickEvent( event );
    update();
}




void SketchScene::dragEnterEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}


void SketchScene::dragMoveEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}


void SketchScene::dragLeaveEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}


void SketchScene::dropEvent( QGraphicsSceneDragDropEvent* event )
{

    const QMimeData *mime_data = event->mimeData();

    if ( mime_data->hasUrls() == false )
        return;

    QString url_file = mime_data->urls().at( 0 ).toLocalFile();
    url_file = QDir::toNativeSeparators( url_file );

    setImagetoCrossSection( url_file );
}
