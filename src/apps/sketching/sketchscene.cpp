#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>

#include "sketchscene.h"



SketchScene::SketchScene()
{
    user_input = new InputSketch();
    addItem( user_input );

    volume = nullptr;
}


SketchScene::SketchScene( CrossSection* const& raw_ )
{
    volume = nullptr;
    readCrossSection( raw_ );

    user_input = new InputSketch();
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

    csection = raw_;

    Volume* const& vol_ = raw_->getVolume();
    addVolume( vol_ );

    Volume::ObjectsContainer objs_ = vol_->getObjects();
    for( auto o: objs_ )
        addObject( o.second );

}



void SketchScene::updateCrossSection()
{

    std::cout << "Updating main csection " << csection->getDepth() << std::endl << std::flush;
    Volume* const& vol_ = csection->getVolume();


//TODO: Uncomentting
//    Volume::ObjectsContainer objs_ = vol_->getObjects();
//    for( auto o: objs_ )
//        (o.second)->setVisible( false );


    CrossSection::ObjectsContainer objs_cs_ = csection->getObjects();
    for( auto o: objs_cs_ )
    {
        updateObject( o.first );
        objects.getElement( o.first )->setVisible( true );
    }

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
    obj_->updateObject();

    update();
}




void SketchScene::setCurrent( bool status_ )
{
    is_current = status_;
}


bool SketchScene::isCurrent() const
{
    return is_current;
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
