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
}


void SketchScene::init()
{
    if( sketch == nullptr )
    {
        sketch = std::make_shared< CurveItem >();
        addItem( sketch.get() );
    }


    setSketchingMode();

    image = new ImageItemWrapper();
    image->setVisible( false );
    addItem( image );

    resize_marker = new QGraphicsEllipseItem( 0, 0, 10, 10 );
    resize_marker->setBrush( QColor( Qt::red ) );
    resize_marker->setFlag( QGraphicsItem::ItemIsSelectable, true );
    resize_marker->setVisible( false );
    addItem( resize_marker );

    move_marker = new QGraphicsEllipseItem( 0, 0, 10, 10 );
    move_marker->setBrush( QColor( Qt::blue ) );
    move_marker->setFlag( QGraphicsItem::ItemIsSelectable, true );
    move_marker->setVisible( false );
    addItem( move_marker );
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

    volume1 = std::make_shared< VolumeItem >();
    volume1->setRawVolume( volume_, csection_direction, csection_depth );

    addItem( volume1.get() );
    setSceneRect( volume1->boundingRect() );

    QPainterPath area_;
    area_.addRect( volume1->boundingRect() );
    setSelectionArea( area_, Qt::AddToSelection );

    emit ensureObjectsVisibility();

    update();
}


void SketchScene::updateVolume()
{
    volume1->update( csection_direction );
    setSceneRect( volume1->boundingRect() );

    QPainterPath area_;
    area_.addRect( volume1->boundingRect() );
    setSelectionArea( area_, Qt::AddToSelection );

    emit ensureObjectsVisibility();
    update();

}


void SketchScene::addCrossSection( const std::shared_ptr< CrossSection >& csection_ )
{
    std::size_t id_ = csection_->getIndex();
    cross_sections1[ id_ ] = std::make_shared< CrossSectionItem >();
    cross_sections1[ id_ ]->setRawCrossSection( csection_ );
    addItem( cross_sections1[ id_ ].get() );
}


void SketchScene::addImageToCrossSection( const QString& file_ )
{
    QPixmap image1;
    image1.load( file_ );

    if( image1.isNull() == true ) return;

    double ox_ = 0.0;
    double oy_ = 0.0;
    double w_ = static_cast< double >( image1.width() );
    double h_ = static_cast< double >( image1.height() );

    updateImageToCrossSection( file_.toStdString(), ox_, oy_, w_, h_ );

    emit setImageToCrossSection( file_.toStdString(), csection_direction, csection_depth, ox_, oy_, w_, h_ );

}


void SketchScene::updateImageToCrossSection( const std::string& file_, double ox_, double oy_, double w_, double y_ )
{
    QPixmap image1;
    image1.load( QString( file_.c_str() ) );

    if( image1.isNull() == true ) return;

    QTransform myTransform;
    myTransform.scale( 1, -1 );
    image1 = image1.transformed( myTransform );

    image->setImagePath( QString( file_.c_str() )  );
    image->setImage( image1, QPointF( ox_, oy_ ), QPointF( w_, y_ ) );
    image->setVisible( true );
    image->update();

}


void SketchScene::removeImageInCrossSection()
{
    image->setVisible( false );
    emit removeImageFromCrossSection( csection_direction, csection_depth );
}


void SketchScene::addStratigraphy( const std::shared_ptr< Stratigraphy >& strat_ )
{
    std::size_t id_ = strat_->getIndex();
    stratigraphies[ id_ ] = std::make_shared< StratigraphyItem>();
    stratigraphies[ id_ ]->setRawStratigraphy( strat_, csection_direction, csection_depth );
    addItem( stratigraphies[ id_ ].get() );
    update();
}


void SketchScene::updateStratigraphy( const std::size_t& id_ )
{
    if( stratigraphies.find( id_ ) == stratigraphies.end() ) return;
    if( stratigraphies[ id_ ]->isVisible() == false ) return;
    stratigraphies[ id_ ]->update();
    update();
}


void SketchScene::updateStratigraphies()
{

    for( auto it: stratigraphies )
    {
        (it.second)->update();
    }
    update();
}


void SketchScene::updateStratigraphiesTrajectories()
{

    for( auto it: stratigraphies )
    {
        (it.second)->updateLevelCurves();
        (it.second)->updateTrajectory();
    }
    update();
}


void SketchScene::getSelectedStratigraphies()
{
    QList< QGraphicsItem* > items_ = selectedItems();
    if( items_.empty() == true ) return;

    std::cout << "There are " << items_.size() << " curves selected" << std::endl << std::flush;
}


void SketchScene::addRegion( const std::shared_ptr< Regions >& region_ )
{
    std::size_t id_ = region_->getIndex();
    regions[ id_ ] = std::make_shared< RegionItem >();
    regions[ id_ ]->setRawRegion( region_ );
    regions[ id_ ]->setBorderColor( 0, 0, 0 );
    regions[ id_ ]->setFillColor( 255, 255, 255 );
    addItem( regions[ id_ ].get() );
}


void SketchScene::updateRegion( const std::size_t& id_ )
{
    if( regions.find( id_ ) == regions.end() ) return;
    regions[ id_ ]->update();
    update();
}


void SketchScene::updateRegions()
{
    for( auto it: regions )
        it.second->update();
    update();
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


void SketchScene::setSketchColor( const QColor& color_ )
{
    std::cout << "Changing sketch color..." << std::endl << std::flush;

    if( sketch == nullptr ) return;
    sketch->setColor( color_ );
    update();
}


void SketchScene::processSketch()
{
    if( sketch->isEmpty() == true ) return;


    if( ( csection_direction == Settings::CrossSection::CrossSectionDirections::Y )
            )
        sketch->getMonotonicY();

    else
        sketch->getMonotonicX();


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
    sketch = std::make_shared< CurveItem >();
    addItem( sketch.get() );
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

void SketchScene::setOldSelectingStratigraphyMode( bool status_ )
{

    clearSelection();

    sketch->setFlag( QGraphicsItem::ItemIsSelectable, status_ );

    for( auto it: stratigraphies )
    {
        (it.second)->setFlag( QGraphicsItem::ItemIsSelectable, status_ );
    }

    if( status_ == true )
    {
        current_interaction1 = UserInteraction1::SELECTING_STRATIGRAPHY_OLD;
    }
    else
    {
        setSketchingMode();
    }

    update();


}


void SketchScene::setSelectingStratigraphyMode( bool status_ )
{

    clearSelection();

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


void SketchScene::setResizingImageMode( bool status_ )
{
    move_marker->setVisible( status_ );
    resize_marker->setVisible( status_ );

    if( status_ == true )
    {
        current_interaction1 = UserInteraction1::RESIZING_IMAGE;
    }
    else
    {
        setSketchingMode();
    }
}


void SketchScene::setSelectingRegionsMode( bool status_ )
{
    clearSelection();

    for( auto it: regions )
    {
        it.second->setFlag( QGraphicsItem::ItemIsSelectable, status_ );
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



void SketchScene::addToSketchesOfSelection()
{

    if( sketch == nullptr ) return;

    std::shared_ptr< CurveItem > sketch_ = std::make_shared< CurveItem >();
    sketch_->setCurve( sketch->getCurve() );

    std::size_t id_ = sketches_of_selection.size();
    sketches_of_selection.push_back( sketch_ );
    addItem( sketches_of_selection[ id_ ].get() );


    sketch->clear();

}


void SketchScene::removeSketchesOfSelection()
{

    for( auto sk_: sketches_of_selection )
    {
        removeItem( sk_.get() );
        sk_->clear();
        sk_.reset();
    }
    sketches_of_selection.clear();

}


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
//        regions[ nregions ] ->addPoint( p_ );
    }


    if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction1 == UserInteraction1::SELECTING_STRATIGRAPHY ) )
    {
        sketch->create( p_ );
    }


    QGraphicsScene::mousePressEvent( event_ );
    update();

}


void SketchScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event_ )
{

    QGraphicsScene::mouseDoubleClickEvent( event_ );
    update();

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

    if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction1 == UserInteraction1::SELECTING_STRATIGRAPHY )  )
    {
        sketch->add(  p_ );
    }

    if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction1 == UserInteraction1::RESIZING_IMAGE )  )
    {
        if( resize_marker->isSelected() )
            scaleImage( resize_marker->pos() );
        else if( resize_marker->isSelected() )
            moveImage( move_marker->pos() );
    }

    QGraphicsScene::mouseMoveEvent( event_ );
    update();

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

        emit resizeVolumeDimensions( csection_direction, static_cast< double >( volume1->boundingRect().width() ), static_cast< double >( volume1->boundingRect().height() ) );
//        emit ensureObjectsVisibility();
    }

    if( ( event_->button() == Qt::RightButton ) && ( current_interaction1 == UserInteraction1::SELECTING_STRATIGRAPHY_OLD )  )
    {
        getSelectedStratigraphies();
    }

    if( ( event_->button() == Qt::RightButton ) && ( current_interaction1 == UserInteraction1::SELECTING_STRATIGRAPHY )  )
    {
        addToSketchesOfSelection();
    }

    QGraphicsScene::mouseReleaseEvent( event_ );
    update();

}


void SketchScene::dragEnterEvent( QGraphicsSceneDragDropEvent* event_ )
{

    if( ( event_->mimeData()->hasUrls() == true ) && ( event_->mimeData()->hasImage() ) )
    {
        event_->acceptProposedAction();
    }

}


void SketchScene::dropEvent( QGraphicsSceneDragDropEvent* event_ )
{
    const QMimeData *mime_data = event_->mimeData();
    QString url_file = mime_data->urls().at( 0 ).toLocalFile();
    url_file = QDir::toNativeSeparators( url_file );

    addImageToCrossSection( url_file );
}


void SketchScene::dragMoveEvent( QGraphicsSceneDragDropEvent* event_ )
{
    event_->accept();
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

}


void SketchScene::keyPressEvent( QKeyEvent *event )
{
    switch( event->key() )
    {
        case Qt::Key_Delete:
            std::cout << "Remove item" << std::endl << std::flush;
//            removeItem();
            break;
    case Qt::Key_S:
            setSelectingStratigraphyMode( true );
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


void SketchScene::clearScene()
{
    for( auto &it: items() )
        QGraphicsScene::removeItem( it );


    if( sketch != nullptr )
        sketch.reset();
    sketch = nullptr;

    if( volume1 != nullptr )
        volume1.reset();
    volume1 = nullptr;

    for( auto it: cross_sections1 )
        (it.second).reset();
    cross_sections1.clear();

    for( auto it: stratigraphies )
        (it.second).reset();
    stratigraphies.clear();

    for( auto it: regions )
        (it.second).reset();
    regions.clear();



    //init();


}


SketchScene::~SketchScene()
{

    clearScene();


}
