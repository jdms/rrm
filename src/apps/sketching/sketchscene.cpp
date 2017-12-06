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
    image = new ImageItemWrapper();
    addItem( image );

    user_input = new InputSketch();
    addItem( user_input );

    csection_image = new QGraphicsPixmapItem();
    addItem( csection_image );

    resize_marker = new QGraphicsEllipseItem( 0, 0, 10, 10 );
    resize_marker->setBrush( QColor( Qt::red ) );
    resize_marker->setFlag( QGraphicsItem::ItemIsSelectable, true );
    addItem( resize_marker );

    move_marker = new QGraphicsEllipseItem( 0, 0, 10, 10 );
    move_marker->setBrush( QColor( Qt::blue ) );
    move_marker->setFlag( QGraphicsItem::ItemIsSelectable, true );
    addItem( move_marker );

    move_marker->setVisible( false );
    resize_marker->setVisible( false );

    axes.setVisible( true );
    addItem( &axes );



    volume = nullptr;
}


SketchScene::SketchScene( CrossSection* const& raw_ ):csection( raw_ ), volume( nullptr )
{


    image = new ImageItemWrapper();
    addItem( image );

    user_input = new InputSketch();
    user_input->setCurrentColor( QColor( current_color.red, current_color.green, current_color.blue ) );
    addItem( user_input );

    csection_image = new QGraphicsPixmapItem();
    addItem( csection_image );

    resize_marker = new QGraphicsEllipseItem( 0, 0, 10, 10 );
    resize_marker->setBrush( QColor( Qt::red ) );
    resize_marker->setFlag( QGraphicsItem::ItemIsSelectable, true );
    addItem( resize_marker );

    move_marker = new QGraphicsEllipseItem( 0, 0, 10, 10 );
    move_marker->setBrush( QColor( Qt::blue ) );
    move_marker->setFlag( QGraphicsItem::ItemIsSelectable, true );
    addItem( move_marker );


    axes.setVisible( true );
    addItem( &axes );

    readCrossSection( raw_ );

    move_marker->setVisible( false );
    resize_marker->setVisible( false );

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

}


void SketchScene::editItem()
{
//    QList< QGraphicsItem* > items = selectedItems();
//    if( items.empty() == true ) return;

//    if( items.size() > 1 )
//        items.removeFirst();

//    QGraphicsItem* item_ = items[ 0 ];


//    if( item_->type() == QGraphicsEllipseItem::Type )
//    {

        bool move_selected = move_marker->isUnderMouse();
        bool resize_selected = resize_marker->isUnderMouse();

        if( move_selected == true )
        {
            current_interaction = UserInteraction::MOVING_IMAGE;
        }
        else if( resize_selected == true )
        {
            current_interaction = UserInteraction::RESIZING_IMAGE;
        }

//    }

    else
        std::cout << "path type " << std::endl << std::flush;

    update();
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



void SketchScene::setImageToCrossSection( const QString& file_ )
{

    QPixmap image1;
    image1.load( file_ );

    float x_ = image1.width();
    float y_ = image1.height();

    setImageToCrossSection( file_, 0.0, 0.0, x_, y_ );
}


void SketchScene::setImageToCrossSection( const QString& file_, double ox_, double oy_, double x_, double y_ )
{

    QPixmap image1;
    image1.load( file_ );

    if( image1.isNull() == true ) return;

    QTransform myTransform;
    myTransform.scale( 1, -1 );
    image1 = image1.transformed( myTransform );

    image->setImagePath( file_ );
    image->setImage( image1, QPointF( ox_, oy_ ), QPointF( x_, y_ ) );
    image->setVisible( true );
    image->update();


    emit setImageCrossSection( csection->getDepth(), file_, ox_, oy_, x_, y_ );

    update();
}



void SketchScene::updateImageCrossSection()
{
    QString file_ = image->getImagePath();
    QPointF origin_ = image->getOrigin();
    QPointF topright_ = image->getTopRight();


    move_marker->setPos( origin_.x(), origin_.y() );
    move_marker->setVisible( true );
    move_marker->update();

    resize_marker->setPos( topright_.x(), topright_.y() );
    resize_marker->setVisible( true );
    resize_marker->update();


    emit setImageCrossSection( csection->getDepth(), file_, origin_.x(), origin_.y(),
                               topright_.x(), topright_.y() );

}


void SketchScene::updateCrossSection()
{

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
        double x_;
        double y_;
        csection->getImage( path_, ox_, oy_, x_, y_ );
        setImageToCrossSection( QString( path_.c_str() ), ox_, oy_, x_, y_ );
    }
    else
    {
        image->setVisible( false );
        image->update();
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

    if( image != nullptr )
        image->update();

}


void SketchScene::updateObject(  const std::size_t& index_ )
{
    if( csection->getDirection() != Settings::CrossSection::CrossSectionDirections::Z ) return;

    ObjectItemWrapper* obj_ = objects.getElement( index_ );

    obj_->updateDepth( csection->getDepth() );
    obj_->updateObject();

    if( image != nullptr )
        if( image->isVisible() == true )
            image->show();

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
    image->setFlag( QGraphicsItem::ItemIsSelectable, false );
    resize_marker->setVisible( false );
    move_marker->setVisible( false );
}


void SketchScene::setModeEditingBoundary()
{
    current_interaction = UserInteraction::EDITING_BOUNDARY;
}


void SketchScene::setModeEditingScene()
{
    current_interaction = UserInteraction::EDITING_SCENE;
//    image->setFlag( QGraphicsItem::ItemIsMovable, true );
//    image->setFlag( QGraphicsItem::ItemIsSelectable, true );

    move_marker->setPos( image->getOrigin() );
    move_marker->setVisible( true );
    move_marker->update();

    resize_marker->setPos( image->getTopRight() );
    resize_marker->setVisible( true );
    resize_marker->update();

    move_marker->setVisible( true );
    resize_marker->setVisible( true );
    image->update();

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

    else if(( event->buttons() & Qt::LeftButton ) &&
             ( current_interaction == UserInteraction::EDITING_SCENE ||
               current_interaction == UserInteraction::MOVING_IMAGE  ||
               current_interaction == UserInteraction::RESIZING_IMAGE ) )
    {
        editItem();
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

    else if( ( event->buttons() & Qt::LeftButton ) &&
        ( current_interaction == UserInteraction::RESIZING_IMAGE ) )
    {
        image->resizeRectangle( event->scenePos() );
        updateImageCrossSection();
    }

    else if( ( event->buttons() & Qt::LeftButton ) &&
        ( current_interaction == UserInteraction::MOVING_IMAGE ) )
   {
        image->moveRectangle( event->scenePos() );
        updateImageCrossSection();
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

    else if( ( event->buttons() & Qt::LeftButton ) &&
        ( current_interaction == UserInteraction::RESIZING_IMAGE ) )
    {
        image->resizeRectangle( event->scenePos() );
        updateImageCrossSection();
    }

    else if( ( event->buttons() & Qt::LeftButton ) &&
        ( current_interaction == UserInteraction::MOVING_IMAGE ) )
    {

        image->moveRectangle( event->scenePos() );
        updateImageCrossSection();
    }

    else if( ( current_interaction == UserInteraction::EDITING_SCENE ||
               current_interaction == UserInteraction::MOVING_IMAGE  ||
               current_interaction == UserInteraction::RESIZING_IMAGE ) )
    {
        current_interaction = UserInteraction::EDITING_SCENE;

        resize_marker->setSelected( false );
        move_marker->setSelected( false );
        editItem();

    }

//    else if(
//             ( current_interaction == UserInteraction::EDITING_SCENE ) )
//    {
//        editItem();
//    }




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
    image = nullptr;


}
