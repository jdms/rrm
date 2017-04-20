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

    current_reconstruction = ReconstructionMode::EXTRUSION;

    current_crosssection = 0.0f;
    current_stratigraphy = 0;

}




void Controller::addCrossSection( const float& d )
{

    CrossSection* cross_section = new CrossSection();
    cross_section->setDepth( d );

    crosssections_list[ d ] = cross_section;
    current_crosssection = d;

}



bool Controller::addBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth )
{

    if( crosssections_list.empty() == true ) return false;


    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    Boundary* boundary_data;


    if( cross_section->hasBoundary() == false )
    {
        boundary_data = new Boundary();
    }
    else
    {
        boundary_data = cross_section->getBoundary();
    }


    boundary_data->edit( origin_x, origin_y, origin_z, width, height, depth );
    cross_section->setBoundary( boundary_data );


    return true;

}


void Controller::editBoundary( const float& origin_x, const float& origin_y, const float& origin_z, const float& width, const float& height, const float& depth )
{

    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    Boundary* boundary_data = cross_section->getBoundary();


    boundary_data->edit( origin_x, origin_y, origin_z, width, height, depth );
    cross_section->setBoundary( boundary_data );

    update();

}


Boundary* Controller::getCurrentBoundary()
{

    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    return cross_section->getBoundary();

}




void Controller::initRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth )
{

//    rules_processor.setOrigin( orig_x, orig_y, orig_z );
//    rules_processor.setLenght( width, height, depth );

}


void Controller::editRulesProcessor( const float& orig_x, const float& orig_y, const float& orig_z, const float& width, const float& height, const float& depth )
{

//    rules_processor.setOrigin( orig_x, orig_y, orig_z );
//    rules_processor.setLenght( width, height, depth );

}




bool Controller::addCurve( const Curve2D& curve )
{

    if( stratigraphics_list.empty() == true )
        addStratigraphy();

    Stratigraphy* strat = stratigraphics_list[ current_stratigraphy ];
    strat->addCurve( current_crosssection, curve );


    CrossSection* cross_section = crosssections_list[ current_crosssection ];
    cross_section->addStratigraphy( strat );


    return true;


}


bool Controller::addStratigraphy()
{

    Stratigraphy* strat = new Stratigraphy();

    current_stratigraphy = strat->getId();
    stratigraphics_list[ current_stratigraphy ] = strat;

    return true;


}


Stratigraphy* Controller::getCurrentStratigraphy()
{

    if( stratigraphics_list.empty() == true ) return 0;

    return stratigraphics_list[ current_stratigraphy ];

}


bool Controller::interpolateStratigraphy( const std::vector< size_t >& lower_bound, const std::vector< size_t >& upper_bound )
{

    if( stratigraphics_list.empty() == true ) return false;


    Stratigraphy* strat = stratigraphics_list[ current_stratigraphy ];
    Curve2D* c = strat->getCurve( current_crosssection );


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


    update();
    return true;

}


size_t Controller::getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
{
    return 0;//rules_processor.getLegacyMeshes( points, nu, nv, num_extrusion_steps );
}




bool Controller::defineSketchingAbove( std::vector< size_t >& allowed )
{
    return false;//rules_processor.requestDefineAbove( allowed );

}


bool Controller::defineSketchingBelow( std::vector< size_t >& allowed )
{
    return false;//rules_processor.requestDefineBelow( allowed );
}




bool Controller::defineRegionAbove( const std::vector< size_t >& selections )
{
    if( selections.empty() == true ) return false;
    return true;//rules_processor.defineAbove( selections[ 0 ] );
}


bool Controller::defineRegionBelow( const std::vector< size_t >& selections )
{
    if( selections.empty() == true ) return false;
    return true;//rules_processor.defineBelow( selections[ 0 ] );
}


bool Controller::stopSketchingAbove()
{
//    rules_processor.stopDefineAbove();
    return true;
}


bool Controller::stopSketchingBelow()
{
//    rules_processor.stopDefineBelow();
    return true;
}




void Controller::setStratigraphicRule( const std::string& rule )
{

//    if( rule.compare( "RA_SKETCHING" ) == 0 )
//        rules_processor.removeAbove();

//    else if( rule.compare( "RAI_SKETCHING" ) == 0 )
//        rules_processor.removeAboveIntersection();

//    else if( rule.compare( "RB_SKETCHING" ) == 0 )
//        rules_processor.removeBelow();

//    else if( rule.compare( "RBI_SKETCHING" ) == 0 )
//        rules_processor.removeBelowIntersection();

}


void Controller::changeResolution( const int numI_ , const int numJ_ )
{

    size_t numI = numI_;
    size_t numJ = numJ_;

//    rules_processor.requestChangeDiscretizationLevel( numI, numJ );
    update();

}


void Controller::undo()
{

//    bool undo_ok = rules_processor.undo();
//    update();


//    bool can_undo = rules_processor.canUndo();
//    emit enableUndo( can_undo );

//    bool can_redo = rules_processor.canRedo();
//    emit enableRedo( can_redo );

}


void Controller::redo()
{
//    bool redo_ok = rules_processor.redo();
    update();


//    bool can_undo = rules_processor.canUndo();
//    emit enableUndo( can_undo );

//    bool can_redo = rules_processor.canRedo();
//    emit enableRedo( can_redo );

}




void Controller::clear()
{
    current_reconstruction = ReconstructionMode::EXTRUSION;

    current_crosssection = 0.0f;
    current_stratigraphy = 0;

    stratigraphics_list.clear();
    crosssections_list.clear();

//    rules_processor.clear();

}



void Controller::update()
{

    std::map< size_t, Stratigraphy* >::iterator it;

    for( it = stratigraphics_list.begin(); it != stratigraphics_list.end(); ++it )
    {

        std::vector< float > curve_vertices;
        std::vector< size_t > curve_edges;

        std::vector< float > surface_vertices;
        std::vector< size_t > surface_faces;


        Stratigraphy* strat = it->second;
//        bool getcurve_ok = rules_processor.getCurve( strat->getId(), curve_vertices, curve_edges );
//        bool getmesh_ok = rules_processor.getMesh ( strat->getId(), surface_vertices, surface_faces );



//        if( ( getcurve_ok == false ) || ( getmesh_ok  == false ) )
//        {
//            strat->clear();
//            continue;
//        }


        strat->updateCurve( current_crosssection, Model3DUtils::convertToCurve2D( curve_vertices ) );
        strat->updateCurveWireframe( curve_edges );
        strat->updateSurface( surface_vertices, surface_faces );

    }

    emit updateScene();



}
