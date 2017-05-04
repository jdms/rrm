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

#include "Controller.hpp"


Controller::Controller()
{

    scene3d = nullptr;
    sketch_scene = nullptr;
    path_scene = nullptr;
    object_tree = nullptr;

    disc_width = 10;
    disc_depth = 10;
    step_depth = 10;

    current_depth_csection = 0.0f;

    current_object_type = Object::TYPE::Stratigraphy;
    current_object = 0;

    RuleStatus::RA_SKETCHING;

}

void Controller::init()
{
    addInputVolume();
    initRulesProcessor();

    setCurrentCrossSection( input_volume.getDepth() );

    createObject();

}


void Controller::addInputVolume()
{
    input_volume.initialize();
    scene3d->addVolume( &input_volume );
    sketch_scene->setVolume( &input_volume );
    path_scene->setVolume( &input_volume );

    ObjectTreeItem* item = new ObjectTreeItem( ObjectTreeItem::TreeItemType::VOLUME, 0 );
    object_tree->addInputVolume( item );
}



void Controller::addCurrentCrossSectionToList()
{

}

void Controller::setCurrentCrossSection( double depth_ )
{
    if( isValidCrossSection( depth_ ) == false )
    {
        std::cout << "Cross-section out of range" <<std::endl;
        return;
    }

    current_depth_csection = depth_;
    updateScenesWithCurrentCrossSection();


}

bool Controller::getCurrentCrossSectionDimensions( double& width_, double& height_ )
{
    if( isCrossSectionAdded( current_depth_csection ) == false )
        return false;

    CrossSection& csection_ = depth_of_cross_sections[ current_depth_csection ];
    csection_.getDimensions( width_, height_ );
    return true;
}



bool Controller::setNameofObjectofId( std::size_t id_, const std::string& name_ )
{
    if( isValidObject( id_ ) == false ) return false;

    Object* obj = objects[ id_ ];
    obj->setName( name_ );
    return true;
}

bool Controller::getNameofObjectofId( std::size_t id_, std::string& name_ )
{
    if( isValidObject( id_ ) == false ) return false;

    Object* obj = objects[ id_ ];
    name_ = obj->getName();
    return true;
}

bool Controller::setVisibilityofObjectofId( std::size_t id_, bool option )
{
    if( isValidObject( id_ ) == false ) return false;

    Object* obj = objects[ id_ ];
    obj->setVisibility( option );
    return true;
}

bool Controller::getVisibilityofObjectofId( std::size_t id_ )
{
    if( isValidObject( id_ ) == false ) return false;

    Object* obj = objects[ id_ ];
    return obj->getVisibility();
}

void Controller::addInputCurvetoCurrentObject( const Curve2D& curve_ )
{
    if( isValidObject( current_object ) == false ) return;

    Object* const& obj_ = objects[ current_object ];
    obj_->addInputCurve( current_depth_csection, curve_ );

    addCurrentObjectToCurrentCrossSection();
    setCurrentCrossSectionAsUsed();
    addCurrentObjectToScenes();

}

bool Controller::getAllInputCurvesofCurrentObject( std::vector< Curve2D >& input_curves_ )
{
    if( isValidObject( current_object ) == false ) return false;

    Object* const& obj_ = objects[ current_object ];
    input_curves_ = obj_->getAllInputCurves();
    return true;

}

std::size_t Controller::getNumberofInputCurvesinObjectofId( std::size_t id_ )
{
    if( isValidObject( id_ ) == false ) return false;

    Object* obj_ = objects[ id_ ];
    return obj_->getNumberofInputCurves();
}

bool Controller::getAllCrossSectionsRelatedtoCurrentObject( std::vector< double >& cross_sections_related_ )
{
    if( isValidObject( current_object ) == false ) return false;

    Object* const& obj_ = objects[ current_object ];
    cross_sections_related_ = obj_->getAllCrossSectionsRelatedtoObject();
    return true;
}

bool Controller::setPathCurvetoCurrentObject( const Curve2D& path_curve_ )
{
    if( isValidObject( current_object ) == false ) return false;

    Object* const& obj_ = objects[ current_object ];
    obj_->setPathCurve( path_curve_ );
    return true;
}

bool Controller::getPathCurveofCurrentObject( Curve2D& path_ )
{
    if( isValidObject( current_object ) == false ) return false;


    Object* const& obj_ = objects[ current_object ];
    path_ =  obj_->getPathCurve();
    return true;
}

bool Controller::interpolate()
{

    if( isValidObject( current_object ) == false ) return false;

    Object* const& obj_ = objects[ current_object ];
    std::vector< std::tuple< Curve2D, double > > curves_ = obj_->getAllCurves();


    bool interpolate_ok = false;

    if( obj_->getType() == Object::TYPE::Stratigraphy )
    {
        interpolate_ok = rules_processor.createSurface( obj_->getId(), curves_ );
    }
    else if( obj_->getType() == Object::TYPE::Channel )
    {
        Curve2D path_ = obj_->getPathCurve();
        Curve2D curve_ = std::get<0>( curves_[ 0 ] );
        interpolate_ok = rules_processor.extrudeAlongPath( obj_->getId(), curve_,
                                                           current_depth_csection, path_ );
    }

    if( interpolate_ok == false ) return false;

    obj_->clearAllCurves();
    updateObjects();    

    return true;

}



void Controller::updateRule( const std::string &rule_ )
{

    if( rule_.compare( "RA_SKETCHING" ) == 0 )
    {
        rules_processor.removeAbove();
    }
    else if( rule_.compare( "RAI_SKETCHING" ) == 0 )
    {
        rules_processor.removeAboveIntersection();
    }
    else if( rule_.compare( "RB_SKETCHING" ) == 0 )
    {
        rules_processor.removeBelow();
    }
    else if( rule_.compare( "RBI_SKETCHING" ) == 0 )
    {
        rules_processor.removeBelowIntersection();
    }

}



bool Controller::enableCreateAbove( bool status_ )
{
    unsetObjectsAsAllowed( allowed_upper );

    if( status_ == true )
    {
        rules_processor.stopDefineAbove();
        unSelectObject( boundering_above );
        return false;
    }


    bool ok_ = rules_processor.requestCreateAbove( allowed_upper );
    if( ok_ == false ) return false;

    setObjectsAsAllowed( allowed_upper );
    current_region = RequestRegion::ABOVE;
    return true;
}



void Controller::defineSketchAbove( std::size_t surface_id_ )
{
    rules_processor.defineAbove( surface_id_ );
    current_region = RequestRegion::NONE;
}


void Controller::setObjectsAsAllowed( std::vector< std::size_t >& objects_ )
{
    sketch_scene->setAllowedObjects( objects_ );
//    scene3d->setAllowedObjects( objects_ );
}

void Controller::unsetObjectsAsAllowed( std::vector< std::size_t >& objects_ )
{
    sketch_scene->unsetAllowedObjects( objects_ );
//    scene3d->unsetAllowedObjects( objects_ );
    objects_.clear();
}


bool Controller::enableCreateBelow( bool status_ )
{

    unsetObjectsAsAllowed( allowed_below );

    if( status_ == false )
    {
        rules_processor.stopDefineBelow();
        unSelectObject( boundering_below );
        return false;
    }

    bool ok_ = rules_processor.requestCreateBelow( allowed_below );
    if( ok_ == false ) return false;

    setObjectsAsAllowed( allowed_below );
    current_region = RequestRegion::BELOW;
    return true;

}

void Controller::defineSketchBelow( std::size_t surface_id_ )
{
    rules_processor.defineBelow( surface_id_ );
    current_region = RequestRegion::NONE;
}


void Controller::sendSelectedSurface( const std::size_t& id_ )
{
    if( current_region == RequestRegion::ABOVE )
    {
        boundering_above = id_;
        rules_processor.defineAbove( id_ );
    }

    else if( current_region == RequestRegion::BELOW )
    {
        boundering_below = id_;
        rules_processor.defineBelow( id_ );
    }
}

void Controller::unSelectObject( const std::size_t& id_ )
{
    sketch_scene->unselectObject( id_ );
//    scene3d->unselectObject( id_ );
}

void Controller::updateObjects()
{

    for( auto &it_: objects )
    {

        Object* obj_ = it_.second;


        std::vector< float > curve_vertices;
        std::vector< std::size_t > curve_edges;

        bool getcurve_ok = rules_processor.getCrossSection( obj_->getId(),
                                                            rowIndexfromDepth( current_depth_csection ),
                                                            curve_vertices, curve_edges );

        if( getcurve_ok  == false )
        {
            std::cout << "Curve not valid \n\n" << std::flush;
            obj_->clearCurve( current_depth_csection );
//            continue;
        }


        //TODO: change addInputCurve to updateInputCurve
        std::vector< double > curve_vertices1( curve_vertices.begin(), curve_vertices.end() );
        obj_->addInputCurve( current_depth_csection, Model3DUtils::convertToCurve2D( curve_vertices1 ) );
        obj_->addInputEdges( current_depth_csection, curve_edges );


//        std::vector< double > csections_ = obj_->getAllCrossSectionsRelatedtoObject();

//        for( auto d_: csections_ )
//        {
//            std::vector< float > curve_vertices;
//            std::vector< std::size_t > curve_edges;

//            bool getcurve_ok = rules_processor.getCrossSection( obj_->getId(),
//                                              rowIndexfromDepth( d_ ), curve_vertices, curve_edges );

//            if( getcurve_ok  == false )
//            {
//                std::cout << "Curve not valid \n\n" << std::flush;
//                obj_->clearCurve( d_ );
//                continue;
//            }


//            //TODO: change addInputCurve to updateInputCurve
//            std::vector< double > curve_vertices1( curve_vertices.begin(), curve_vertices.end() );
//            obj_->addInputCurve( d_, Model3DUtils::convertToCurve2D( curve_vertices1 ) );
//            obj_->addInputEdges( d_, curve_edges );
//        }

        std::vector< float > surface_vertices;
        std::vector< std::size_t > surface_faces;


        bool getmesh_ok = rules_processor.getMesh( obj_->getId(), surface_vertices, surface_faces );
        if( getmesh_ok  == false )
        {
            std::cout << "Mesh not valid \n\n" << std::flush;
            obj_->clearSurface();
        }

        std::vector< double > surface_vertices1( surface_vertices.begin(), surface_vertices.end() );
        obj_->updateSurface( surface_vertices1, surface_faces );

    }

}

void Controller::clear()
{

    if( scene3d != nullptr )
        scene3d->clearScene();

    if( sketch_scene != nullptr )
        sketch_scene->clearScene();


    if( path_scene != nullptr )
        path_scene->clearScene();


    object_tree->clear();

    input_volume.clear();
    disc_width = 10;
    disc_depth = 10;
    step_depth = 10;

    current_depth_csection = 0;
    used_cross_sections.clear();

    current_object_type = Object::TYPE::Stratigraphy;
    current_object = 0;

    for( auto &it: depth_of_cross_sections )
    {
        (it.second).clear();
    }
    depth_of_cross_sections.clear();


    for( auto &it: objects )
    {
        (it.second)->clear();
        delete it.second;
    }
    objects.clear();

    for( auto &it: regions )
    {
        (it.second)->clear();
        delete it.second;
    }
    regions.clear();

    current_rule = RuleStatus::RA_SKETCHING;
    rules_processor.clear();
}

