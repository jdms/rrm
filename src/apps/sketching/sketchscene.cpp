#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

#include "sketchscene.h"



SketchScene::SketchScene()
{
    user_input = new InputSketch();
    addItem( user_input );

    volume = nullptr;
}


SketchScene::SketchScene( CrossSection* const& raw_ ):csection( raw_ ), volume( nullptr )
{
    readCrossSection( raw_ );

    user_input = new InputSketch();
    user_input->setCurrentColor( QColor( current_color.red, current_color.green, current_color.blue ) );
    addItem( user_input );

    connect( this, &SketchScene::discard, [=](){ user_input->clear(); update(); } );
    connect( this, &SketchScene::commit, [=](){ emit acceptCurve( user_input->done() ); } );
    connect( this, &SketchScene::create, [=](){ emit acceptCurve( user_input->done() ); } );
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


void SketchScene::readCrossSection( CrossSection* const& raw_ )
{
    if( raw_ == nullptr ) return;

    csection = raw_;

    Volume* const& vol_ = raw_->getVolume();
    addVolume( vol_ );

    Volume::ObjectsContainer objs_ = vol_->getObjects();
    for( auto o: objs_ )
        addObject( o.second );

    if( raw_->getDirection() != CrossSection::Direction::Y ) return;

    Volume::CrossSectionsContainer csections_ = vol_->getCrossSections();
    for( auto c: csections_ )
    {
        addCrossSection( c.second );
    }


}



void SketchScene::updateCrossSection()
{

    Volume* const& vol_ = csection->getVolume();


////TODO: Uncomentting
    Volume::ObjectsContainer objs_ = vol_->getObjects();
    for( auto o: objs_ )
    {
        updateObject( o.first );
    }
//        (o.second)->setVisible( false );


//    CrossSection::ObjectsContainer objs_cs_ = csection->getObjects();
//    for( auto o: objs_cs_ )
//    {
////        objects.getElement( o.first )->setVisible( true );
//        updateObject( o.first );
//    }


}



void SketchScene::addVolume( Volume* const &raw_ )
{
    clearVolume();
    volume = new VolumeItemWrapper( raw_, VolumeItemWrapper::Section::XY );

    addItem( volume );
    setSceneRect( volume->boundingRect() );
    update();
}


void SketchScene::updateVolume()
{
    volume->updateItem();
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
    //TODO: check if valid raw->getIndex


    std::size_t index_ = raw_->getIndex();

    ObjectItemWrapper* obj_ = new ObjectItemWrapper( raw_, depth_ );
    objects.addElement( index_, obj_ );

    addItem( obj_ );

}


void SketchScene::updateObject(  const std::size_t& index_ )
{
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

    std::cout << "Object selected: " << id << std::endl << std::flush;
    emit objectSelected( id );

}




void SketchScene::addCrossSection( CrossSection * const &raw_ )
{

    Volume* const& vol_ = raw_->getVolume();

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
        emit acceptCurve( user_input->done() );
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
    if( ( event->buttons() & Qt::LeftButton ) )
    {
        if( csection == nullptr ) return;
        if( views().empty() == true ) return;
        emit setAsCurrent( csection->getDepth(), views()[ 0 ] );
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
        emit acceptVolumeDimensions( static_cast< double >( volume->boundingRect().width() ),
                                     static_cast< double >( volume->boundingRect().height() ) );
    }

    else if( current_interaction == UserInteraction::SELECTING )
    {
        selectObjectAsBounderingRegion();
    }


    QGraphicsScene::mouseReleaseEvent( event );
    update();
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
