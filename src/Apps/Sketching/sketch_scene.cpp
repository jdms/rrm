#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QDir>
#include <QUrl>
#include <QKeyEvent>
#include <QSvgGenerator>
#include <QCursor>

#include "Core/Geology/Models/object.h"
#include "sketch_scene.h"


SketchScene::SketchScene()
{

    csection_image = nullptr;
    sketch = nullptr;
    object_test = nullptr;
    marker = nullptr;
    image_resizing = nullptr;

    setDefaultValues();

}


void SketchScene::addVolume( Volume* const& vol )
{

    volume.setRawVolume( vol );
    setSceneRect( volume.boundingRect() );
    addItem( &volume );
}


void SketchScene::resizingVolume( const QPointF& point, bool done )
{


    int w = static_cast< int > ( point.x() - boundary_anchor.x() );
    int h = static_cast< int > ( point.y() - boundary_anchor.y() );

    if( done == false )
        volume.resize( boundary_anchor.x(), boundary_anchor.y(), w, h ) ;
    else
    {
        emit updateVolumeDimensions( static_cast< double > ( abs( w ) ),
                                     static_cast< double > ( abs( h ) ) );
        setModeSketching();
    }

}



void SketchScene::updateVolume()
{

    volume.updateItem();
    setSceneRect( volume.boundingRect() );

    axes.setAxisXLenght( volume.getWidth() );
    axes.setAxisYLenght( volume.getHeight() );

    update();
}


bool SketchScene::isAddedObject( std::size_t id ) const
{
    auto search = objects.find( id );
    if( search == objects.end() )
        return false;
    else
        return true;
}



void SketchScene::addObject( Object* const& obj )
{
    if( isAddedObject( obj->getId() ) == true )
    {
        reActiveObject( obj->getId() );
        return;
    }

    ObjectItemWrapper* wrapper = new ObjectItemWrapper();
    wrapper->setRawObject( obj, current_csection );

    objects[ obj->getId() ] = wrapper;
    addItem( wrapper );

    enableDeletingCurves( true );
    update();
}


void SketchScene::updateObject( std::size_t id )
{
    if( isAddedObject( id ) == false ) return;

    ObjectItemWrapper* const& wrapper = objects[ id ];
    wrapper->updateObject();
    update();
}


void SketchScene::reActiveObject( std::size_t id )
{
    if( isAddedObject( id ) == false ) return;

    ObjectItemWrapper* const& wrapper = objects[ id ];
    wrapper->updateDepth( current_csection );

    wrapper->setVisible( true );
    update();
}


void SketchScene::addObjectTest( const std::vector< double >& vertices,
                                            const std::vector< std::size_t >&edges )
{

    QPainterPath curve;

    if( edges.empty() == true )
    {
        std::size_t number_of_vertices = static_cast< std::size_t > ( vertices.size()/2 );

        curve.moveTo( QPointF( vertices[ 0 ], vertices[ 1 ] ) );
        for( std::size_t it = 1; it < number_of_vertices; ++it )
        {
            curve.lineTo( QPointF( vertices[ 2*it ], vertices[ 2*it + 1 ] ) );
        }
    }
    else
    {

        std::size_t nedges = static_cast< std::size_t >( edges.size()/2 );
        std::size_t last_id = 10000;

        for( size_t i = 0; i < nedges; ++i )
        {

            std::size_t id0 = edges[ 2*i ];
            std::size_t id1 = edges[ 2*i + 1 ];

            if( last_id != id0 )
                curve.moveTo( QPointF( vertices[ 2*id0 ],
                                       vertices[ 2*id0 + 1 ] ) );
            else
                curve.lineTo( QPointF( vertices[ 2*id1 ],
                                       vertices[ 2*id1 + 1 ] ) );


            last_id = id1;
        }
    }

    pen_testing.setColor( QColor( current_color.red(), current_color.green(), current_color.blue(),
                                  100 ) );
    pen_testing.setStyle( Qt::DashDotLine );
    pen_testing.setWidth( 2 );
    object_test1.setPath(  curve );
    object_test1.setPen( pen_testing );
    object_test1.setVisible( true );
    update();

}


void SketchScene::removeObjectTest()
{

    object_test1.setVisible( false );
    update();
}



void SketchScene::removeObjectsFromScene()
{

    for( auto it: objects )
    {
        ObjectItemWrapper* const& wrapper = objects[ it.first ];
        wrapper->setVisible( false );
    }

    if( isValidSketch() == true )
        clearSketch();

    removeObjectTest();
    volume.setVisible( true );
//    axes.setVisible( axes_visible );


}



void SketchScene::setModeEditable( bool status )
{
    if( status == false )
    {
        setModeSketching();
        return;
    }

    for( auto it: objects )
    {
        ObjectItemWrapper* const& wrapper = objects[ it.first ];
        wrapper->enableEditing();
    }

    current_interaction = UserInteraction::EDITING_SCENE;
}


void SketchScene::setModeSketching()
{
    current_interaction = UserInteraction::SKETCHING;
    clearSketch();
}


void SketchScene::setModeSelecting()
{
    current_interaction = UserInteraction::SELECTING;
}


void SketchScene::setModeEditingBoundary()
{
    current_interaction = UserInteraction::EDITING_BOUNDARY;
}


void SketchScene::setModeMovingImage()
{
    current_interaction = UserInteraction::MOVING_IMAGE;
    csection_image->setFlag( QGraphicsItem::ItemIsMovable, true );
}


void SketchScene::disableMovingImage()
{

    if( hasImageInCrossSection() == false ) return;

    csection_image->setFlag( QGraphicsItem::ItemIsMovable, false );
    csection_image->setFlag( QGraphicsItem::ItemIsSelectable, false );

    ImageData& image_data = backgrounds[ current_csection ];
    image_data.origin = csection_image->scenePos();

    setModeSketching();
}


void SketchScene::setModeResizingImage( bool status )
{

    if( status == true )
    {
        if( hasImageInCrossSection() == false || csection_image == nullptr ) return;

        marker = addEllipse( 0, 0, 25, 25, QPen( Qt::blue ) );
        marker->setPos( csection_image->sceneBoundingRect().topRight().x()-25*0.5,
                        csection_image->sceneBoundingRect().topRight().y()-25*0.5 );
        current_interaction = UserInteraction::RESIZING_IMAGE;
    }
    else
    {


        if( marker != nullptr )
        {
            removeItem( marker );
            delete marker;
            marker = nullptr;
            current_interaction = UserInteraction::SKETCHING;
        }

    }
}


void SketchScene::enableDeletingCurves( bool status )
{
    is_delete_enabled = status;
    emit enableDeleting( status );
}


void SketchScene::setCurrentColor( const QColor& color )
{
    current_color = color;
    pen_testing.setColor( color );

    if( isValidSketch() == true )
        sketch->setColor( color );

    if( object_test != nullptr )
        object_test->setPen( pen_testing );

    emit updateColor( color );
    update();

}


void SketchScene::setCurrentColor( int r, int g, int b )
{
    setCurrentColor( QColor( r, g, b ) );
}




void SketchScene::startSketch( const QPointF& p )
{

    if( ( isValidSketch() == true ) || ( is_sketch_enabled == false ) ) return;

    sketch = new InputSketch( current_color );
    sketch->create( p );
    addItem( sketch );

    update();
}


void SketchScene::clearSketch()
{
    if( isValidSketch() == false ) return;

    removeItem( sketch );
    delete sketch;
    sketch = nullptr;

    update();
}


void SketchScene::finishSketch()
{

    if( isValidSketch() == false ) return;
    if( sketch->isEmpty() == true ) return;

    Curve2D curve;
    if( acceptSketch( curve ) == true  )
    {
        emit addCurveToObject( curve );
    }

    clearSketch();

    update();
}


bool SketchScene::acceptSketch( Curve2D& curve )
{
    if( sketch->isEmpty() == true ) return false;

    QPolygonF c = sketch->getSketchFunctionGraph();
    curve = PolyQtUtils::qPolyginFToCurve2D( c );
    processCurve( curve );

    return true;

}


void SketchScene::enableSketch( bool status )
{
    if( status == false )
        clearSketch();

    is_sketch_enabled = status;
}


bool SketchScene::isValidSketch()
{
    if ( sketch == nullptr ) return false;
    return true;
}




void SketchScene::removeCurve()
{

    if( ( current_interaction != UserInteraction::EDITING_SCENE ) ||
        ( is_delete_enabled == false ) ) return;

    QList < QGraphicsItem* > items = selectedItems();
    if( items.empty() == true ) return;

    emit removeCurveFromObject( current_csection );
    update();
}


void SketchScene::selectBounderingRegion()
{
    QList< QGraphicsItem* > items = selectedItems();
    if( items.empty() == true ) return;


    ObjectItemWrapper* const& obj = ( ObjectItemWrapper* )items[ 0 ];
    std::size_t id = obj->getId();

    emit selectedObject( id );

}



void SketchScene::processCurve( Curve2D& curve )
{
    Curve2D cpy = curve;
    cpy.lineFilter();
    cpy.meanFilter();
    cpy.meanFilter();
    curve = cpy;
}


void SketchScene::setCurrentCrossSection( double depth )
{

    current_csection = depth;

    removeObjectsFromScene();

    if( hasImageInCrossSection() == false )
    {
        removeImageFromCrossSection();        
    }
    else
        setImageToCrossSection();


}




void SketchScene::createCrossSectionImageItem()
{
    csection_image = new QGraphicsPixmapItem();
    addItem( csection_image );

    update();
}


void SketchScene::setImageToCrossSection( const QString& file )
{

    QPixmap image;
    image.load( file );

    if( image.isNull() == true ) return;

    QTransform myTransform;
    myTransform.scale( 1, -1 );
    image = image.transformed( myTransform );

    csection_image->setPixmap( image );
    csection_image->setPos( QPointF( 0, 0 ) );
    csection_image->setVisible( true );
    csection_image->update();


    ImageData image_data;
    image_data.file = file;
    image_data.origin = csection_image->scenePos();
    image_data.scale = csection_image->scale();
    backgrounds[ current_csection ] = image_data;



    enableResizingImage( true );
    update();
}


void SketchScene::setImageToCrossSection()
{

    const ImageData& image_data = backgrounds[ current_csection ];

    QPixmap image;
    image.load( image_data.file );

    if( image.isNull() == true ) return;

    QTransform myTransform;
    myTransform.scale( 1, -1 );
    image = image.transformed( myTransform );


    csection_image->setPixmap( image );
    csection_image->setPos( image_data.origin );
    csection_image->setScale( image_data.scale );
    csection_image->setVisible( true );
    csection_image->update();


    enableResizingImage( true );

    update();

}


void SketchScene::resizingCrossSectionImage( const QPointF& p )
{

    if( marker == nullptr || csection_image == nullptr ) return;
    if( std::abs( p.y() - marker->y() ) > marker->boundingRect().width() ) return;


    float aratio = ( p.x() - csection_image->pos().x() )/csection_image->boundingRect().width();
    marker->setX( p.x() - marker->boundingRect().width()*0.5f );

    csection_image->setScale( aratio );

    ImageData& image_data = backgrounds[ current_csection ];
    image_data.scale = aratio;

}


void SketchScene::removeImageFromCrossSection()
{

    csection_image->setVisible( false );

    if( hasImageInCrossSection() == true )
        backgrounds.erase( current_csection );

    if( marker != nullptr )
    {
        removeItem( marker );
        delete marker;
        marker = nullptr;
    }
    enableResizingImage( false );

    update();
}


bool SketchScene::hasImageInCrossSection()
{
    auto search = backgrounds.find( current_csection );
    if( search == backgrounds.end() )
        return false;
    else
        return true;
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
    painter.end();

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
    painter.end();
}



void SketchScene::enableAxes( bool status )
{
    is_axes_enable = status;
    update();
}


void SketchScene::clear()
{

    clearData();
    setDefaultValues();
    update();
}


void SketchScene::clearData()
{


    for( auto &it: items() )
        removeItem( it );

    if( csection_image != nullptr )
        delete csection_image;
    csection_image = nullptr;

    volume.clear();
    clearSketch();

    for( auto& it : objects )
    {
        ( it.second )->clear();
        delete ( it.second );
    }
    objects.clear();

    backgrounds.clear();
}


void SketchScene::setDefaultValues()
{
    current_csection = 0.0;
    current_color = Qt::red;
    current_interaction = DEFAULT_INTERACTION;

    boundary_anchor = QPointF( 0.0, 0.0 );

    is_sketch_enabled = true;
    is_delete_enabled = false;
    is_axes_enable = true;

    addItem( &axes );

    createCrossSectionImageItem();

    object_test1.setVisible( false );
    addItem( &object_test1 );

}



void SketchScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{


    if(  event->modifiers() & Qt::ControlModifier )
    {
        if( csection_image->isUnderMouse() == true )
            setModeMovingImage();
    }

    else if ( ( event->buttons() & Qt::LeftButton ) &&
              ( current_interaction == UserInteraction::SKETCHING ) )
    {
        startSketch( event->scenePos() );
    }

    else if ( ( event->buttons() & Qt::RightButton ) &&
              ( current_interaction == UserInteraction::SKETCHING ) )
    {
        finishSketch();
    }


    else if( ( event->buttons() & Qt::LeftButton ) &&
             ( current_interaction == UserInteraction::EDITING_BOUNDARY ) )
    {
        boundary_anchor = event->scenePos();
    }
    
    QGraphicsScene::mousePressEvent( event );
    update();
}


void SketchScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

    if ( ( event->buttons() & Qt::LeftButton ) &&
         ( current_interaction == UserInteraction::SKETCHING ) &&
         ( isValidSketch() == true ) )
    {
            sketch->add( event->scenePos() );
    }

    else if( ( event->buttons() & Qt::LeftButton ) &&
             ( current_interaction == UserInteraction::EDITING_BOUNDARY ) )
    {
        resizingVolume( event->scenePos() );
    }

    else if( ( event->buttons() & Qt::LeftButton ) &&
             ( current_interaction == UserInteraction::RESIZING_IMAGE ) )
    {
        resizingCrossSectionImage( event->scenePos() );

    }

    QGraphicsScene::mouseMoveEvent( event );
    update();
}


void SketchScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if( ( current_interaction == UserInteraction::SKETCHING ) &&
            ( isValidSketch() == true ) )
    {
        sketch->process( event->scenePos() );
    }

    else if( current_interaction == UserInteraction::MOVING_IMAGE )
    {
        disableMovingImage();
    }

    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        bool done = true;
        resizingVolume( event->scenePos(), done );
    }

    else if( current_interaction == UserInteraction::SELECTING )
    {
        selectBounderingRegion();
    }


    QGraphicsScene::mouseReleaseEvent( event );
    update();
}



void SketchScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
    if( current_interaction == UserInteraction::SKETCHING )
    {
         emit createSurface();
//        clearSketch();
    }

    QGraphicsScene::mouseDoubleClickEvent( event );
    update();
}




void SketchScene::dragEnterEvent( QGraphicsSceneDragDropEvent* event )
{
    if( ( event->mimeData()->hasUrls() == true ) && ( event->mimeData()->hasImage() ) )
    {
            event->acceptProposedAction();
    }

}


void SketchScene::dropEvent( QGraphicsSceneDragDropEvent* event )
{
    const QMimeData *mime_data = event->mimeData();
    QString url_file = mime_data->urls().at( 0 ).toLocalFile();
    url_file = QDir::toNativeSeparators( url_file );

    setImageToCrossSection( url_file );

}


void SketchScene::dragMoveEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}



void SketchScene::keyPressEvent( QKeyEvent *event )
{
    switch( event->key() )
    {
        case Qt::Key_Delete:
            removeCurve();
            break;
        default:
            break;
    };
}
