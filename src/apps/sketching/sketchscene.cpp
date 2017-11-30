#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QPixmap>
#include <QDir>
#include <QUrl>
#include <QMimeData>

#include "sketchscene.h"



SketchScene::SketchScene()
{
    user_input = new InputSketch();
    addItem( user_input );

    csection_image = new QGraphicsPixmapItem();
    addItem( csection_image );

    axes.setVisible( true );
    addItem( &axes );

    volume = nullptr;
}


SketchScene::SketchScene( CrossSection* const& raw_ ):csection( raw_ ), volume( nullptr )
{
    readCrossSection( raw_ );

    user_input = new InputSketch();
    user_input->setCurrentColor( QColor( current_color.red, current_color.green, current_color.blue ) );
    addItem( user_input );

    csection_image = new QGraphicsPixmapItem();
    addItem( csection_image );

    axes.setVisible( true );
    addItem( &axes );


    InputSketch::Direction dir_;
    if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
        dir_ = InputSketch::Direction::Z;
    else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Y )
        dir_ = InputSketch::Direction::Y;
    else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
        dir_ = InputSketch::Direction::X;


    connect( this, &SketchScene::discard, [=](){ user_input->clear(); update(); } );
    connect( this, &SketchScene::commit, [=](){ emit acceptCurve( user_input->done( dir_ ), csection->getDepth() ); } );
    connect( this, &SketchScene::create, [=](){ emit commitObject(); } );
}



void SketchScene::edit( bool status_ )
{

    if( status_ == false )
        setModeSketching();
    else
        setModeEditingScene();

    for ( ObjectsContainer::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        ObjectItemWrapper* obj_ = ( it->second );
        obj_->updateState();
    }


    update();


//    if( obj_ == nullptr ) return;

//    if( obj_->type() == QGraphicsPixmapItem::Type )
//        std::cout << "pixmap type: " << pixmap_->type() << std::endl << std::flush;
//    else if( obj_->type() == QGraphicsPathItem::Type )
//        std::cout << "path type: " << path_->type() << std::endl << std::flush;


}


void SketchScene::setAxesVisible( bool status_ )
{
    axes.setVisible( status_ );
    update();
}


void SketchScene::readCrossSection( CrossSection* const& raw_ )
{
    if( raw_ == nullptr ) return;

    csection = raw_;



    Volume* vol_ = const_cast< Volume* >( raw_->getVolume() );


    if( raw_->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
        createCrossSectionScene( vol_ );

    else if( raw_->getDirection() == Settings::CrossSection::CrossSectionDirections::Y )
        createTopViewScene( vol_ );



}


void SketchScene::createCrossSectionScene( Volume* const& vol_ )
{
    addVolume( vol_, Settings::CrossSection::CrossSectionDirections::Z );

    axes.setPlane( CoordinateAxes2d::Plane::XY );
    axes.setAxisXLenght( vol_->getWidth() );
    axes.setAxisYLenght( vol_->getHeight() );

    Volume::ObjectsContainer objs_ = vol_->getObjects();
    for( auto o: objs_ )
        addObject( o.second );
}


void SketchScene::createTopViewScene( Volume* const& vol_ )
{
    addVolume( vol_, Settings::CrossSection::CrossSectionDirections::Y );

    axes.setPlane( CoordinateAxes2d::Plane::XZ );
    axes.setAxisXLenght( vol_->getWidth() );
    axes.setAxisYLenght( vol_->getHeight() );

    Volume::CrossSectionsContainer csections_ = vol_->getCrossSections();
    for( auto c: csections_ )
    {
        addCrossSection( c.second );
    }


    Volume::ObjectsContainer objs_ = vol_->getObjects();
    for( auto o: objs_ )
        addTrajectory( o.second );


}


void SketchScene::setImageToCrossSection( const QString& file_, double ox_, double oy_, double scale_ )
{

    QPixmap image;
    image.load( file_ );

    if( image.isNull() == true ) return;

    QTransform myTransform;
    myTransform.scale( 1, -1 );
    image = image.transformed( myTransform );

    csection_image->setPixmap( image );
    csection_image->setPos( QPointF( ox_, oy_ ) );
    csection_image->setVisible( true );
    csection_image->setScale( scale_ );
    csection_image->update();


    emit setImageCrossSection( csection->getDepth(), file_, ox_, oy_, scale_ );

//    ImageData image_data;
//    image_data.file = file;
//    image_data.origin = csection_image->scenePos();
//    image_data.scale = csection_image->scale();
//    backgrounds[ current_csection ] = image_data;



//    enableResizingImage( true );
    update();
}


void SketchScene::updateCrossSection()
{

    std::cout << "Cross-section being updated: " << csection->getDepth() << std::endl << std::flush;

    Volume* const& vol_ = const_cast< Volume* >( csection->getVolume() );

    std::cout << "Volume dimensions: ( " << vol_->getWidth() << ", " << vol_->getHeight() << ", "
              << vol_->getLenght() << ") " << std::endl << std::flush;


    if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
        updateCrossSectionScene();

    else if( csection->getDirection() ==Settings::CrossSection::CrossSectionDirections::Y )
        updateTopViewScene();

}



void SketchScene::updateCrossSectionScene()
{

    if( csection->hasImage() == true )
    {
        std::string path_;
        double ox_;
        double oy_;
        double scale_;
        csection->getImage( path_, ox_, oy_, scale_ );
        setImageToCrossSection( QString( path_.c_str() ), ox_, oy_, scale_ );
    }
    else
    {
        csection_image->setVisible( false );
        csection_image->update();
    }

    Volume* const& vol_ = const_cast< Volume* >( csection->getVolume() );
    Volume::ObjectsContainer objs_ = vol_->getObjects();

    for( auto o: objs_ )
        updateObject( o.first );


    update();
}


void SketchScene::updateTopViewScene()
{

    Volume* const& vol_ = const_cast< Volume* >( csection->getVolume() );
    Volume::ObjectsContainer objs_ = vol_->getObjects();

    for( auto o: objs_ )
        updateTrajectory( o.first );

}




void SketchScene::addVolume( Volume* const &raw_, Settings::CrossSection::CrossSectionDirections dir_ )
{
    clearVolume();
    volume = new VolumeItemWrapper( raw_, dir_ );

    addItem( volume );
    setSceneRect( volume->boundingRect() );
    update();
}


void SketchScene::updateVolume()
{

    volume->updateItem();

    axes.setAxisXLenght( volume->getWidth() );
    axes.setAxisYLenght( volume->getHeight() );


    setSceneRect( volume->boundingRect() );
    update();
}


void SketchScene::clearVolume()
{
    if( volume != nullptr )
        delete volume;
    volume = nullptr;
}




void SketchScene::addObject( Object* const& raw_ )
{
    addObject( raw_, csection->getDepth() );
}


void SketchScene::addObject( Object* const& raw_, double depth_ )
{

    if( csection->getDirection() != Settings::CrossSection::CrossSectionDirections::Z ) return;

    std::size_t index_ = raw_->getIndex();

    ObjectItemWrapper* obj_ = new ObjectItemWrapper( raw_, depth_ );
    objects.addElement( index_, obj_ );

    addItem( obj_ );

}


void SketchScene::updateObject(  const std::size_t& index_ )
{
    if( csection->getDirection() != Settings::CrossSection::CrossSectionDirections::Z ) return;

    ObjectItemWrapper* obj_ = objects.getElement( index_ );

    obj_->updateDepth( csection->getDepth() );
    obj_->updateObject();


    update();
}


void SketchScene::selectObjectAsBounderingRegion()
{
    QList< QGraphicsItem* > items = selectedItems();
    if( items.empty() == true ) return;


    ObjectItemWrapper* const& obj = ( ObjectItemWrapper* )items[ 0 ];
    std::size_t id = obj->getIndex();

    emit objectSelected( id );

}



void SketchScene::addTrajectory( Object* const& raw_ )
{

    std::size_t index_ = raw_->getIndex();

    TrajectoryItemWrapper* traj_ = new TrajectoryItemWrapper( raw_ );
    trajectories.addElement( index_, traj_ );

    addItem( traj_ );

}


void SketchScene::updateTrajectory(  const std::size_t& index_ )
{
    TrajectoryItemWrapper* traj_ = trajectories.getElement( index_ );
    traj_->updateTrajectory();

    update();
}


void SketchScene::updateTrajectories()
{
    for ( TrajectoriesContainer::Iterator it =  trajectories.begin(); it != trajectories.end(); ++it )
    {
        TrajectoryItemWrapper* item_ = trajectories.getElement( it->first );
        if( item_ == nullptr ) continue;
        item_->updateTrajectory();
    }
    update();
}


void SketchScene::addCrossSection( CrossSection * const &raw_ )
{

    Volume* const& vol_ = const_cast< Volume* >( raw_->getVolume() );

    CrossSectionItemWrapper* csection_ = new CrossSectionItemWrapper( vol_->getWidth(), raw_->getDepth() );
    cross_sections.addElement( raw_->getIndex(), csection_ );

    addItem( csection_ );
}



void SketchScene::setCurrent( bool status_ )
{
    is_current = status_;
}


bool SketchScene::isCurrent() const
{
    return is_current;
}



void SketchScene::setCurrentColor( int r, int g, int b )
{
    if( user_input == nullptr ) return;

    user_input->setCurrentColor( QColor( r, g, b ) );
    current_color.red   = r;
    current_color.green = g;
    current_color.blue  = b;

}


void SketchScene::getCurrentColor( int& r, int& g, int& b )
{
    if( user_input == nullptr ) return;
    r = current_color.red;
    g = current_color.green;
    b = current_color.blue;
}



void SketchScene::setModeSketching()
{
    user_input->clear();
    current_interaction = UserInteraction::SKETCHING;
}


void SketchScene::setModeEditingBoundary()
{
    current_interaction = UserInteraction::EDITING_BOUNDARY;
}


void SketchScene::setModeEditingScene()
{
    current_interaction = UserInteraction::EDITING_SCENE;
}


void SketchScene::setModeSelecting()
{
    current_interaction = UserInteraction::SELECTING;
}



void SketchScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{

    if( is_current == false ) return;


    if( ( event->buttons() & Qt::LeftButton ) &&
        ( current_interaction == UserInteraction::SKETCHING ) )
    {
        user_input->create(  event->scenePos() );
    }

    else if( ( event->buttons() & Qt::RightButton ) &&
        ( current_interaction == UserInteraction::SKETCHING ) )
    {
        if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
            emit acceptCurve( user_input->done( InputSketch::Direction::Z ), csection->getDepth() );
        else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Y )
            emit acceptCurve( user_input->done( InputSketch::Direction::Y  ), csection->getDepth() );
        else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
            emit acceptCurve( user_input->done( InputSketch::Direction::X  ), csection->getDepth() );
    }

    else if( ( event->buttons() & Qt::LeftButton ) &&
        ( current_interaction == UserInteraction::EDITING_BOUNDARY ) )
    {
        volume->startPoint( event->scenePos() );
    }

    QGraphicsScene::mousePressEvent( event );
    update();
}


void SketchScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
    if( event->modifiers() & Qt::ControlModifier )
    {
        if( csection == nullptr ) return;
        if( views().empty() == true ) return;
        emit setAsCurrent( csection->getDepth(), views()[ 0 ] );
    }

    else if( current_interaction == UserInteraction::SKETCHING )
    {
         emit commitObject();
    }

}


void SketchScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

    if( is_current == false ) return;

    if( ( event->buttons() & Qt::LeftButton ) &&
        ( current_interaction == UserInteraction::SKETCHING ) )
    {
        user_input->add(  event->scenePos() );
    }
    else if( ( event->buttons() & Qt::LeftButton ) &&
        ( current_interaction == UserInteraction::EDITING_BOUNDARY ) )
    {
        volume->resize( event->scenePos() );
    }

    QGraphicsScene::mouseMoveEvent( event );
    update();

}


void SketchScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if( is_current == false ) return;

    if( current_interaction == UserInteraction::SKETCHING )
    {
        user_input->process();
    }
    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        emit acceptVolumeDimensions( csection->getDirection(), static_cast< double >( volume->boundingRect().width() ),
                                     static_cast< double >( volume->boundingRect().height() ) );
    }

    else if( current_interaction == UserInteraction::SELECTING )
    {
        selectObjectAsBounderingRegion();
    }


    QGraphicsScene::mouseReleaseEvent( event );
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



void SketchScene::wheelEvent( QGraphicsSceneWheelEvent *event )
{

    if( views().isEmpty() == true ) return;

    QGraphicsView* gv_ = views()[ 0 ];

    if( event->delta() > 0 )
        gv_->scale( ZOOM_SCALE, ZOOM_SCALE );
    else
        gv_->scale( 1.0/ZOOM_SCALE, 1.0/ZOOM_SCALE );


    QGraphicsScene::wheelEvent( event );
    update();
}



void SketchScene::clear()
{

    for( auto &it: items() )
        removeItem( it );


    if( user_input != nullptr )
    {
        user_input->clear();
        delete user_input;
        user_input = nullptr;
    }


    if( csection != nullptr )
    {
        csection = nullptr;
    }


    if( volume != nullptr )
    {
        volume->clear();
        delete volume;
        volume = nullptr;
    }


    for ( ObjectsContainer::Iterator it =  objects.begin(); it != objects.end(); ++it )
    {
        ObjectItemWrapper* item_ = objects.getElement( it->first );
        if( item_ == nullptr ) continue;


        item_->clear();
        delete item_;
        item_ = nullptr;
    }
    objects.clear();


    for ( CrossSectionsContainer::Iterator it =  cross_sections.begin(); it != cross_sections.end(); ++it )
    {
        CrossSectionItemWrapper* item_ = cross_sections.getElement( it->first );
        if( item_ == nullptr ) continue;

        item_->clear();
        delete item_;
        item_ = nullptr;
    }
    cross_sections.clear();


    for ( TrajectoriesContainer::Iterator it =  trajectories.begin(); it != trajectories.end(); ++it )
    {
        TrajectoryItemWrapper* item_ = trajectories.getElement( it->first );
        if( item_ == nullptr ) continue;

        item_->clear();
        delete item_;
        item_ = nullptr;
    }
    trajectories.clear();

    initialize();
}


void SketchScene::initialize()
{
    current_color.red = 255;
    current_color.green = 0;
    current_color.blue = 0;
    current_interaction = UserInteraction::SKETCHING;
    user_input = nullptr;
    csection = nullptr;
    volume = nullptr;
    is_current = false;


}
