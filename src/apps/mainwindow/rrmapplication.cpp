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



#include "rrmapplication.h"
#include "mainwindow.h"

#include "./core/models/controller.h"
#include "./sketching/sketchingcontroller.h"
#include "./3dview/view3dcontroller.h"



std::size_t Object::number_of_objects = 0;
std::size_t CrossSection::number_of_csections = 0;



RRMApplication::RRMApplication(const RRMApplication & app_)
{
    this->controller = app_.controller;

}


RRMApplication& RRMApplication::operator=(const RRMApplication & app_)
{
    this->controller = app_.controller;
    return *this;
}



void RRMApplication::setMainWindow( MainWindow* const& window_ )
{
    window = window_;
    setController();
}

void RRMApplication::setController()
{
    controller = window->controller;
}


///================================================================================


void RRMApplication::init()
{

    controller->init();
    emit startApplication();

    setDiscretization();

}


void RRMApplication::setDiscretization()
{
    emit updateDiscretization( controller->getCurrentDiscretization() );
    double min_ = 0.0, max_ = 0.0;
    controller->getCurrentRange( min_, max_ );
    emit updateRange( min_, max_ );
}



void RRMApplication::moveMainCrossSection( double depth_ )
{
    if( controller->getMainCrossSection()->getDirection() == Settings::CrossSection::CrossSectionDirections::Y )
    {
        controller->moveTopViewCrossSection( depth_ );
        emit updateTopViewCrossSection();
        return;
    }

    controller->moveMainCrossSection( depth_ );
    emit updateMainCrossSection();

}


void RRMApplication::changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections dir_ )
{
    controller->changeMainCrossSectionDirection( dir_ );
    setDiscretization();
    emit updateMainCrossSection();
}


void RRMApplication::addFixedCrossSection( double depth_, QColor color_ )
{
    controller->addCrossSection( controller->getMainCrossSection()->getDirection(), depth_ );
    emit addFixedCrossSectionWindow( controller->getMainCrossSection()->getDirection(), depth_/*, color_*/ );
}


void RRMApplication::removeFixedCrossSection( double depth_ )
{
    controller->removeCrossSection( controller->getMainCrossSection()->getDirection(), depth_ );
    emit removeFixedCrossSectionWindow( controller->getMainCrossSection()->getDirection(), depth_/*, color_*/ );
}



void RRMApplication::addCurveToObject( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    controller->addCurveToObject( dir_, depth_, curve_ );
    emit updateObjects();

}


void RRMApplication::addTrajectoryToObject( const PolyCurve& curve_ )
{
    if( controller->getMainCrossSection()->getDirection() ==
            Settings::CrossSection::CrossSectionDirections::Y )
    {
        controller->addCurveToObject( Settings::CrossSection::CrossSectionDirections::Y, 0.0, curve_ );
        emit updateObjects();
        return;
    }

    controller->addTrajectoryToObject( curve_ );
    emit updateTrajectories();


}


void RRMApplication::createObjectSurface()
{

    bool status_ = controller->createObjectSurface();
    if( status_ == false ) return;

    emit addObject( controller->getCurrentObject() );
    emit updateObjects();

    checkUndoRedo();

}



void RRMApplication::setStratigraphicRule( const Settings::Stratigraphy::StratigraphicRules& rules_ )
{
    if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE )
        controller->setRemoveAbove();
    else if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
        controller->setRemoveAboveIntersection();
    else if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW )
        controller->setRemoveBelow();
    else if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
        controller->setRemoveBelowIntersection();

}


void RRMApplication::reset()
{

    emit resetApplication();
    controller->clear();

    init();


}


void RRMApplication::previewLastTrajectory()
{
    controller->addLastTrajectoryToObject();
    emit updateObjects();
}

void RRMApplication::undo()
{
    bool status_ = controller->undo();
    if( status_ == false ) return;

    emit updateObjects();

    checkUndoRedo();
    checkPreserveStatus();

}


void RRMApplication::redo()
{
    bool status_ = controller->redo();
    if( status_ == false ) return;

    emit updateObjects();

    checkUndoRedo();
    checkPreserveStatus();
}


void RRMApplication::checkUndoRedo()
{
    bool undo_ = controller->canUndo();
    bool redo_ = controller->canRedo();

    window->ac_undo->setEnabled( undo_ );
    window->ac_redo->setEnabled( redo_ );
}


void RRMApplication::checkPreserveStatus()
{
     bool above_ = true;//controller->isDefineAboveActive();
     bool below_ = true;//controller->isDefineBelowActive();

     window->ac_sketch_above->setChecked( above_ );
     window->ac_sketch_below->setChecked( below_ );

}

///================================================================================






//RRMApplication::RRMApplication(MainWindow *mw_ ): mainwindow( mw_ )
//{
//    mainwindow->controller = new Controller();

//}



//void RRMApplication::setSiderBarVisibility( bool status_ )
//{
//    mainwindow->dw_object_properties->setVisible( status_ );
//    mainwindow->dw_object_tree->setVisible( status_ );
//}


//void RRMApplication::setDefaultRule( Settings::Stratigraphy::StratigraphicRules rule_ )
//{
//    if( rule_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE )
//    {
//        mainwindow->ac_remove_above->setChecked( Variables::ON );
//        mainwindow->controller->setRemoveAbove();
//    }
//    else if( rule_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
//    {
//        mainwindow->ac_remove_above_int->setChecked( Variables::ON );
//        mainwindow->controller->setRemoveAboveIntersection();
//    }
//    else if( rule_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW )
//    {
//        mainwindow->ac_remove_below->setChecked( Variables::ON );
//        mainwindow->controller->setRemoveBelow();
//    }
//    else if( rule_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
//    {
//        mainwindow->ac_remove_below_int->setChecked( Variables::ON );
//        mainwindow->controller->setRemoveBelowIntersection();
//    }
//    else if( rule_ == Settings::Stratigraphy::StratigraphicRules::TRUNCATE )
//    {
////        mainwindow->ac_truncate->setChecked( Variables::ON );
////        mainwindow->controller->setTruncate();
//    }
//}


//void RRMApplication::setDefaultSketchingRegion( Settings::Objects::BounderingRegion sketching_region_ )
//{
//    if( sketching_region_ == Settings::Objects::BounderingRegion::ABOVE )
//        mainwindow->ac_sketch_above->setChecked( Variables::ON );
//    else if( sketching_region_ == Settings::Objects::BounderingRegion::BELOW )
//        mainwindow->ac_sketch_below->setChecked( Variables::ON );
//    else
//    {
//        mainwindow->ac_sketch_above->setChecked( Variables::OFF );
//        mainwindow->ac_sketch_below->setChecked( Variables::OFF );
//    }

//}


//void RRMApplication::setDefaultSiderBarValues()
//{
//    mainwindow->object_properties->clear();
//    mainwindow->object_properties->setEnabledVolumeResize( mainwindow->controller->isVolumeResizable() );
//    mainwindow->object_properties->checkMediumResolution();
//}


//void RRMApplication::setRRMDefaultValuesOnInterface()
//{

//    setSiderBarVisibility( Settings::Application::DEFAULT_SIDEBAR_VISIBILITY );
//    setDefaultRule( Settings::Stratigraphy::DEFAULT_STRAT_RULES );
//    setDefaultSketchingRegion( Settings::Objects::BounderingRegion::NONE );
//    setDefaultSiderBarValues();

//    setCurrentColor( 255, 0, 0 );
//    checkUndoRedo();

//    setVolumeOriginToController( Settings::Volume::VOLUME_ORIGINX, Settings::Volume::VOLUME_ORIGINY, Settings::Volume::VOLUME_ORIGINZ );
//    setVolumeDimensionsToController( Settings::Volume::VOLUME_WIDTH, Settings::Volume::VOLUME_HEIGHT, Settings::Volume::VOLUME_LENGTH );

//    mainwindow->sl_depth_csection->setValue( Settings::Volume::VOLUME_LENGTH );

//}



//void RRMApplication::setVolumeOriginToController( double ox_, double oy_, double oz_ )
//{
//    mainwindow->controller->setVolumeOrigin( ox_, oy_, oz_ );
//    getVolumeDimensionsFromController();
//}


//void RRMApplication::setVolumeDimensionsToController( double width_, double height_, double length_ )
//{
//    mainwindow->controller->setVolumeDimensions( width_, height_, length_ );
//    getVolumeDimensionsFromController();
//}


//void RRMApplication::getVolumeDimensionsFromController() const
//{

//    double ox_ = 0, oy_ = 0, oz_ = 0;
//    mainwindow->controller->getVolumeOrigin( ox_, oy_, oz_ );

//    double width_ = 0, height_ = 0, length_ = 0;
//    mainwindow->controller->getVolumeDimensions( width_, height_, length_ );

//    std::size_t disc_ = 1;
//    double step_ = 1.0;
//    mainwindow->controller->setupCrossSectionDiscretization( disc_, step_ );


//    mainwindow->object_properties->changeVolumeSize( ox_ + width_, oy_ + height_, oz_ + length_ );
//    mainwindow->sl_depth_csection->setDiscretization( static_cast< int > ( disc_ ) );
//    mainwindow->sl_depth_csection->setRange( oz_,  oz_ + length_ );
//    mainwindow->sl_depth_csection->setValue( Settings::Volume::VOLUME_LENGTH );


//}


//void RRMApplication::changeVolumeDimension( const Settings::CrossSection::CrossSectionDirections& dir_, double value_ )
//{

//    double ox_ = 0, oy_ = 0, oz_ = 0;
//    mainwindow->controller->getVolumeOrigin( ox_, oy_, oz_ );

//    double width_ = 0, height_ = 0, length_ = 0;
//    mainwindow->controller->getVolumeDimensions( width_, height_, length_ );


//    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
//    {
//        mainwindow->controller->setVolumeDimensions( value_, height_, length_ );
//    }
//    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
//    {
//        mainwindow->controller->setVolumeDimensions( width_, value_, length_ );
//    }
//    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
//    {
//        std::size_t disc_ = 1;
//        double step_ = 1.0;
//        mainwindow->controller->setVolumeDimensions( width_, height_, value_ );
//        mainwindow->controller->setupCrossSectionDiscretization( disc_, step_ );
//        mainwindow->sl_depth_csection->setDiscretization( static_cast< int > ( disc_ ) );
//        mainwindow->sl_depth_csection->setRange( oz_,  oz_ + value_ );
//        mainwindow->sl_depth_csection->setValue( Settings::Volume::VOLUME_LENGTH );
//    }

//}


//void RRMApplication::changeVolumeDimensions( const Settings::CrossSection::CrossSectionDirections& dir_, double dim1_, double dim2_ )
//{
//    double ox_ = 0, oy_ = 0, oz_ = 0;
//    mainwindow->controller->getVolumeOrigin( ox_, oy_, oz_ );

//    double width_ = 0, height_ = 0, length_ = 0;
//    mainwindow->controller->getVolumeDimensions( width_, height_, length_ );


//    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
//    {
//        mainwindow->controller->setVolumeDimensions( width_, dim2_, dim1_ );
//    }
//    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
//    {
//        mainwindow->controller->setVolumeDimensions( dim1_, height_, dim2_ );
//    }
//    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
//    {
//        mainwindow->controller->setVolumeDimensions( dim1_, dim2_, length_ );
//    }

//    getVolumeDimensionsFromController();
//}


//void RRMApplication::setVolumeName( std::size_t index_, const std::string& name_ )
//{
//    mainwindow->controller->setVolumeName( name_ );
//}


//void RRMApplication::setVolumeVisible( std::size_t index_, bool status_ )
//{
//    if( index_ == 0 )
//        mainwindow->controller->setVolumeVisibility( status_ );
//     updateSketchingCanvas();
//}




//void RRMApplication::setCurrentCrossSection( double depth_ )
//{
//    mainwindow->controller->setCurrentCrossSection( depth_ );
//    updateSketchingCanvas();
//}




//void RRMApplication::setObjectName( std::size_t index_, const std::string& name_ )
//{
//    mainwindow->controller->setObjectName( index_, name_ );
//}


//void RRMApplication::setObjectVisible( std::size_t index_, bool status_ )
//{
//    mainwindow->controller->setObjectVisibility( index_, status_ );
//    updateSketchingCanvas();
//}


//void RRMApplication::setObjectColor( std::size_t index_, int r_, int g_, int b_ )
//{
//    mainwindow->controller->setObjectColor( index_, r_, g_, b_ );
//    updateSketchingCanvas();
//}


//void RRMApplication::saveObjectInformation( const std::string & text_ )
//{
//    QList< QTreeWidgetItem* > items = mainwindow->object_tree->selectedItems();
//    if( items.empty() == true ) return;

//    std::size_t index_ = static_cast< ObjectTreeItem *> (items[ 0 ] )->getIndex();
//    mainwindow->controller->saveObjectInformation( index_, text_ );
//}


//void RRMApplication::getObjectInformation( QTreeWidgetItem* const& item_ ) const
//{
//    ObjectTreeItem* obj_item_ = static_cast< ObjectTreeItem* >( item_ );

//    if( obj_item_->getType() == Settings::Objects::ObjectType::VOLUME )
//        mainwindow->object_properties->setCurrentIndex( 0 );
//    else if( obj_item_->getType() == Settings::Objects::ObjectType::STRATIGRAPHY )
//    {
//        mainwindow->object_properties->setCurrentIndex( 1 );
//        std::string text_ = mainwindow->controller->getObjectInformation( obj_item_->getIndex() );
//        std::string name_ = mainwindow->controller->getObjectName( obj_item_->getIndex() );
//        mainwindow->object_properties->loadObjectInformation( name_, text_ );
//    }

//}




//void RRMApplication::setRegionName( std::size_t index_, const std::string& name_ )
//{
//    mainwindow->controller->setRegionName( index_, name_ );
//}


//void RRMApplication::setRegionVisible( std::size_t index_, bool status_ )
//{
//    mainwindow->controller->setRegionVisibility( index_, status_ );
//    updateSketchingCanvas();
//}


//void RRMApplication::setRegionColor( std::size_t index_, int r_, int g_, int b_ )
//{
//    mainwindow->controller->setRegionColor( index_, r_, g_, b_ );
//    mainwindow->flow_window->updateRegionColor( index_, r_, g_, b_ );
//    updateSketchingCanvas();
//}






//void RRMApplication::updateSketchingCanvas()
//{
//    mainwindow->sketch_window->updateCanvas();
//    mainwindow->sketch_topview_window->updateCanvas();
//}


//void RRMApplication::acceptSketchingCurve( const PolyCurve& curve_, double depth_ )
//{

//    bool status_ = mainwindow->controller->addObjectCurve( curve_, depth_ );
//    if( status_ == false ) return;

//    CrossSection* csection_ = mainwindow->controller->getCrossSection( depth_ );
//    mainwindow->sketch_topview_window->addCrossSection( csection_ );
    
//    updateSketchingCanvas();
    
//}


//void RRMApplication::acceptSketchingTrajectory( const PolyCurve& curve_ )
//{

//    bool status_ = mainwindow->controller->addObjectTrajectory( curve_ );
//    if( status_ == false ) return;

//    updateSketchingCanvas();

//}


//void RRMApplication::createObjectSurface()
//{
//    bool status_ = mainwindow->controller->createObjectSurface();
//    if( status_ == false ) return;

//    mainwindow->controller->addObject();
//    mainwindow->object_properties->setEnabledVolumeResize( mainwindow->controller->isVolumeResizable() );

//    mainwindow->sketch_window->addObject( mainwindow->controller->getCurrentObject().get() );
//    mainwindow->sketch_topview_window->addTrajectory( mainwindow->controller->getCurrentObject().get() );

//    updateSketchingCanvas();
//    defineRandomColor();
//    checkUndoRedo();
//}







//void RRMApplication::setSketchAbove( bool status_ )
//{

//    bool below_done = mainwindow->controller->isDefineBelowObjectSelected();
//    if( below_done == false )
//    {
//        mainwindow->ac_sketch_above->setChecked( !status_ );
//        return;
//    }

//    bool enabled_ = mainwindow->controller->enableCreateAbove( status_ );


//    if( status_ == false )
//    {
//        if( enabled_ == false )
//        {
//            mainwindow->ac_sketch_above->setChecked( false );
//            setModeSketching();
//        }

//        updateSketchingCanvas();

//    }

//    else if(  status_ == true )
//    {
//        if( enabled_ == true )
//            setModeSelecting();
//        else
//            setModeSketching();
//    }

//}


//void RRMApplication::setSketchBelow( bool status_ )
//{

//    bool above_done = mainwindow->controller->isDefineAboveObjectSelected();
//    if( above_done == false )
//    {
//        mainwindow->ac_sketch_below->setChecked( !status_ );
//        return;
//    }

//    bool enabled_ = mainwindow->controller->enableCreateBelow( status_ );

//    if( status_ == false )
//    {

//        if( enabled_ == false )
//        {
//            mainwindow->ac_sketch_below->setChecked( false );
//            setModeSketching();
//        }

//         updateSketchingCanvas();

//    }

//    else if(  status_ == true )
//    {
//        if( enabled_ == true )
//            setModeSelecting();
//        else
//            setModeSketching();
//    }


//}




//void RRMApplication::setObjectAsBoundering( std::size_t index_ )
//{
//    mainwindow->controller->setObjectAsBoundering( index_ );
//    setModeSketching();
//}




//void RRMApplication::setModeSelecting()
//{
//    mainwindow->sketch_window->setModeSelecting();
//    updateSketchingCanvas();
//}


//void RRMApplication::setModeSketching()
//{
//    mainwindow->sketch_window->setModeSketching();
//    updateSketchingCanvas();
//}





//void RRMApplication::save( const std::string& filename_ )
//{
//    mainwindow->controller->saveFile( filename_ );
//}


//void RRMApplication::load( const std::string& filename_ )
//{
//    clear();

//    Controller::MeshResolution resol_;

//    mainwindow->controller->loadFile( filename_, resol_ );

//    if ( resol_ == Controller::MeshResolution::LOW )
//        mainwindow->object_properties->checkLowResolution();
//    else if ( resol_ == Controller::MeshResolution::MEDIUM )
//        mainwindow->object_properties->checkMediumResolution();
//    else if ( resol_ == Controller::MeshResolution::HIGH )
//        mainwindow->object_properties->checkHighResolution();

//    getVolumeDimensionsFromController();
//    mainwindow->object_properties->setEnabledVolumeResize( mainwindow->controller->isVolumeResizable() );
//    checkUndoRedo();
//    checkSketchStatus();
//    initSketchingApp();
//}





//void RRMApplication::undo()
//{
//    mainwindow->controller->undo();
//    updateSketchingCanvas();
//    checkUndoRedo();
//    checkSketchStatus();
//}


//void RRMApplication::redo()
//{
//    mainwindow->controller->redo();
//    updateSketchingCanvas();
//    checkUndoRedo();
//    checkSketchStatus();
//}





//void RRMApplication::checkUndoRedo()
//{
//    if( mainwindow->controller == nullptr ) return;

//    mainwindow->ac_undo->setEnabled(  mainwindow->controller->canUndo() );
//    mainwindow->ac_redo->setEnabled(  mainwindow->controller->canRedo() );


//    updateSketchingCanvas();
//}


//void RRMApplication::checkSketchStatus()
//{
//    if( mainwindow->controller == nullptr ) return;

//    mainwindow->ac_sketch_above->setChecked(  mainwindow->controller->isDefineAboveActive() );
//    mainwindow->ac_sketch_below->setChecked(  mainwindow->controller->isDefineBelowActive() );

//    updateSketchingCanvas();
//}





//void RRMApplication::setCurrentColor( int r_, int g_, int b_ )
//{
//    mainwindow->controller->setCurrentColor( r_, g_, b_ );
//    mainwindow->sketch_window->setCurrentColor( r_, g_, b_ );
//    mainwindow->sketch_topview_window->setCurrentColor( r_, g_, b_ );
//}


//void RRMApplication::defineRandomColor()
//{
//    std::random_device rd;
//    std::mt19937 eng( rd() );
//    std::uniform_int_distribution< size_t > distr( 0, 255 );

//    int r_ = distr( eng );
//    int g_ = distr( eng );
//    int b_ = distr( eng );

//    setCurrentColor( r_, g_, b_ );


//}


//void RRMApplication::screenshot()
//{
//    mainwindow->canvas3d->screenshot();
//}



//void RRMApplication::clearInterface()
//{
//    mainwindow->sl_depth_csection->clear();
//    mainwindow->object_properties->clear();
//}


//void RRMApplication::clear()
//{
//    clearInterface();


//    mainwindow->sketch_window->clear();
//    mainwindow->sketch_topview_window->clear();
//    mainwindow->controller->clear();

//    closeFlowDiagnostics();

//    mainwindow->controller->init();
//    setRRMDefaultValuesOnInterface();
//    mainwindow->controller->addMainCrossSection( Settings::CrossSection::DEFAULT_CSECTION_DIRECTION, Settings::CrossSection::INITIAL_CSECTIONZ_POSITION );
//    mainwindow->controller->addTopViewCrossSection( Settings::CrossSection::INITIAL_CSECTIONY_POSITION );

//}


//void RRMApplication::restart()
//{
//    clear();
//    initSketchingApp();
//}



//void RRMApplication::addCrossSectionCanvas( double depth_, QColor color_ )
//{
//    bool status_ = mainwindow->controller->addFixedCrossSection( depth_ );
//    if( status_ == false ) return;
//    mainwindow->sketch_window->addFixedCrossSectionCanvas( mainwindow->controller->getCrossSection( depth_ ), color_ );
//    updateSketchingCanvas();
//}


//void RRMApplication::removeCrossSectionCanvas( double depth_ )
//{
//    bool status_ = mainwindow->sketch_window->removeFixedCrossSectionCanvas( depth_ );
//    if( status_ == false ) return;

//    status_ = mainwindow->controller->removeFixedCrossSection( depth_ );

//}


//void RRMApplication::setImageToCrossSection( double depth_, std::string file_, double ox_, double oy_, double x_, double y_ )
//{
//    mainwindow->controller->setImageCrossSection( depth_, file_, ox_, oy_, x_, y_ );
//}


//void RRMApplication::getHeightMapTopView()
//{
//    double ox_, oy_, oz_;
//    mainwindow->controller->getVolumeOrigin( ox_, oy_, oz_ );
//    double w_, h_, l_;
//    mainwindow->controller->getVolumeDimensions( w_, h_, l_ );

//    double max_ = ( l_ > h_ ? l_:h_ );
//    std::string image_ = mainwindow->canvas3d->sendImage( oy_, oy_ + h_, w_, l_ );
//    mainwindow->sketch_topview_window->setTopViewImage( image_ );

//}


//void RRMApplication::removeCurveFromObject(  double depth_, std::size_t index_ )
//{
//     mainwindow->controller->removeObjectCurve( index_, depth_ );
//     updateSketchingCanvas();
//}


//void RRMApplication::removeImageFromCrossSection( double depth_ )
//{
//    mainwindow->controller->clearImageCrossSection( depth_ );
//    updateSketchingCanvas();
//}


//void RRMApplication::setImageToTopView( std::string file_, double ox_, double oy_, double x_, double y_ )
//{
//    mainwindow->controller->setTopViewImage( file_, ox_, oy_, x_, y_ );
//}


//void RRMApplication::removeImageFromTopView()
//{
//    mainwindow->controller->removeTopViewImage();
//}


//void RRMApplication::startFlowDiagnostics()
//{
//    mainwindow->ac_undo->setEnabled( false );
//    mainwindow->ac_redo->setEnabled( false );

//    mainwindow->dw_sketchwindow->setVisible( false );
//    mainwindow->dw_topview_window->setVisible( false );
//    mainwindow->dw_flow_window->setVisible( true );

//    std::map< std::size_t, Volume::Color > regions_map_ ;
//    mainwindow->controller->getOutputVolume( regions_map_ );
//    if( regions_map_.empty() == true ) return;

//    std::map< int, std::vector< int > > color_regions_ ;
//    for( auto it: regions_map_ )
//    {

//        int index_ = static_cast< int >( it.first );
//        std::vector< int > color_;
//        color_.resize( 3 );

//        color_[ 0 ] = it.second.r;
//        color_[ 1 ] = it.second.g;
//        color_[ 2 ] = it.second.b;

//        color_regions_[ index_ ] = color_;

//    }


//    mainwindow->flow_window->loadSurfacesfromSketch1();
//    mainwindow->flow_window->setRegions( color_regions_ );
//}


//void RRMApplication::closeFlowDiagnostics()
//{

//    mainwindow->ac_undo->setEnabled( true );
//    mainwindow->ac_redo->setEnabled( true );

//    mainwindow->dw_flow_window->setVisible( false );
//    mainwindow->ac_output_volume->setChecked( false );
//    mainwindow->dw_sketchwindow->setVisible( true );
//    mainwindow->dw_topview_window->setVisible( true );

//    mainwindow->controller->hideRegions();

//}


//void RRMApplication::getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
//{
//     mainwindow->controller->getLegacyMeshes( points, nu, nv, num_extrusion_steps ) ;
//}


//void RRMApplication::getSurfacesMeshes( std::vector< FlowWindow::TriangleMesh >& triangles_meshes,
//                                        std::vector< FlowWindow::CurveMesh>& left_curves,
//                                        std::vector< FlowWindow::CurveMesh >& right_curves,
//                                        std::vector< FlowWindow::CurveMesh > & front_curves,
//                                        std::vector< FlowWindow::CurveMesh >& back_curves )
//{
//    std::vector< Controller::TriangleMesh > meshes;
//    std::vector< Controller::CurveMesh > lcurves;
//    std::vector< Controller::CurveMesh > rcurves;
//    std::vector< Controller::CurveMesh > fcurves;
//    std::vector< Controller::CurveMesh > bcurves;

//    mainwindow->controller->setSurfacesMeshes( meshes, lcurves, rcurves, fcurves, bcurves );

//    for( std::size_t i = 0; i < meshes.size(); ++i )
//    {
//        FlowWindow::TriangleMesh t;
//        t.vertex_list = meshes[i].vertex_list;
//        t.face_list = meshes[i].face_list;
//        triangles_meshes.push_back( t );
//    }

//    for ( std::size_t i = 0; i < lcurves.size(); ++i )
//    {
//        FlowWindow::CurveMesh cm_lb, cm_rb, cm_fb, cm_bb;

//        std::copy( lcurves[i].vertex_list.begin(), lcurves[i].vertex_list.end(), std::back_inserter(cm_lb.vertex_list) );
//        std::copy( lcurves[i].edge_list.begin(), lcurves[i].edge_list.end(), std::back_inserter(cm_lb.edge_list) );

//        std::copy( rcurves[i].vertex_list.begin(), rcurves[i].vertex_list.end(), std::back_inserter(cm_rb.vertex_list) );
//        std::copy( rcurves[i].edge_list.begin(), rcurves[i].edge_list.end(), std::back_inserter(cm_rb.edge_list) );


//        std::copy( fcurves[i].vertex_list.begin(), fcurves[i].vertex_list.end(), std::back_inserter(cm_fb.vertex_list) );
//        std::copy( fcurves[i].edge_list.begin(), fcurves[i].edge_list.end(), std::back_inserter(cm_fb.edge_list) );


//        std::copy( bcurves[i].vertex_list.begin(), bcurves[i].vertex_list.end(), std::back_inserter(cm_bb.vertex_list) );
//        std::copy( bcurves[i].edge_list.begin(), bcurves[i].edge_list.end(), std::back_inserter(cm_bb.edge_list) );


//        left_curves.push_back( cm_lb );
//        right_curves.push_back( cm_rb );
//        front_curves.push_back( cm_fb );
//        back_curves.push_back( cm_bb );
//    }
//}


//void RRMApplication::getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces )
//{
//    std::vector< int > regions_ = mainwindow->controller->getTetrahedronsRegions( vertices, edges, faces );

//    std::map< int, std::vector< float > > colors_;
//    for( auto it: regions_ )
//    {
//        int r = 255, g = 0, b = 0;
//        mainwindow->controller->getRegionColor( it, r, g, b );

//        std::vector< float > color_;
//        color_.resize( 3 );

//        color_[ 0 ]= static_cast< float >( r/255.f );
//        color_[ 1 ]= static_cast< float >( g/255.f );
//        color_[ 2 ]= static_cast< float >( b/255.f );

//        colors_[ it ] = color_;

//    }

//    mainwindow->flow_window->setTetrahedronRegions( regions_, colors_ );

//}


//void RRMApplication::setLowResolution()
//{
//    mainwindow->controller->setMeshResolution( Controller::MeshResolution::LOW );
//    getVolumeDimensionsFromController();
//}

//void RRMApplication::setMediumResolution()
//{
//    mainwindow->controller->setMeshResolution( Controller::MeshResolution::MEDIUM );
//    getVolumeDimensionsFromController();
//}

//void RRMApplication::setHighResolution()
//{
//    mainwindow->controller->setMeshResolution( Controller::MeshResolution::HIGH );
//    getVolumeDimensionsFromController();
//}


//void RRMApplication::enablePreview( bool status_ )
//{
//    mainwindow->controller->setPreviewEnabled( status_ );
//    updateSketchingCanvas();
//}

//void RRMApplication::exportToIRAP()
//{
//    mainwindow->controller->exportToIrapGrid();
//}
