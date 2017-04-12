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

#include "./src/Core/Geology/Models/Boundary.hpp"

#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"



Controller::Controller()
{
/*
    current_reconstruction = ReconstructionMode::EXTRUSION;
    current_crosssection = 0.0f;
    current_stratigraphy = 0;
*/
}

void Controller::init()
{
    addInputVolume();
    setCurrentCrossSection( input_volume.getDepth() );

    createObject();
}


void Controller::addInputVolume()
{
    //TODO: create visualization volume
    //TODO: add in object tree, etc

    input_volume.initialize();
    scene3d->addVolume( &input_volume );
    sketch_scene->setVolume( &input_volume );

    //object_tree.add( input_volume, "INPUT VOLUME" );
}



void Controller::addCurrentCrossSectionToList()
{
//    if( isCrossSectionAdded( current_depth_csection ) == true ) return;
//    addCrossSectionofDepth( current_depth_csection );
}

void Controller::setCurrentCrossSection( double depth_ )
{
    if( isValidCrossSection( depth_ ) == false )
    {
        std::cout << "Cross-section out of range" <<std::endl;
        return;
    }

    current_depth_csection = depth_;

    updateScenesWithCurrentCrossSection();    //    viewCrossSection();

//    sketch_scene->init();

//    std::cout << "-- Current depth of cross-section is " << current_depth_csection << "\n\n"
//              << std::flush;

}

bool Controller::getCurrentCrossSectionDimensions( double& width_, double& height_ )
{
    if( isCrossSectionAdded( current_depth_csection ) == false )
        return false;

    CrossSection1& csection_ = depth_of_cross_sections[ current_depth_csection ];
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



    std::cout << "Curve added to Object " << obj_->getId() << "\n\n"  << std::flush;
    std::cout << " -- Object: " << obj_->getId() << " has now " << obj_->getNumberofInputCurves()
              << " input curves\n"  << std::flush;

    std::vector< double > csections = obj_->getAllCrossSectionsRelatedtoObject();
    std::cout << " -- Used Cross-Sections by the object: " << csections.size() << "\n\n"
              <<  std::flush;


    std::cout << "The program has in total " << depth_of_cross_sections.size() << " cross-sections: \n"
              << std::flush;

    for( auto const &it : depth_of_cross_sections )
    {
        std::set< Object* > objs_ = (it.second).getObjectsReferenced();
        std::cout << "\t*depth " << it.first << " has " << objs_.size() << " objects\n" << std::flush;
    }

    std::cout << "\n" << std::flush;

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
    std::vector< Curve2D > curves_ = obj_->getAllCurves();

    std::cout << "Object " << current_object << " interpolated\n" << std::flush;

//    bool interpolate_ok = rules_processor.addSurface( curves_, obj_->getId() );
//    if( interpolate_ok == false ) return false;

    createObject();
    return true;
}



void Controller::updateRule( const std::string &rule_ )
{

    if( rule_.compare( "RA_SKETCHING" ) == 0 )
    {
//        rules_processor.update( State::RA_SKETCHING );
        std::cout << "Remove above\n\n" <<std::flush;
    }
    else if( rule_.compare( "RAI_SKETCHING" ) == 0 )
    {
//        rules_processor.update( State::RAI_SKETCHING );
        std::cout << "Remove above intersection\n\n" <<std::flush;
    }
    else if( rule_.compare( "RB_SKETCHING" ) == 0 )
    {
//        rules_processor.update( State::RB_SKETCHING );
        std::cout << "Remove below\n\n" <<std::flush;
    }
    else if( rule_.compare( "RBI_SKETCHING" ) == 0 )
    {
//        rules_processor.update( State::RBI_SKETCHING );
        std::cout << "Remove below intersection\n\n" <<std::flush;
    }
    else if( rule_.compare( "SKETCHING" ) == 0 )
    {
//        rules_processor.update( State::SKETCHING );
        std::cout << "Sketching \n\n" <<std::flush;

    }

}

void Controller::defineSketchAbove( bool status_ )
{
    if( status_ == true )
    {
//        rules_processor.defineAbove();
        std::cout << "Defining above \n\n" <<std::flush;
    }
    else
    {
//        rules_processor.stopDefineAbove();
        std::cout << "Stop define above \n\n" <<std::flush;
    }
}

void Controller::defineSketchBelow( bool status_ )
{
    if( status_ == true )
    {
//        rules_processor.defineBelow();
        std::cout << "Defining below \n\n" <<std::flush;
    }
    else
    {
//        rules_processor.stopDefineBelow();
        std::cout << "Stop define below \n\n" <<std::flush;
    }
}


void Controller::updateObjects()
{

    std::cout << "Updating objects in controller\n\n" << std::flush;
    for( auto &it_: objects )
    {

//        std::vector< double > curve_vertices;
//        std::vector< double > curve_edges;

//        std::vector< double > surface_vertices;
//        std::vector< std::size_t > surface_faces;


        Object* obj_ = it_.second;
        std::vector< double > csections_ = obj_->getAllCrossSectionsRelatedtoObject();

        for( auto d_: csections_ )
        {
//            bool getcurve_ok = rules_processor.getCurve( obj_->getId(), d_, curve_vertices,
//                                                         curve_edges );
//            obj_->addInputCurve( d_, curve_vertices );
//            obj_->addInputEdges( d_, curve_edges );
        }


//        bool getmesh_ok = rules_processor.getMesh( obj_->getId(), surface_vertices, surface_faces );
//        if( getmesh_ok  == false )
//        {
//            obj_->clear();
//            continue;
//        }

//        obj_->updateCurveWireframe( curve_edges );
//        obj_->updateSurface( surface_vertices, surface_faces );

    }

}


//void Controller::addCrossSection( const float& d )
//{

//    CrossSection* cross_section = new CrossSection();
//    cross_section->setDepth( d );

//    crosssections_list[ d ] = cross_section;
//    current_crosssection = d;

//}



//bool Controller::addBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth )
//{

//    if( crosssections_list.empty() == true ) return false;


//    CrossSection* cross_section = crosssections_list[ current_crosssection ];
//    Boundary* boundary_data;


//    if( cross_section->hasBoundary() == false )
//    {
//        boundary_data = new Boundary();
//    }
//    else
//    {
//        boundary_data = cross_section->getBoundary();
//    }


//    boundary_data->edit( origin_x, origin_y, origin_z, width, height, depth );
//    cross_section->setBoundary( boundary_data );


//    return true;

//}


//void Controller::editBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth )
//{

//    CrossSection* cross_section = crosssections_list[ current_crosssection ];
//    Boundary* boundary_data = cross_section->getBoundary();


//    boundary_data->edit( origin_x, origin_y, origin_z, width, height, depth );
//    cross_section->setBoundary( boundary_data );

//    update();

//}


//Boundary* Controller::getCurrentBoundary()
//{

//    CrossSection* cross_section = crosssections_list[ current_crosssection ];
//    return cross_section->getBoundary();

//}




//void Controller::initRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth )
//{


//    rules_processor.setOrigin( orig_x, orig_y, orig_z );
//    rules_processor.setLenght( width, height, depth );
//    rules_processor.init();
//    rules_processor.update( RRM::ExtrusionRulesProcessor::State::SKETCHING );


//}


//void Controller::editRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth )
//{

//    rules_processor.setOrigin( orig_x, orig_y, orig_z );
//    rules_processor.setLenght( width, height, depth );
//    rules_processor.init();

//}




//bool Controller::addCurve( const Curve2D& curve )
//{

//    if( stratigraphics_list.empty() == true )
//        addStratigraphy();

//    Stratigraphy* strat = stratigraphics_list[ current_stratigraphy ];
//    strat->addCurve( current_crosssection, curve );


//    CrossSection* cross_section = crosssections_list[ current_crosssection ];
//    cross_section->addStratigraphy( strat );


//    return true;


//}


//bool Controller::addStratigraphy()
//{

//    Stratigraphy* strat = new Stratigraphy();

//    current_stratigraphy = strat->getId();
//    stratigraphics_list[ current_stratigraphy ] = strat;

//    return true;


//}


//Stratigraphy* Controller::getCurrentStratigraphy()
//{

//    if( stratigraphics_list.empty() == true ) return 0;

//    return stratigraphics_list[ current_stratigraphy ];

//}


//bool Controller::interpolateStratigraphy( const std::vector< size_t >& lower_bound, const std::vector< size_t >& upper_bound )
//{

//    if( stratigraphics_list.empty() == true ) return false;


//    Stratigraphy* strat = stratigraphics_list[ current_stratigraphy ];
//    Curve2D* c = strat->getCurve( current_crosssection );


//    bool status_ok;

//    if( ( lower_bound.empty() == true ) && ( upper_bound.empty() == true ) )
//        status_ok = rules_processor.update( *c, strat->getId() );

//    else
//        status_ok = rules_processor.update( *c, strat->getId(), lower_bound, upper_bound );



//    if( status_ok == false ) return false;


//    bool can_undo = rules_processor.canUndo();
//    emit enableUndo( can_undo );

//    bool can_redo = rules_processor.canRedo();
//    emit enableRedo( can_redo );


//    update();
//    return true;

//}


//size_t Controller::getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
//{
//    return rules_processor.getLegacyMeshes( points, nu, nv, num_extrusion_steps );
//}




//bool Controller::defineSketchingAbove( std::vector< size_t >& allowed )
//{
//    return rules_processor.requestDefineAbove( allowed );

//}


//bool Controller::defineSketchingBelow( std::vector< size_t >& allowed )
//{
//    return rules_processor.requestDefineBelow( allowed );
//}




//bool Controller::defineRegionAbove( const std::vector< size_t >& selections )
//{
//    if( selections.empty() == true ) return false;
//    return rules_processor.defineAbove( selections[ 0 ] );
//}


//bool Controller::defineRegionBelow( const std::vector< size_t >& selections )
//{
//    if( selections.empty() == true ) return false;
//    return rules_processor.defineBelow( selections[ 0 ] );
//}


//bool Controller::stopSketchingAbove()
//{
//    rules_processor.stopDefineAbove();
//    return true;
//}


//bool Controller::stopSketchingBelow()
//{
//    rules_processor.stopDefineBelow();
//    return true;
//}




//void Controller::setStratigraphicRule( const std::string& rule )
//{

//    if( rule.compare( "SKETCHING" ) == 0 )
//        rules_processor.update( RRM::ExtrusionRulesProcessor::State::SKETCHING );

//    else if( rule.compare( "RA_SKETCHING" ) == 0 )
//        rules_processor.update( RRM::ExtrusionRulesProcessor::State::RA_SKETCHING );

//    else if( rule.compare( "RAI_SKETCHING" ) == 0 )
//        rules_processor.update( RRM::ExtrusionRulesProcessor::State::RAI_SKETCHING );

//    else if( rule.compare( "RB_SKETCHING" ) == 0 )
//        rules_processor.update( RRM::ExtrusionRulesProcessor::State::RB_SKETCHING );

//    else if( rule.compare( "RBI_SKETCHING" ) == 0 )
//        rules_processor.update( RRM::ExtrusionRulesProcessor::State::RBI_SKETCHING );

//}


//void Controller::changeResolution( const int numI_ , const int numJ_ )
//{

//    size_t numI = numI_;
//    size_t numJ = numJ_;

//    rules_processor.requestChangeDiscretizationLevel( numI, numJ );
//    update();

//}


//void Controller::undo()
//{

//    bool undo_ok = rules_processor.undo();
//    update();


//    bool can_undo = rules_processor.canUndo();
//    emit enableUndo( can_undo );

//    bool can_redo = rules_processor.canRedo();
//    emit enableRedo( can_redo );

//}


//void Controller::redo()
//{
//    bool redo_ok = rules_processor.redo();
//    update();


//    bool can_undo = rules_processor.canUndo();
//    emit enableUndo( can_undo );

//    bool can_redo = rules_processor.canRedo();
//    emit enableRedo( can_redo );

//}




//void Controller::clear()
//{
//    current_reconstruction = ReconstructionMode::EXTRUSION;

//    current_crosssection = 0.0f;
//    current_stratigraphy = 0;

//    stratigraphics_list.clear();
//    crosssections_list.clear();

//    rules_processor.clear();

//}



//void Controller::update()
//{

//    std::map< size_t, Stratigraphy* >::iterator it;

//    for( it = stratigraphics_list.begin(); it != stratigraphics_list.end(); ++it )
//    {

//        std::vector< float > curve_vertices;
//        std::vector< size_t > curve_edges;

//        std::vector< float > surface_vertices;
//        std::vector< size_t > surface_faces;


//        Stratigraphy* strat = it->second;
//        bool getcurve_ok = rules_processor.getCurve( strat->getId(), curve_vertices, curve_edges );
//        bool getmesh_ok = rules_processor.getMesh ( strat->getId(), surface_vertices, surface_faces );



//        if( ( getcurve_ok == false ) || ( getmesh_ok  == false ) )
//        {
//            strat->clear();
//            continue;
//        }


//        strat->updateCurve( current_crosssection, Model3DUtils::convertToCurve2D( curve_vertices ) );
//        strat->updateCurveWireframe( curve_edges );
//        strat->updateSurface( surface_vertices, surface_faces );

//    }

//    emit updateScene();



//}
