/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QPixmap>
#include <QDir>
#include <QUrl>
#include <QMimeData>
#include <QKeyEvent>
#include <QtSvg/QSvgGenerator>
#include "sketchscene.h"



SketchScene::SketchScene()
{

    init();


    ///================================================================================


}


void SketchScene::init()
{
    if( sketch == nullptr )
    {
        sketch = new CurveItem();
        addItem( sketch );
    }
    setSketchingMode();
}


void SketchScene::setCrossSectionInformation( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    csection_direction = dir_;
    csection_depth = depth_;

    for( auto it: stratigraphies )
    {
        (it.second)->setCrossSection( csection_direction, csection_depth );

    }

}


void SketchScene::createVolume( const std::shared_ptr< Volume >& volume_ )
{

    volume1 = new VolumeItem();
    volume1->setRawVolume( volume_, csection_direction, csection_depth );

    addItem( volume1 );
    setSceneRect( volume1->boundingRect() );

    emit ensureObjectsVisibility();

    update();
}

void SketchScene::updateVolume()
{
    volume1->update();
    emit ensureObjectsVisibility();
    update();

}


void SketchScene::addStratigraphy( const std::shared_ptr< Stratigraphy >& strat_ )
{
    std::size_t id_ = strat_->getIndex();
    stratigraphies[ id_ ] = new StratigraphyItem();
    stratigraphies[ id_ ]->setRawStratigraphy( strat_, csection_direction, csection_depth );
    addItem( stratigraphies[ id_ ] );
    update();
}

void SketchScene::updateStratigraphy( const std::size_t& id_ )
{
    if( stratigraphies[ id_ ]->isVisible() == false ) return;
    stratigraphies[ id_ ]->update();
    update();
}


void SketchScene::addRegion( const std::shared_ptr< Regions >& region_ )
{
    std::size_t id_ = region_->getIndex();
    regions[ id_ ] = new RegionItem();
    regions[ id_ ]->setRawRegion( region_ );
    regions[ id_ ]->setBorderColor( 0, 0, 0 );
    regions[ id_ ]->setFillColor( 255, 255, 255 );
    addItem( regions[ id_ ] );
}

void SketchScene::addCrossSection( const std::shared_ptr< CrossSection >& csection_ )
{
    std::size_t id_ = csection_->getIndex();
    cross_sections1[ id_ ] = new CrossSectionItem();
    cross_sections1[ id_ ]->setRawCrossSection( csection_ );
    addItem( cross_sections1[ id_ ] );
}

void SketchScene::enableSketch( bool status_ )
{
    sketch_enabled = status_;
}


bool SketchScene::isSketchEnabled() const
{
    if( sketch == nullptr ) return false;
    return sketch_enabled;
}

void SketchScene::cancelSketch()
{
    std::cout << "Canceling sketch..." << std::endl << std::flush;

    if( sketch == nullptr ) return;
    sketch->clear();
    update();
}

void SketchScene::submitSketch()
{
    std::cout << "Submitting sketch..." << std::endl << std::flush;

    if( sketch == nullptr ) return;

    emit sketchDone( sketch->getCurve(), csection_direction, csection_depth );
    sketch->clear();

    update();
}

void SketchScene::setSketchColor()
{
    std::cout << "Changing sketch color..." << std::endl << std::flush;

    if( sketch == nullptr ) return;

    sketch->setColor( 255, 0, 0 );
    update();
}


void SketchScene::endObject()
{
    std::cout << "Ending object..." << std::endl << std::flush;

    if( sketch == nullptr ) return;
    sketch->setDone();

    emit createObject();

    update();
}


void SketchScene::setSketchingMode()
{
    current_interaction1 = UserInteraction1::SKETCHING;
    sketch_enabled = true;

    if ( sketch != nullptr ) return;
    sketch = new CurveItem();
    addItem( sketch );
}


void SketchScene::setResizingBoundaryMode( bool status_ )
{
    if( status_ == true )
        current_interaction1 = UserInteraction1::RESIZING_BOUNDARY;
    else
    {
        setSketchingMode();
    }
}


//temporary
void SketchScene::setCreateRegionMode()
{
    current_interaction1 = UserInteraction1::CREATE_REGION;
}
//temporary

void SketchScene::setSelectingStratigraphyMode( bool status_ )
{

    clearSelection();

    sketch->setFlag( QGraphicsItem::ItemIsSelectable, status_ );
    sketch->setFlag( QGraphicsItem::ItemIsMovable, status_ );


    if( status_ == true )
    {
        current_interaction1 = UserInteraction1::SELECTING_STRATIGRAPHY;
    }
    else
    {
        setSketchingMode();
    }

    update();


}


void SketchScene::setSelectingRegionsMode( bool status_ )
{
    clearSelection();

    for( auto it: regions )
    {
        it.second->setFlag( QGraphicsItem::ItemIsSelectable, status_ );
        it.second->setFlag( QGraphicsItem::ItemIsMovable, status_ );
        it.second->update();
    }

    if( status_ == true )
        current_interaction1 = UserInteraction1::SELECTING_REGIONS;
    else
    {
        setSketchingMode();
    }

    update();
}


//void SketchScene::setSelectingWellsMode( bool status_ )
//{
//    clearSelection();

//    for( auto it: wells )
//    {
//        it.second->setFlag( QGraphicsItem::ItemIsSelectable, status_ );
//        it.second->setFlag( QGraphicsItem::ItemIsMovable, status_ );
//        it.second->update();
//    }

//    if( status_ == true )
//        current_interaction1 = UserInteraction1::SELECTING_WELLS;
//    else
//    {
//        setSketchingMode();
//    }

//    update();
//}



///================================================================================

void SketchScene::mousePressEvent( QGraphicsSceneMouseEvent *event_ )
{

    QPointF p_ = event_->scenePos();


    if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction1 == UserInteraction1::SKETCHING ) )
    {
        sketch->create( p_ );
    }

    else if( ( event_->buttons() & Qt::RightButton ) && ( current_interaction1 == UserInteraction1::SKETCHING )  )
    {
       submitSketch();
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction1 == UserInteraction1::RESIZING_BOUNDARY )  )
    {
        volume1->setStartPoint( p_ );
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction1 == UserInteraction1::CREATE_REGION )  )
    {
        regions[ nregions ] ->addPoint( p_ );
    }


    QGraphicsScene::mousePressEvent( event_ );
    update();



//    if( is_current == false ) return;


//    if( ( event->buttons() & Qt::LeftButton ) &&
//        ( current_interaction == UserInteraction::SKETCHING ) )
//    {
//        user_input->create( event->scenePos() );
//    }

//    else if( ( event->buttons() & Qt::RightButton ) &&
//        ( current_interaction == UserInteraction::SKETCHING ) )
//    {
//        if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::X )
//            emit acceptCurve( user_input->done( InputSketch::Direction::Z ), csection->getDepth() );
//        else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Y )
//            emit acceptCurve( user_input->done( InputSketch::Direction::Y  ), csection->getDepth() );
//        else if( csection->getDirection() == Settings::CrossSection::CrossSectionDirections::Z )
//            emit acceptCurve( user_input->done( InputSketch::Direction::X  ), csection->getDepth() );


//        user_input->clear();
//    }

//    else if( ( event->buttons() & Qt::LeftButton ) &&
//        ( current_interaction == UserInteraction::EDITING_BOUNDARY ) )
//    {
//        volume->startPoint( event->scenePos() );
//    }

//    else if(( event->buttons() & Qt::LeftButton ) &&
//             ( current_interaction == UserInteraction::EDITING_SCENE ||
//               current_interaction == UserInteraction::MOVING_IMAGE  ||
//               current_interaction == UserInteraction::RESIZING_IMAGE ) )
//    {
//        editItem();
//    }


//    QGraphicsScene::mousePressEvent( event );
//    update();
}


void SketchScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event_ )
{

    QGraphicsScene::mouseDoubleClickEvent( event_ );
    update();

//    if( event->modifiers() & Qt::ControlModifier )
//    {
//        if( csection == nullptr ) return;
//        if( views().empty() == true ) return;
//        emit setAsCurrent( csection->getDepth(), views()[ 0 ] );
//    }

//    else if( current_interaction == UserInteraction::SKETCHING )
//    {
//         emit commitObject();
//        user_input->clear();
//    }

}


void SketchScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event_ )
{
    QPointF p_ = event_->scenePos();

    if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction1 == UserInteraction1::SKETCHING )  )
    {
        sketch->add(  p_ );
    }
    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction1 == UserInteraction1::RESIZING_BOUNDARY )  )
    {
        volume1->setEndPoint( p_ );
    }

    QGraphicsScene::mouseMoveEvent( event_ );
    update();


//    if( is_current == false ) return;

//    if( ( event->buttons() & Qt::LeftButton ) &&
//        ( current_interaction == UserInteraction::SKETCHING ) /*&& ( user_input != nullptr )*/ )
//    {
//        user_input->add(  event->scenePos() );
//    }
//    else if( ( event->buttons() & Qt::LeftButton ) &&
//        ( current_interaction == UserInteraction::EDITING_BOUNDARY ) )
//    {
//        volume->resize( event->scenePos() );
//    }

//    else if( ( event->buttons() & Qt::LeftButton ) &&
//        ( current_interaction == UserInteraction::RESIZING_IMAGE ) )
//    {
//        image->resizeRectangle( event->scenePos() );
//        updateImageCrossSection();
//    }

//    else if( ( event->buttons() & Qt::LeftButton ) &&
//        ( current_interaction == UserInteraction::MOVING_IMAGE ) )
//   {
//        image->moveRectangle( event->scenePos() );
//        updateImageCrossSection();
//    }



//    QGraphicsScene::mouseMoveEvent( event );
//    update();

}


void SketchScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event_ )
{


    QPointF p_ = event_->scenePos();

    if( ( event_->button() == Qt::LeftButton ) && ( current_interaction1 == UserInteraction1::SKETCHING )  )
    {
        sketch->connect();
    }
    else if( ( event_->button() == Qt::LeftButton ) && ( current_interaction1 == UserInteraction1::RESIZING_BOUNDARY )  )
    {
        volume1->setEndPoint( p_ );
        setSceneRect( volume1->boundingRect() );
        emit ensureObjectsVisibility();
    }

    QGraphicsScene::mouseReleaseEvent( event_ );
    update();


//    if( is_current == false ) return;

//    if( ( current_interaction == UserInteraction::SKETCHING ) /*&& ( user_input != nullptr )*/ )
//    {
//        user_input->process();
//    }

//    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
//    {
//        emit acceptVolumeDimensions( csection->getDirection(), static_cast< double >( volume->boundingRect().width() ),
//                                     static_cast< double >( volume->boundingRect().height() ) );
//    }

//    else if( current_interaction == UserInteraction::SELECTING )
//    {
//        selectObjectAsBounderingRegion();
//    }

//    else if( ( event->buttons() & Qt::LeftButton ) &&
//        ( current_interaction == UserInteraction::RESIZING_IMAGE ) )
//    {
//        image->resizeRectangle( event->scenePos() );
//        updateImageCrossSection();
//    }

//    else if( ( event->buttons() & Qt::LeftButton ) &&
//        ( current_interaction == UserInteraction::MOVING_IMAGE ) )
//    {

//        image->moveRectangle( event->scenePos() );
//        updateImageCrossSection();
//    }

//    else if( ( current_interaction == UserInteraction::EDITING_SCENE ||
//               current_interaction == UserInteraction::MOVING_IMAGE  ||
//               current_interaction == UserInteraction::RESIZING_IMAGE ) )
//    {
//        current_interaction = UserInteraction::EDITING_SCENE;

//        resize_marker->setSelected( false );
//        move_marker->setSelected( false );
//        editItem();

//    }




    QGraphicsScene::mouseReleaseEvent( event_ );
    update();
}



void SketchScene::dragEnterEvent( QGraphicsSceneDragDropEvent* event_ )
{
//    if( ( event->mimeData()->hasUrls() == true ) && ( event->mimeData()->hasImage() ) )
//    {
//        event->acceptProposedAction();
//    }

}


void SketchScene::dropEvent( QGraphicsSceneDragDropEvent* event_ )
{
//    const QMimeData *mime_data = event->mimeData();
//    QString url_file = mime_data->urls().at( 0 ).toLocalFile();
//    url_file = QDir::toNativeSeparators( url_file );

//    setImageToCrossSection( url_file );

}


void SketchScene::dragMoveEvent( QGraphicsSceneDragDropEvent* event_ )
{
//    event->accept();
}



void SketchScene::wheelEvent( QGraphicsSceneWheelEvent *event_ )
{
    const double ZOOM_SCALE = 1.1;

    if( views().isEmpty() == true ) return;

    QGraphicsView* gv_ = views()[ 0 ];

    if( event_->delta() > 0 )
        gv_->scale( ZOOM_SCALE, ZOOM_SCALE );
    else
        gv_->scale( 1.0/ZOOM_SCALE, 1.0/ZOOM_SCALE );


    QGraphicsScene::wheelEvent( event_ );
    update();


//    if( views().isEmpty() == true ) return;

//    QGraphicsView* gv_ = views()[ 0 ];

//    if( event->delta() > 0 )
//        gv_->scale( ZOOM_SCALE, ZOOM_SCALE );
//    else
//        gv_->scale( 1.0/ZOOM_SCALE, 1.0/ZOOM_SCALE );


//    QGraphicsScene::wheelEvent( event );
//    update();
}


void SketchScene::keyPressEvent( QKeyEvent *event )
{
    switch( event->key() )
    {
        case Qt::Key_Delete:
            std::cout << "Remove item" << std::endl << std::flush;
//            removeItem();
            break;
        default:
            break;
    };
}


//void SketchScene::addWell( const std::shared_ptr< Well >& well_ )
//{

//    std::size_t id_ = well_->getIndex();
//    wells[ id_ ] = new WellItem();
//    wells[ id_ ]->setRawWell( well_, csection_direction, csection_depth );
//    addItem( wells[ id_ ] );

//    emit ensureObjectsVisibility();
//}


