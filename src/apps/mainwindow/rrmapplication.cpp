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


#include <QFileDialog>

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

    double ox_, oy, oz, w_, h_, d_;
    controller->getVolumeGeometry( ox_, oy, oz, w_, h_, d_ );
    emit defineVolumeGeometry( ox_, oy, oz, w_, h_, d_ );

    window->object_tree->addInputVolume();
    setDiscretization( Settings::CrossSection::CrossSectionDirections::Z );

}




void RRMApplication::setVolumeName( const std::string& name_ )
{
    controller->setVolumeName( name_ );
}


void RRMApplication::setVolumeVisible( bool status_ )
{
    controller->setVolumeVisibility( status_ );
    emit updateVolume();
}


void RRMApplication::setVolumeDimensions( const  Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ )
{
    std::cout << "width_: " << width_ << ", height: " << height_ << std::endl << std::flush;

    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        controller->setVolumeLenght( width_ );
        controller->setVolumeHeight( height_ );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        controller->setVolumeWidth( width_ );
        controller->setVolumeLenght( height_ );
    }
    else
    {
        controller->setVolumeWidth( width_ );
        controller->setVolumeHeight( height_ );
    }

    emit updateVolume();

    double ox_, oy, oz, w_, h_, d_;
    controller->getVolumeGeometry( ox_, oy, oz, w_, h_, d_ );
    emit defineVolumeGeometry( ox_, oy, oz, w_, h_, d_ );

    setDiscretization( controller->getCurrentDirection() );

}


void RRMApplication::setVolumeWidth( double width_ )
{
    controller->setVolumeWidth( width_ );
    emit updateVolume();

    setDiscretization( controller->getCurrentDirection() );
}


void RRMApplication::setVolumeHeight( double height_ )
{
    controller->setVolumeHeight( height_ );
    emit updateVolume();

    setDiscretization( controller->getCurrentDirection() );
//    setDiscretization( controller->getMainCrossSection()->getDirection() );
}


void RRMApplication::setVolumeDepth( double lenght_ )
{
    controller->setVolumeLenght( lenght_ );
    emit updateVolume();

    setDiscretization( controller->getCurrentDirection() );
//    setDiscretization( controller->getMainCrossSection()->getDirection() );
}




void RRMApplication::setDiscretization( const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    bool inverted_ = ( dir_ == Settings::CrossSection::CrossSectionDirections::Y? true:false );

    emit updateDiscretization( controller->getCurrentDiscretization() );
    double min_ = 0.0, max_ = 0.0;
    controller->getCurrentRange( min_, max_ );
    emit updateRange( min_, max_, inverted_ );
}



void RRMApplication::moveMainCrossSection( double depth_ )
{
//    if( controller->getMainCrossSection()->getDirection() == Settings::CrossSection::CrossSectionDirections::Y )
    if( controller->getCurrentDirection() == Settings::CrossSection::CrossSectionDirections::Y )
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
    setDiscretization( dir_ );

    if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
        emit changeToTopViewDirection();
    else
        emit changeToCrossSectionDirection();

//    emit updateVolume();

}


void RRMApplication::addFixedCrossSection( double depth_, QColor color_ )
{
    controller->addCrossSection( controller->getCurrentDirection(), depth_ );
    emit addFixedCrossSectionWindow( controller->getCurrentDirection(), depth_, color_ );
}


void RRMApplication::removeFixedCrossSection( double depth_ )
{
    controller->removeCrossSection( controller->getCurrentDirection(), depth_ );
    emit removeFixedCrossSectionWindow( controller->getCurrentDirection(), depth_ );
}


void RRMApplication::setImageToCrossSection( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_  )
{
    controller->setImageToCrossSection( file_, dir_, depth_, ox_, oy_, w_, h_ );
    emit updateImageInCrossSection();
}


void RRMApplication::clearImageInCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    controller->clearImageInCrossSection( dir_, depth_ );
    emit updateImageInCrossSection();
}



void RRMApplication::setObjectVisible( std::size_t index_, bool status_ )
{
    controller->setObjectVisibility( index_, status_ );
    emit updateObjects();
}


void RRMApplication::setObjectName( std::size_t index_, const std::string& name_ )
{
    controller->setObjectName( index_, name_ );
}


void RRMApplication::setObjectColor( std::size_t index_, int red_, int green_, int blue_ )
{
    controller->setObjectColor( index_, red_, green_, blue_ );
    emit updateObjects();
}


void RRMApplication::setObjectLog( std::size_t index_, const QString& log_ )
{
    controller->setObjectLog( index_, log_ );
}


void RRMApplication::getObjectLog( std::size_t index_, QString& log_ )
{
    log_ = controller->getObjectLog( index_ );
}


void RRMApplication::setObjectSelectedAsBoundering( const std::size_t& index_ )
{
    controller->setObjectSelectedAsBoundering( index_ );
    emit selectEnabled( "NONE" );
}




void RRMApplication::setCurrentObjectType( const Settings::Objects::ObjectType& type_ )
{
    controller->setCurrentObjectType( type_ );
}


void RRMApplication::addCurveToObject( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{

    bool new_obj_ = controller->getCurrentObject()->isEmpty();

    bool status_ = controller->addCurveToObject( dir_, depth_, curve_ );
    if( status_ == false ) return;

    emit updateObjects();
    emit updateMainCrossSection();
    emit updateTopViewCrossSection();

    if( new_obj_ == true )
    {
        const ObjectPtr& obj_ = controller->getCurrentObject();

        int r_, g_, b_;
        obj_->getColor( r_, g_, b_ );

        window->object_tree->addObject( obj_->getIndex(), obj_->getType(), obj_->getName(), r_, g_, b_ );

        emit disableVolumeResizing();
        emit lockDirection( dir_ );
    }


    if( dir_ != Settings::CrossSection::CrossSectionDirections::Y )
        emit addCrossSection( dir_, depth_ );




}


void RRMApplication::addTrajectoryToObject( const PolyCurve& curve_ )
{
    if( controller->getCurrentDirection() ==
            Settings::CrossSection::CrossSectionDirections::Y )
    {
        addCurveToObject( curve_, Settings::CrossSection::CrossSectionDirections::Y, 0.0 );
        return;
    }

    bool new_obj_ = controller->getCurrentObject()->isEmpty();

    controller->addTrajectoryToObject( curve_ );
    emit updateTrajectories();

    if( new_obj_ == true )
    {
        const ObjectPtr& obj_ = controller->getCurrentObject();
        Settings::CrossSection::CrossSectionDirections dir_ = obj_->getCrossSectionDirection();

        int r_, g_, b_;
        obj_->getColor( r_, g_, b_ );

        window->object_tree->addObject( obj_->getIndex(), obj_->getType(), obj_->getName(), r_, g_, b_ );

        emit disableVolumeResizing();
        emit lockDirection( dir_ );
    }

}


void RRMApplication::removeLastCurve( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    controller->removeCurveFromObject( dir_, depth_ );
    emit updateObjects();
}


void RRMApplication::createObjectSurface()
{


    bool status_ = controller->commitObjectSurface();

    emit updateObjects();


    if( status_ == false ) return;
    checkUndoRedo();
    updateObjectTree();


    emit addObject( controller->getCurrentObject() );
    defineRandomColor();
    emit unlockDirections();
    emit updateMainCrossSection();

}


void RRMApplication::getRegions( bool status_ )
{
    controller->removeRegions();
    window->object_tree->removeDomains();
    window->object_tree->removeRegions();

    emit clearRegions();

    if( status_ == false )
    {
        return;
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // Changed for release:
    //
    /* It was like this before: */
    /* std::vector< std::size_t > surfaces_indexes_ = controller->defineRegions(); */

    /* Changed to: */
    controller->defineRegions();

    std::vector< std::size_t > surfaces_indexes_ = controller->getOrderedSurfacesIndices();
    // This surfaces_indexes_ is later used in the object tree in a way that
    // all surfaces (both active and empty) are counted together to order
    // stratigraphies and structurals
    ///////////////////////////////////////////////////////////////////////////////////////

    window->object_tree->sortStratigraphies( surfaces_indexes_ );
    window->object_tree->addOutputVolume();

    double total_volume_ = controller->getTotalVolume();
    window->object_tree->setTotalVolume( total_volume_ );


    const std::map< std::size_t, RegionsPtr >& regions_ = controller->getRegions();
    for( auto it: regions_ )
    {
        RegionsPtr & reg_ = (it.second);

        int r_, g_, b_;
        double volume_;
        double perc_;

        reg_->getColor( r_, g_, b_ );
        volume_  = reg_->getVolume();

        if( total_volume_ != 0 )
            perc_ = 100*volume_/total_volume_;

        window->object_tree->addRegion( reg_->getIndex(), reg_->getName(), r_, g_, b_, volume_, perc_ );

        emit addRegionCrossSectionBoundary( reg_ );
    }

    emit addRegions();
}


void RRMApplication::setRegionsVisible( bool status_ )
{
    controller->setRegionsVisible( status_ );
    updateRegions();
}


void RRMApplication::setRegionVisible( std::size_t index_, bool status_ )
{
    controller->setRegionVisible( index_, status_ );
    emit updateRegions();
}


void RRMApplication::setRegionName( std::size_t index_, const std::string& name_ )
{
//    controller->setRegionName( index_, name_ );
    emit updateRegions();
}


void RRMApplication::setRegionColor( std::size_t index_, int red_, int green_, int blue_ )
{
    controller->setRegionColor( index_, red_, green_, blue_ );
    emit updateRegions();
}



void RRMApplication::createDomain()
{
    std::size_t id_ = controller->createDomain();
    window->object_tree->createDomain( id_ );
    double volume_ = controller->getDomainVolume( id_ );
    window->object_tree->updateVolumeDomain( id_, volume_ );
}


void RRMApplication::setDomainName( std::size_t index_, const std::string& name_ )
{
    controller->setDomainName( index_, name_ );
}


void RRMApplication::setDomainColor( std::size_t index_, int red_, int green_, int blue_ )
{
    controller->setDomainColor( index_, red_, green_, blue_ );
}


void RRMApplication::addRegionToDomain( std::size_t reg_id_, std::size_t domain_id_ )
{
    controller->addRegionToDomain( reg_id_, domain_id_ );
    double volume_ = controller->getDomainVolume( domain_id_ );
    window->object_tree->updateVolumeDomain( domain_id_, volume_ );
}




void RRMApplication::removeRegionFromDomain( std::size_t reg_id_, std::size_t domain_id_ )
{
    controller->removeRegionFromDomain( reg_id_, domain_id_ );
    double volume_ = controller->getDomainVolume( domain_id_ );
    window->object_tree->updateVolumeDomain( domain_id_, volume_ );
}


void RRMApplication::removeDomain( std::size_t index_ )
{
    controller->removeDomain( index_ );
    window->object_tree->deleteDomain( index_ );
}



void RRMApplication::addRegionsToDomain( std::size_t domain_id_, std::vector< std::size_t > regions_ )
{

    std::vector< std::size_t > regions_added_;
    for( auto id_: regions_ )
    {
        bool status_ = controller->addRegionToDomain( id_, domain_id_ );
        if( status_ == false ) continue;
        regions_added_.push_back( id_ );
    }

    window->object_tree->addRegionsInDomain( domain_id_, regions_added_ );
    double volume_ = controller->getDomainVolume( domain_id_ );
    window->object_tree->updateVolumeDomain( domain_id_, volume_ );
}


void RRMApplication::removeRegionsFromDomains( const std::vector< std::size_t >& regions_, const std::vector< std::size_t >& domains_, bool delete_  )
{
    std::size_t nregions_ = regions_.size();

    std::vector< std::size_t > regions_removed_;
    std::vector< std::size_t > domains_removed_;

    for( std::size_t i = 0; i < nregions_; ++i )
    {
        bool status_ = controller->removeRegionFromDomain(  regions_[ i ], domains_[ i ] );
        if( status_ == true )
        {
            regions_removed_.push_back( regions_[ i ] );
            domains_removed_.push_back( domains_[ i ] );
        }

        double volume_ = controller->getDomainVolume( domains_[ i ] );
        window->object_tree->updateVolumeDomain( domains_[ i ], volume_ );
    }

    if( delete_ == true )
        window->object_tree->removeRegionsOfTheirDomains( regions_removed_, domains_removed_ );
    else
        window->object_tree->removeRegionsOfTheirDomainsNoDelete( regions_removed_, domains_removed_ );


}





void RRMApplication::setSketchAbove( bool status_ )
{
    controller->enablePreserveAbove( status_ );
    emit enablePreserve( "ABOVE ", status_ );
    emit updateBoundary();
}


void RRMApplication::setSketchBelow( bool status_ )
{
    controller->enablePreserveBelow( status_ );
    emit enablePreserve( "BELOW ", status_ );
    emit updateBoundary();
}


void RRMApplication::setSketchRegion( bool status_ )
{
    emit enablePreserve( "REGION", status_ );

    if( status_ == false )
    {
//        controller->clearBounderingArea();
        controller->enablePreserveAbove( false );
        controller->enablePreserveBelow( false );
        window->activatePreserveAbove( false );
        window->activatePreserveBelow( false );
        window->activatePreserveRegion( false );
    }

    emit updateBoundary();

}


void RRMApplication::updateRegionBoundary()
{

    if( window->isRegionEnabled() == false ) return;

    emit updateRegions();


}


// set domains for objecttree
void RRMApplication::loadDomains()
{
    std::vector< std::size_t > domains_ = controller->getDomains();
    for( auto it_: domains_ )
    {
        bool status_ = window->object_tree->createDomain( it_ );
        if( status_ == false ) continue;

        std::set< std::size_t > regions_ = controller->getRegionsFromDomain( it_ );
        window->object_tree->addRegionsInDomain( it_, regions_ );
        double volume_ = controller->getDomainVolume( it_ );
        window->object_tree->updateVolumeDomain( it_, volume_ );
    }

//    return domains_;
}


// set domains when using flow diagnostics
std::vector< std::size_t > RRMApplication::getDomainsToFlowDiagnostics() const
{

    std::vector< std::size_t > domains_ = controller->getDomainsToFlowDiagnostics();
    for( auto it_: domains_ )
    {
        bool status_ = window->object_tree->createDomain( it_ );
        if( status_ == false ) continue;

        std::set< std::size_t > regions_ = controller->getRegionsFromDomain( it_ );
        window->object_tree->addRegionsInDomain( it_, regions_ );
        double volume_ = controller->getDomainVolume( it_ );
        window->object_tree->updateVolumeDomain( it_, volume_ );
    }

    return domains_;
}


void RRMApplication::getRegionByPointAsBoundering( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    bool status_ = controller->setRegionByPointAsBoundering( px_, py_, depth_, dir_ );

    window->activatePreserveAbove( status_ );
    window->activatePreserveBelow( status_ );

    if( status_ == true )
    {
//        controller->getRegionByPointAsBoundering();
        emit updateBoundary();

    }
    else
        setSketchRegion( false );

    emit selectEnabled( "NONE" );

}



void RRMApplication::selectBounderingBySketch(  const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_  )
{
    PolyCurve boundary_;

    bool status_ = controller->setRegionBySketchAsBoundering( curve_, dir_, depth_, boundary_ );
    emit updateBoundary();
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
    emit enableVolumeResizing();
    window->initializeInterface();
    controller->clear();

    init();


}


void RRMApplication::previewLastTrajectory()
{
    controller->addLastTrajectoryToObject();
    emit updateObjects();
}


void RRMApplication::defineCurrentColor( int red_, int green_, int blue_ )
{
    controller->setCurrentColor( red_, green_, blue_ );
    emit setCurrentColor( red_, green_, blue_ );
    emit updateObjects();
}


void RRMApplication::undo()
{
    bool status_ = controller->undo();

    checkUndoRedo();
    checkPreserveStatus();
    emit updateBoundary();

    if( status_ == false ) return;

    emit updateObjects();
    emit updateTrajectories();

    updateObjectTree();

}


void RRMApplication::redo()
{
    bool status_ = controller->redo();

    checkUndoRedo();
    checkPreserveStatus();
    emit updateBoundary();

    if( status_ == false ) return;

    emit updateObjects();
    emit updateTrajectories();

    updateObjectTree();

}


void RRMApplication::checkUndoRedo()
{
    bool undo_ = controller->canUndo();
    bool redo_ = controller->canRedo();

    window->ac_undo->setEnabled( undo_ );
    window->ac_redo->setEnabled( redo_ );

    emit updateBoundary();
}


void RRMApplication::checkPreserveStatus()
{
    PolyCurve lboundary_, uboundary_;

     bool above_ = controller->isDefineAboveActive( lboundary_ );
     bool below_ = controller->isDefineBelowActive( uboundary_ );

     window->ac_sketch_above->setChecked( above_ );
     window->ac_sketch_below->setChecked( below_ );

     emit updateBoundary();

}


void RRMApplication::save( const std::string& filename_ )
{
    controller->saveFile( filename_ );
}


void RRMApplication::load( const std::string& filename_ )
{
    emit resetApplication();

    Controller::MeshResolution resol_;
    controller->loadFile( filename_, resol_ );

    emit startApplication();

    double ox_, oy, oz, w_, h_, d_;
    controller->getVolumeGeometry( ox_, oy, oz, w_, h_, d_ );
    emit defineVolumeGeometry( ox_, oy, oz, w_, h_, d_ );
    emit disableVolumeResizing();

    setDiscretization( controller->getCurrentDirection() );

    emit updateVolume();
    emit updateObjects();

    loadObjectTree();
    loadRegions();
    loadDomains();

    checkUndoRedo();
    checkPreserveStatus();



}


void RRMApplication::loadRegions()
{
    window->object_tree->addOutputVolume();
    const std::map< std::size_t, RegionsPtr >& regions_ = controller->getRegions();

    double total_volume_ = controller->getTotalVolume();
    window->object_tree->setTotalVolume( total_volume_ );

    for( auto it: regions_ )
    {
        RegionsPtr & reg_ = (it.second);

        int r_, g_, b_;
        reg_->getColor( r_, g_, b_ );

        double volume_ = reg_->getVolume();
        double perc_ = 0.0;

        if( total_volume_ != 0.0 )
            perc_ = 100*volume_/total_volume_;


        window->object_tree->addRegion( reg_->getIndex(), reg_->getName(), r_, g_, b_, volume_, perc_ );

        emit addRegionCrossSectionBoundary( reg_ );
    }

    if( regions_.empty() == false )
        emit addRegions();
}


void RRMApplication::loadObjectTree()
{
    window->object_tree->addInputVolume();

    const std::map< std::size_t, ObjectPtr >& objects_ = controller->getObjects();

    for( auto it: objects_ )
    {
        const ObjectPtr& obj_ = it.second;
        if( obj_->getIndex() == controller->getCurrentObject()->getIndex() ) continue;

        int r_, g_, b_;
        obj_->getColor( r_, g_, b_ );
        window->object_tree->addObject( obj_->getIndex(), obj_->getType(), obj_->getName(), r_, g_, b_ );
    }
}


void RRMApplication::updateObjectTree()
{

    const std::map< std::size_t, ObjectPtr >& objects_ = controller->getObjects();

    for( auto it: objects_ )
    {
        const ObjectPtr& obj_ = it.second;
        if( obj_->getIndex() == controller->getCurrentObject()->getIndex() ) continue;

        if( obj_->isActive() == false )
            window->object_tree->setObjectVisibility( obj_->getIndex(), false );
        else
            window->object_tree->setObjectVisibility( obj_->getIndex(), true );

    }
}


void RRMApplication::defineRandomColor()
{
    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );

    int r_ = distr( eng );
    int g_ = distr( eng );
    int b_ = distr( eng );

   defineCurrentColor( r_, g_, b_ );

}


void RRMApplication::setMeshResolution( const std::string& resolution_ )
{
    if( resolution_.compare( "HIGH" ) == 0 )
        controller->setMeshResolution( Controller::MeshResolution::HIGH );
    else if( resolution_.compare( "MEDIUM" ) == 0 )
        controller->setMeshResolution( Controller::MeshResolution::MEDIUM );
    else if( resolution_.compare( "LOW" ) == 0 )
        controller->setMeshResolution( Controller::MeshResolution::LOW );

    setDiscretization( controller->getCurrentDirection() );
//    setDiscretization( controller->getMainCrossSection()->getDirection() );

}


void RRMApplication::setRegionSelected( const std::size_t& id_, bool status_ )
{
    controller->setRegionSelected( id_, status_ );
    emit updateRegions();
}


void RRMApplication::setPointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ )
{

    controller->setPointGuidedExtrusion( px_, py_, depth_, dir_ );
}


void RRMApplication::exportToIRAP()
{
    controller->exportToIrapGrid();
}


void RRMApplication::setVerticalExaggeration( double scale_ )
{
    emit setVerticalExaggerationScale( scale_ );
}


void RRMApplication::setGuidedExtrusion( float px_, float py_, float pz_, const PolyCurve& curve_ )
{
    controller->setGuidedExtrusion( px_, py_, pz_, curve_ );
}


void RRMApplication::getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
{
     controller->getLegacyMeshes( points, nu, nv, num_extrusion_steps ) ;
}


void RRMApplication::getSurfacesMeshes( std::vector< FlowWindow::TriangleMesh >& triangles_meshes,
                                        std::vector< FlowWindow::CurveMesh>& left_curves,
                                        std::vector< FlowWindow::CurveMesh >& right_curves,
                                        std::vector< FlowWindow::CurveMesh > & front_curves,
                                        std::vector< FlowWindow::CurveMesh >& back_curves )
{
    std::vector< Controller::TriangleMesh > meshes;
    std::vector< Controller::CurveMesh > lcurves;
    std::vector< Controller::CurveMesh > rcurves;
    std::vector< Controller::CurveMesh > fcurves;
    std::vector< Controller::CurveMesh > bcurves;

    controller->setSurfacesMeshes( meshes, lcurves, rcurves, fcurves, bcurves );

    for( std::size_t i = 0; i < meshes.size(); ++i )
    {
        FlowWindow::TriangleMesh t;
        t.vertex_list = meshes[i].vertex_list;
        t.face_list = meshes[i].face_list;
        triangles_meshes.push_back( t );
    }

    for ( std::size_t i = 0; i < lcurves.size(); ++i )
    {
        FlowWindow::CurveMesh cm_lb, cm_rb, cm_fb, cm_bb;

        std::copy( lcurves[i].vertex_list.begin(), lcurves[i].vertex_list.end(), std::back_inserter(cm_lb.vertex_list) );
        std::copy( lcurves[i].edge_list.begin(), lcurves[i].edge_list.end(), std::back_inserter(cm_lb.edge_list) );

        std::copy( rcurves[i].vertex_list.begin(), rcurves[i].vertex_list.end(), std::back_inserter(cm_rb.vertex_list) );
        std::copy( rcurves[i].edge_list.begin(), rcurves[i].edge_list.end(), std::back_inserter(cm_rb.edge_list) );


        std::copy( fcurves[i].vertex_list.begin(), fcurves[i].vertex_list.end(), std::back_inserter(cm_fb.vertex_list) );
        std::copy( fcurves[i].edge_list.begin(), fcurves[i].edge_list.end(), std::back_inserter(cm_fb.edge_list) );


        std::copy( bcurves[i].vertex_list.begin(), bcurves[i].vertex_list.end(), std::back_inserter(cm_bb.vertex_list) );
        std::copy( bcurves[i].edge_list.begin(), bcurves[i].edge_list.end(), std::back_inserter(cm_bb.edge_list) );


        left_curves.push_back( cm_lb );
        right_curves.push_back( cm_rb );
        front_curves.push_back( cm_fb );
        back_curves.push_back( cm_bb );
    }
}


void RRMApplication::getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces,
                                             std::vector< int >& regions_, std::map< int, std::vector< float > >& colors_ )
{
    regions_ = controller->getTetrahedronsRegions( vertices, edges, faces );

    //    std::map< int, std::vector< float > > colors_;
    for( auto it: regions_ )
    {
        int r = 192, g = 192, b = 192;
        if (it >= 0)
        {
            controller->getDomainColor(static_cast<std::size_t>(it), r, g, b);
        }

        std::vector< float > color_;
        color_.resize( 3 );

        color_[ 0 ]= static_cast< float >( r/255.f );
        color_[ 1 ]= static_cast< float >( g/255.f );
        color_[ 2 ]= static_cast< float >( b/255.f );

        colors_[ it ] = color_;

    }

//    return colors_;

}
