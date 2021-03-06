/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file sketchscene.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class SketchScene
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

    // this is what the user uses to make his/her sketch
    if( sketch == nullptr )
    {
        sketch = std::make_shared< CurveItem >();
        addItem( sketch.get() );
    }

    setSketchingMode();

    image = new ImageItemWrapper();
    image->setVisible( false );
    addItem( image );


    // markersto help the iser resize and move the image
    resize_marker = new QGraphicsEllipseItem( 0, 0, 10, 10 );
    resize_marker->setBrush( QColor( Qt::red ) );
    resize_marker->setFlag( QGraphicsItem::ItemIsSelectable, true );
    resize_marker->setFlag( QGraphicsItem::ItemIsMovable, true );
    resize_marker->setVisible( false );
    addItem( resize_marker );

    move_marker = new QGraphicsEllipseItem( 0, 0, 10, 10 );
    move_marker->setBrush( QColor( Qt::blue ) );
    move_marker->setFlag( QGraphicsItem::ItemIsSelectable, true );
    move_marker->setFlag( QGraphicsItem::ItemIsMovable, true );
    move_marker->setVisible( false );
    addItem( move_marker );

    boundary_area = new PolygonItem();
    boundary_area->setBorderVisible( false );
    boundary_area->setTransparency( true );
    boundary_area->setVisible( false );
    addItem( boundary_area );


    trajectory_point = new QGraphicsEllipseItem( 0, 0, 10, 10 );
    trajectory_point->setBrush( QColor( Qt::red ) );
    trajectory_point->setFlag( QGraphicsItem::ItemIsSelectable, true );
    trajectory_point->setFlag( QGraphicsItem::ItemIsMovable, true );
    trajectory_point->setVisible( false );
    addItem( trajectory_point );

    dipangle = new DipAnglePicture();
    dipangle->setVisible( false );
    addItem( dipangle );

    axes.setVisible( true );
    addItem( &axes );

    emit ensureObjectsVisibility();

}


void SketchScene::setCrossSectionInformation( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    csection_direction = dir_;
    csection_depth = depth_;

    if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
        invertImage( true );

    for( auto it: stratigraphies )
    {
        (it.second)->setCrossSection( csection_direction, csection_depth );
    }

}


void SketchScene::getCrossSectionInformation( Settings::CrossSection::CrossSectionDirections& dir_, double& depth_ )
{
    dir_ = csection_direction;
    depth_ = csection_depth;
}


void SketchScene::createVolume( const std::shared_ptr< Volume >& volume_ )
{

    volume1 = std::make_shared< VolumeItem >();
    volume1->setRawVolume( volume_, csection_direction, csection_depth );

    addItem( volume1.get() );
    setSceneRect( volume1->boundingRect() );

    // the volume dimensions on the scene 2d depends on the current cross-section direction

    if( csection_direction == Settings::CrossSection::CrossSectionDirections::X )
    {
        axes.setPlane( CoordinateAxes2d::Plane::YZ );
        axes.setAxisXLenght( volume_->getLenght() );
        axes.setAxisYLenght( volume_->getHeight() );
    }
    else if( csection_direction == Settings::CrossSection::CrossSectionDirections::Y )
    {
        axes.setPlane( CoordinateAxes2d::Plane::XZ );
        axes.setAxisXLenght( volume_->getWidth() );
        axes.setAxisYLenght( volume_->getLenght() );
    }
    else if( csection_direction == Settings::CrossSection::CrossSectionDirections::Z )
    {
        axes.setPlane( CoordinateAxes2d::Plane::XY );
        axes.setAxisXLenght( volume_->getWidth() );
        axes.setAxisYLenght( volume_->getHeight() );
    }

    emit ensureObjectsVisibility();

    update();
}


void SketchScene::updateVolume()
{
    volume1->update( csection_direction );
    setSceneRect( volume1->boundingRect() );

    const std::shared_ptr< Volume >& volume_ = volume1->getRawVolume();
    if( csection_direction == Settings::CrossSection::CrossSectionDirections::X )
    {
        axes.setPlane( CoordinateAxes2d::Plane::YZ );
        axes.setAxisXLenght( volume_->getLenght() );
        axes.setAxisYLenght( volume_->getHeight() );
    }
    else if( csection_direction == Settings::CrossSection::CrossSectionDirections::Y )
    {
        axes.setPlane( CoordinateAxes2d::Plane::XZ );
        axes.setAxisXLenght( volume_->getWidth() );
        axes.setAxisYLenght( volume_->getLenght() );
    }
    else if( csection_direction == Settings::CrossSection::CrossSectionDirections::Z )
    {
        axes.setPlane( CoordinateAxes2d::Plane::XY );
        axes.setAxisXLenght( volume_->getWidth() );
        axes.setAxisYLenght( volume_->getHeight() );
    }

    emit ensureObjectsVisibility();

    update();

}


void SketchScene::addCrossSection( const std::shared_ptr< CrossSection >& csection_ )
{
    std::size_t id_ = csection_->getIndex();
    cross_sections[ id_ ] = std::make_shared< CrossSectionItem >();
    cross_sections[ id_ ]->setRawCrossSection( csection_ );
    addItem( cross_sections[ id_ ].get() );
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

    // this method set the image in the scene
    setImageInCrossSection( file_.toStdString(), ox_, oy_, w_, h_ );

    // this signal notify the controller to associate the path and the geometry of the image to the given cross-section
    emit setImageToCrossSection( file_.toStdString(), csection_direction, csection_depth, ox_, oy_, w_, h_ );

}


void SketchScene::setImageInCrossSection( const std::string& file_, double ox_, double oy_, double w_, double y_ )
{

    if( image == nullptr ) return;

    QPixmap image1;
    image1.load( QString( file_.c_str() ) );

    if( image1.isNull() == true ) return;

    QTransform myTransform;
    myTransform.scale( 1, -1*factor );
    image1 = image1.transformed( myTransform );

    image->setImagePath( QString( file_.c_str() )  );
    image->setImage( image1, QPointF( ox_, oy_ ), QPointF( w_, y_ ) );
    image->setVisible( true );
    image->update();

    if( volume1 != nullptr )
        image->setZValue( volume1->zValue() + 1 );

    if( dipangle->isVisible() == true )
        dipangle->setZValue( image->zValue() + 1 );

}


void SketchScene::removeImageInCrossSection()
{
    // makes the image invisible but the controller does not remove the image from the cross-section
    if( image == nullptr ) return;
    image->setVisible( false );
    update();
}


void SketchScene::removeImageInCrossSectionAndUpdate()
{
    // this method makes the controller removes the image from the cross-section
    if( image == nullptr ) return;
    image->setVisible( false );
    emit removeImageFromCrossSection( csection_direction, csection_depth );
    update();
}


void SketchScene::updateImageinCrossSection()
{
    if( image == nullptr ) return;

    QString file_ = image->getImagePath();
    QPointF origin_ = image->getOrigin();
    QPointF topright_ = image->getTopRight();

    emit setImageToCrossSection( file_.toStdString(), csection_direction, csection_depth, origin_.x(), origin_.y(), topright_.x(), topright_.y() );
    QGraphicsScene::update();

}


void SketchScene::addStratigraphy( const std::shared_ptr< Stratigraphy >& strat_ )
{
    std::size_t id_ = strat_->getIndex();
    stratigraphies[ id_ ] = std::make_shared< StratigraphyItem>();
    stratigraphies[ id_ ]->setRawStratigraphy( strat_, csection_direction, csection_depth );
    addItem( stratigraphies[ id_ ].get() );
    QGraphicsScene::update();

}


void SketchScene::updateStratigraphy( const std::size_t& id_ )
{
    if( stratigraphies.find( id_ ) == stratigraphies.end() ) return;
    if( stratigraphies[ id_ ]->isVisible() == false ) return;
    stratigraphies[ id_ ]->update();
    QGraphicsScene::update();
}


void SketchScene::updateStratigraphies()
{

    for( auto it: stratigraphies )
    {
        (it.second)->update();
    }
    QGraphicsScene::update();
}


void SketchScene::updateStratigraphiesTrajectories()
{

    for( auto it: stratigraphies )
    {
        (it.second)->updateLevelCurves();
        (it.second)->updateTrajectory();
    }
    QGraphicsScene::update();
}


void SketchScene::getSelectedStratigraphies()
{
    QList< QGraphicsItem* > items_ = selectedItems();
    if( items_.empty() == true ) return;

    // select one curve at time
    StratigraphyItem* const& obj_ = dynamic_cast< StratigraphyItem* >( items_[ 0 ] );
    std::size_t id_ = obj_->getIndex();

    emit objectSelected( id_ );

}


void SketchScene::addRegion( const std::shared_ptr< Regions >& region_ )
{
    std::size_t id_ = region_->getIndex();

    int r_, g_, b_;
    region_->getColor( r_, g_, b_ );

    regions[ id_ ] = std::make_shared< RegionItem >();
    regions[ id_ ]->setRawRegion( region_ );
    regions[ id_ ]->setBorderVisible( false );
    regions[ id_ ]->setFillColor( QColor( r_, g_, b_ ) );
    regions[ id_ ]->setOpacity( 0.7 );
    regions[ id_ ]->setVisible( true );

    // placing the regions in front of the images
    if( image != nullptr )
        regions[ id_ ]->setZValue( image->zValue() );
    addItem( regions[ id_ ].get() );
}


void SketchScene::updateRegion( const std::size_t& id_ )
{
    if( regions.find( id_ ) == regions.end() ) return;
    regions[ id_ ]->updateBoundary();
    QGraphicsScene::update();
}


void SketchScene::updateRegions()
{
    for( auto it: regions )
        it.second->updateBoundary();
    QGraphicsScene::update();
}


void SketchScene::clearRegions()
{
    for( auto it: regions )
    {
        RegionItem* reg_ = dynamic_cast< RegionItem* >( (it.second).get() );
        removeItem( reg_ );
        (it.second).reset();
    }
    regions.clear();
    QGraphicsScene::update();
}


void SketchScene::getSelectedRegions()
{
    QList< QGraphicsItem* > items_ = selectedItems();
    if( items_.empty() == true ) return;

    for( auto it: items_ )
    {
        RegionItem* const& reg_ = dynamic_cast< RegionItem* >( it );
        std::size_t id_ = reg_->getIndex();

        emit regionSelected( id_, true );
    }
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

    if( sketch == nullptr ) return;
    sketch->clear();

    // this signal was created to test undo a curve submission. It should be placed in a new method for this aim.
//    emit removeLastCurve( csection_direction, csection_depth );


    QGraphicsScene::update();
}


void SketchScene::submitSketch()
{

    if( sketch == nullptr ) return;

    emit sketchDone( sketch->getCurve(), csection_direction, csection_depth );

    // after submitting a sketch it should be clear
    sketch->clear();

    QGraphicsScene::update();
}


void SketchScene::setSketchColor( const QColor& color_ )
{

    if( sketch == nullptr ) return;
    sketch->setColor( color_ );
    QGraphicsScene::update();
}


void SketchScene::processSketch()
{
    if( sketch->isEmpty() == true ) return;

    /* if( csection_direction == Settings::CrossSection::CrossSectionDirections::Y ) */
    /* { */
        /* sketch->getMonotonicY(); */
    /* } */
    /* else */
    /* { */
        /* sketch->getMonotonicX(); */
    /* } */
}


void SketchScene::endObject()
{

    if( sketch == nullptr ) return;

    // this makes the object does not accept curves anymore
    sketch->setDone();

    emit createObject();

    QGraphicsScene::update();
}



void SketchScene::setSketchingMode( bool status_ )
{
    if( status_ == false )
    {
        current_interaction = UserInteraction::NONE;
        return;
    }

    current_interaction = UserInteraction::SKETCHING;
    sketch_enabled = true;

    if ( sketch != nullptr ) return;
    sketch = std::make_shared< CurveItem >();
    addItem( sketch.get() );
}


void SketchScene::setResizingBoundaryMode( bool status_ )
{
    if( status_ == true )
        current_interaction = UserInteraction::RESIZING_BOUNDARY;
    else
    {
        setSketchingMode();
    }
}


void SketchScene::setGuidedExtrusionMode( bool status_ )
{
    if( status_ == true )
    {
        current_interaction = UserInteraction::GUIDED_EXTRUSION;
        updatePointGuidedExtrusion( QPointF( 0, 0 ) );
    }
    else
    {
        stopPointGuidedExtrusion();
    }
}


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
        current_interaction = UserInteraction::SELECTING_STRATIGRAPHY_OLD;
    }
    else
    {
        setSketchingMode();
    }

    QGraphicsScene::update();


}


void SketchScene::setSelectingStratigraphyMode( bool status_ )
{

    clearSelection();

    if( status_ == true )
    {
        current_interaction = UserInteraction::SELECTING_STRATIGRAPHY;
    }
    else
    {
        setSketchingMode();
    }

    QGraphicsScene::update();


}


void SketchScene::setResizingImageMode( bool status_ )
{
    move_marker->setVisible( status_ );
    resize_marker->setVisible( status_ );

    if( status_ == true )
    {
        current_interaction = UserInteraction::RESIZING_IMAGE;
        resize_marker->setPos( image->getTopRight() );
        move_marker->setPos( image->getOrigin() );
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
        current_interaction = UserInteraction::SELECTING_REGIONS;
    else
    {
        setSketchingMode();
    }

    update();
}


void SketchScene::setSelectingRegionMode( bool status_ )
{
    clearSelection();

    if( status_ == true )
    {
        current_interaction = UserInteraction::SELECTING_REGION;
    }
    else
    {
        setSketchingMode();
    }

    update();
}



void SketchScene::addToSketchesOfSelection()
{

    if( sketch == nullptr ) return;

    std::shared_ptr< CurveItem > sketch_ = std::make_shared< CurveItem >();
    sketch_->setCurve( sketch->getCurve() );

    std::size_t id_ = sketches_of_selection.size();
    sketches_of_selection.push_back( sketch_ );
    addItem( sketches_of_selection[ id_ ].get() );

    emit sendSketchOfSelection( sketch->getCurve(), csection_direction, csection_depth );

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

    emit stopSketchesOfSelection();
    setSketchingMode();
}


void SketchScene::defineBoundaryArea()
{


    if( ( lower.isEmpty() == false ) && ( upper.isEmpty() == false ) )
    {
        QPolygonF pol_upper_ = SketchLibraryWrapper::fromCurve2DToQt( upper.getSubcurve( 0 ) );
        QPolygonF pol_lower_ = SketchLibraryWrapper::fromCurve2DToQt( lower.getSubcurve( 0 ) );

        QPolygonF pol_ = pol_upper_.intersected( pol_lower_ );
        boundary_area->setPolygon( pol_ );
    }

    else if( lower.isEmpty() == false )
    {
        QPolygonF pol_ = SketchLibraryWrapper::fromCurve2DToQt( lower.getSubcurve( 0 ) );
        boundary_area->setPolygon( pol_ );
    }
    else if( upper.isEmpty() == false )
    {
        QPolygonF pol_ = SketchLibraryWrapper::fromCurve2DToQt( upper.getSubcurve( 0 ) );
        boundary_area->setPolygon( pol_ );

    }
    else
    {
        boundary_area->clear();
    }

    boundary_area->setVisible( true );
    boundary_area->setZValue( image->zValue() + 1 );
    boundary_area->update();

    update();
}



void SketchScene::defineLowerBoundaryCurve( const PolyCurve& boundary_ )
{
    lower = boundary_;
    defineBoundaryArea();
}


void SketchScene::defineUpperBoundaryCurve( const PolyCurve& boundary_ )
{
    upper = boundary_;
    defineBoundaryArea();
}


void SketchScene::clearLowerBoundaryCurve()
{
    lower.clear();
    defineBoundaryArea();
}


void SketchScene::clearUpperBoundaryCurve()
{
    upper.clear();
    defineBoundaryArea();
}



void SketchScene::updatePointGuidedExtrusion( const QPointF& p_ )
{
    trajectory_point->setVisible( true );
    trajectory_point->setPos( p_ );
    update();
}


void SketchScene::stopPointGuidedExtrusion()
{
    trajectory_point->setVisible( false );
    setSketchingMode();
    update();
}


void SketchScene::submitSketchGuidedExtrusion()
{
    if( sketch == nullptr ) return;

    emit sketchDoneGuidedExtrusion( sketch->getCurve() );
    sketch->clear();

    QGraphicsScene::update();
}


void SketchScene::showDipAnglePicture( bool status_, const QPixmap& pix_ )
{
    if( dipangle == nullptr ) return;

    if( status_ == false )
    {
        setSketchingMode();
        dipangle->setVisible( false );
        QGraphicsScene::update();
        return;
    }

    current_interaction =  UserInteraction::NONE;
    dipangle->setImage( pix_ );
    dipangle->setVisible( true );
    dipangle->setZValue( image->zValue() + 1 );

    QGraphicsScene::update();
}



void SketchScene::updateDipAnglePicture( const QPixmap& pix_ )
{
    if( dipangle == nullptr ) return;
    dipangle->setImage( pix_ );
}


void SketchScene::setDipAnglePictureMovable( bool status_ )
{
    if( dipangle == nullptr ) return;
    dipangle->setMovable( status_ );

    if( status_  == false )
        setSketchingMode();
    else
        current_interaction =  UserInteraction::NONE;
}


void SketchScene::setAxesVisible( bool status_ )
{
    axes.setVisible( status_ );
    update();
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


void SketchScene::revertVerticalExaggerationInAxes( QMatrix matrix_, double scale_ )
{

    // first to revert the matrix scaled with the vertical exaggeration applied on the axes
    axes.setMatrix( matrix_.inverted().scale( 1, -1 ), false );

    if( volume1 == nullptr ) return;

    // after pass the real height of the volume and the scale applied to the method
    // which will scale the axes, and keep the text unchanged
    std::shared_ptr< Volume > volume_ = volume1->getRawVolume();
    axes.updateVerticalExaggeration( scale_, volume_->getHeight() );


    // after scaling the models, ensure all objects can be viewed
    emit ensureObjectsVisibility();
    update();
}


void SketchScene::resetVerticalExaggerationInAxes()
{
    QMatrix m_;

    // first set an unitary matrix to the axes
    axes.setMatrix( m_, false );

    // after set the axes as not scaled
    axes.resetVerticalExaggeration();

    if( volume1 == nullptr ) return;

    // set the real volume height to the axes
    std::shared_ptr< Volume > volume_ = volume1->getRawVolume();
    axes.stopVerticalExaggeration( volume_->getHeight() );
    emit ensureObjectsVisibility();

    update();
}


void SketchScene::updateAxes()
{
    //    if( volume1 == nullptr ) return;
    //    std::shared_ptr< Volume > volume_ = volume1->getRawVolume();
    //    axes.updateVerticalExaggeration( 1.0, volume_->getHeight() );
    //    update();
}


void SketchScene::mousePressEvent( QGraphicsSceneMouseEvent *event_ )
{

    QPointF p_ = event_->scenePos();


    if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::SKETCHING ) )
    {
        sketch->create( p_ );
    }

    else if( ( event_->buttons() & Qt::RightButton ) && ( current_interaction == UserInteraction::SKETCHING )  )
    {
        submitSketch();
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::RESIZING_BOUNDARY )  )
    {
        volume1->setStartPoint( p_ );
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::CREATE_REGION )  )
    {
        //        regions[ nregions ] ->addPoint( p_ );
    }


    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::SELECTING_STRATIGRAPHY ) )
    {
        sketch->create( p_ );
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::SELECTING_STRATIGRAPHY_OLD ) )
    {
        getSelectedStratigraphies();
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::SELECTING_REGION ) )
    {
        emit getRegionByPoint( p_.x(), p_.y(), csection_depth, csection_direction );
    }


    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::GUIDED_EXTRUSION ) )
    {
        emit sendPointGuidedExtrusion( p_.x(), p_.y(), csection_depth, csection_direction );
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::TRAJECTORY_GUIDED ) )
    {
        sketch->create( p_ );
    }

    else if( ( event_->buttons() & Qt::RightButton ) && ( current_interaction == UserInteraction::TRAJECTORY_GUIDED )  )
    {
        submitSketchGuidedExtrusion();
        //       submitSketch();
    }

    QGraphicsScene::mousePressEvent( event_ );
    //    update();

}


void SketchScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event_ )
{

    if( current_interaction == UserInteraction::SELECTING_STRATIGRAPHY )
    {
        // send the sketches to rules_processor
        removeSketchesOfSelection();
    }

    else if( current_interaction == UserInteraction::SKETCHING )
    {
        endObject();
    }

    else if( current_interaction == UserInteraction::SELECTING_REGIONS )
    {
        getSelectedRegions();
    }



    QGraphicsScene::mouseDoubleClickEvent( event_ );
    update();

}


void SketchScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event_ )
{
    QPointF p_ = event_->scenePos();

    if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::SKETCHING )  )
    {
        sketch->add(  p_ );
    }
    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::RESIZING_BOUNDARY )  )
    {
        volume1->setEndPoint( p_ );
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::SELECTING_STRATIGRAPHY )  )
    {
        sketch->add(  p_ );
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::RESIZING_IMAGE )  )
    {
        if( resize_marker->isSelected() )
        {
            image->resizeRectangle( p_ );
            move_marker->setPos( image->getOrigin() );
        }
        else if( move_marker->isSelected() )
        {
            image->moveRectangle( p_ );
            resize_marker->setPos( image->getTopRight() );
        }
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::GUIDED_EXTRUSION ) )
    {
        emit sendPointGuidedExtrusion( p_.x(), p_.y(), csection_depth, csection_direction );
    }

    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::TRAJECTORY_GUIDED ) )
    {
        sketch->add(  p_ );
    }


    QGraphicsScene::mouseMoveEvent( event_ );
    update();

}


void SketchScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event_ )
{

    QPointF p_ = event_->scenePos();

    if( ( event_->button() == Qt::LeftButton ) && ( current_interaction == UserInteraction::SKETCHING )  )
    {
        sketch->connect();
    }

    else if( ( event_->button() == Qt::LeftButton ) && ( current_interaction == UserInteraction::RESIZING_BOUNDARY )  )
    {
        volume1->setEndPoint( p_ );

        emit resizeVolumeDimensions( csection_direction, static_cast< double >( volume1->boundingRect().width() ), static_cast< double >( volume1->boundingRect().height() ) );
    }

    else if( current_interaction == UserInteraction::SELECTING_STRATIGRAPHY_OLD )
    {
        getSelectedStratigraphies();
    }

    else if( current_interaction == UserInteraction::SELECTING_STRATIGRAPHY )
    {
        addToSketchesOfSelection();

    }


    else if(  current_interaction == UserInteraction::RESIZING_IMAGE )
    {
        updateImageinCrossSection();
    }


    else if( ( event_->buttons() & Qt::LeftButton ) && ( current_interaction == UserInteraction::TRAJECTORY_GUIDED ) )
    {
        sketch->connect();
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


    gv_->centerOn( sceneRect().center() );

    QGraphicsScene::wheelEvent( event_ );
    update();

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

    if( image != nullptr )
        delete image;
    image = nullptr;

    if( resize_marker != nullptr )
        delete resize_marker;
    resize_marker = nullptr;

    if( move_marker != nullptr )
        delete move_marker;
    move_marker = nullptr;

    if( boundary_area != nullptr )
    {   boundary_area->clear();
        delete boundary_area;
    }
    boundary_area = nullptr;

    if( dipangle  != nullptr )
        delete dipangle;
    dipangle = nullptr;


    if( trajectory_point != nullptr )
        delete trajectory_point;
    trajectory_point = nullptr;

    for( auto it: cross_sections )
        (it.second).reset();
    cross_sections.clear();

    for( auto it: stratigraphies )
        (it.second).reset();
    stratigraphies.clear();

    for( auto it: regions )
        (it.second).reset();
    regions.clear();

    for( auto sk_: sketches_of_selection )
    {
        sk_->clear();
        sk_.reset();
    }
    sketches_of_selection.clear();

    csection_depth = 0.0;
    sketch_enabled = true;
    factor = 1;

    lower.clear();
    upper.clear();

    csection_direction = Settings::CrossSection::DEFAULT_CSECTION_DIRECTION;
    current_interaction = UserInteraction::SKETCHING;
    sketch_enabled = true;

}


SketchScene::~SketchScene()
{

    clearScene();


}
