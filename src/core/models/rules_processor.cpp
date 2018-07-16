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

#include "rules_processor.hpp"

#include "colormap/color_map_wrapper.hpp"
//namespace RRM
//{}

RulesProcessor::RulesProcessor()
{
    modeller_.useOpenGLCoordinateSystem();
    setMediumResolution();
}

std::vector<std::size_t> RulesProcessor::getSurfaces()
{
    return modeller_.getSurfacesIndices();
}


std::size_t RulesProcessor::getWidthResolution()
{
    return modeller_.getWidthDiscretization();
}


std::size_t RulesProcessor::getDepthResolution()
{
    return modeller_.getLengthDiscretization();
}


//
// brief:
// Verifies whether ir is possible to define a new 'drawing' region,
// either above or below.
// Argument: `eligible_surfaces` stores the indices of surfaces that can
// be used as input either for an `defineAbove()` or a `defineBelow()`.
// Return: true if at least one elegible surface was found.
//
/* bool requestDefineRegion( std::vector<size_t> &eligible_surfaces ); */

bool RulesProcessor::requestCreateAbove( std::vector<size_t> &eligible_surfaces )
{
    return modeller_.requestCreateAbove(eligible_surfaces);
}

bool RulesProcessor::requestCreateBelow( std::vector<size_t> &eligible_surfaces )
{
    return modeller_.requestCreateBelow(eligible_surfaces);
}




/* Change the model's properties */



bool RulesProcessor::setLowResolution()
{
    bool status = modeller_.tryChangeDiscretization(16, 16);
    if ( status == false )
    {
        return false;
    }

    current_resolution_ = LOW;

    return status;
}

bool RulesProcessor::setMediumResolution()
{
    bool status = modeller_.tryChangeDiscretization(32, 32);
    if ( status == false )
    {
        return false;
    }

    current_resolution_ = MEDIUM;

    return status;
}

bool RulesProcessor::setHighResolution()
{
    bool status = modeller_.tryChangeDiscretization(64, 64);
    if ( status == false )
    {
        return false;
    }

    current_resolution_ = HIGH;

    return status;
}

bool RulesProcessor::isLowResolution()
{
    size_t num_width = getWidthResolution();
    size_t num_depth = getDepthResolution();

    if ( (num_width == 16) && (num_depth == 16) )
    {
        return true;
    }

    return false;
}

bool RulesProcessor::isMediumResolution()
{
    size_t num_width = getWidthResolution();
    size_t num_depth = getDepthResolution();

    if ( (num_width == 32) && (num_depth == 32) )
    {
        return true;
    }

    return false;
}

bool RulesProcessor::isHighResolution()
{
    size_t num_width = getWidthResolution();
    size_t num_depth = getDepthResolution();

    if ( (num_width == 64) && (num_depth == 64) )
    {
        return true;
    }

    return false;
}

bool isMediumResolution();

bool isHighResolution();


void RulesProcessor::setOrigin( double opengl_x, double opengl_y, double opengl_z )
{
    origin_.x = opengl_x;
    origin_.y = opengl_y;
    origin_.z = opengl_z;

    modeller_.setOrigin(opengl_x, opengl_y, opengl_z);
}

bool RulesProcessor::setLenght( double opengl_x, double opengl_y, double opengl_z )
{

    if ( modeller_.setSize(opengl_x, opengl_y, opengl_z) == false )
    {
        return false;
    }

    lenght_.x = opengl_x;
    lenght_.y = opengl_y;
    lenght_.z = opengl_z;

//    std::cout << "updated " << std::endl << std::flush;

    return true;
}


void RulesProcessor::getOrigin( double &opengl_x, double &opengl_y, double &opengl_z )
{
    modeller_.getOrigin(opengl_x, opengl_y, opengl_z);
}

void RulesProcessor::getLenght( double &opengl_x, double &opengl_y, double &opengl_z )
{
    modeller_.getSize(opengl_x, opengl_y, opengl_z);
}


/* Begin methods to interface with GUI */


/* Clean up */
void RulesProcessor::clear()
{
    modeller_.clear();
}


/* Query or modify the automatum state */


//
// brief:
// Define new input region above surface which index is `surface_index`.
//
bool RulesProcessor::defineAbove( size_t surface_index )
{
    return modeller_.createAbove(surface_index);
}


//
// brief:
// Clear any previous `defineAbove()` call.
// Safe to call anytime.
//
void RulesProcessor::stopDefineAbove()
{
    return modeller_.stopCreateAbove();
}

//
// brief:
// Define new input region below surface which index is `surface_index`.
//
bool RulesProcessor::defineBelow( size_t surface_index )
{
    return modeller_.createBelow(surface_index);
}

//
// brief:
// Clear any previous `defineBelow()` call.
// Safe to call anytime.
//
void RulesProcessor::stopDefineBelow()
{
    modeller_.stopCreateBelow();
}

//<<<<<<< HEAD
//bool RulesProcessor::defineAboveIsActive( std::size_t& index )
//{
//    return modeller_.createAboveIsActive();
//}

//bool RulesProcessor::defineBelowIsActive( std::size_t& index )
//{
//    return modeller_.createBelowIsActive();
//=======
bool RulesProcessor::defineAboveIsActive()
{
    size_t dummy_index;
    return modeller_.createAboveIsActive(dummy_index);
}

bool RulesProcessor::defineBelowIsActive()
{
    size_t dummy_index;
    return modeller_.createBelowIsActive(dummy_index);
}

bool RulesProcessor::defineAboveIsActive( size_t &boundary_index )
{
    return modeller_.createAboveIsActive(boundary_index);
}

bool RulesProcessor::defineBelowIsActive( size_t &boundary_index )
{
    return modeller_.createBelowIsActive(boundary_index);
//>>>>>>> a8073a9921ca7e86e80e6ce7d8d7085f52de6471
}


void RulesProcessor::removeAbove()
{
    truncate_surface_ = false;
    modeller_.removeAbove();
}

void RulesProcessor::removeAboveIntersection()
{
    truncate_surface_ = false;
    modeller_.removeAboveIntersection();
}

void RulesProcessor::removeBelow()
{
    truncate_surface_ = false;
    modeller_.removeBelow();
}

void RulesProcessor::removeBelowIntersection()
{
    truncate_surface_ = false;
    modeller_.removeBelowIntersection();
}


void RulesProcessor::truncate()
{
    truncate_surface_ = true;
    std::cout << "Truncating!\n";
}


bool RulesProcessor::canUndo()
{
	if ( testing_surface_insertion_ )
	{
		return false;
	}

    return modeller_.canUndo();
}

bool RulesProcessor::undo()
{
	if ( !canUndo() )
	{
		return false;
	}

    return modeller_.undo();
}

bool RulesProcessor::canRedo()
{
	if ( testing_surface_insertion_ )
	{
		return false;
	}

    return modeller_.canRedo();
}

bool RulesProcessor::redo()
{
	if ( !canRedo() )
	{
		return false;
	}
    return modeller_.redo();
}

bool RulesProcessor::getMesh( size_t surface_id, std::vector<float> &vlist, std::vector<size_t> &flist )
{
    return modeller_.getMesh(surface_id, vlist, flist);
}

bool RulesProcessor::getMesh( size_t surface_id, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    return modeller_.getMesh(surface_id, vlist, flist);
}

bool RulesProcessor::getNormals( size_t surface_id, std::vector<double> &nlist )
{
    SUtilitiesWrapper util(modeller_);

    return util.getNormalList(surface_id, nlist);
}

bool RulesProcessor::getCrossSection( size_t surface_id, size_t length, std::vector<float> &vlist, std::vector<size_t> &elist )
{
    return modeller_.getLengthCrossSectionCurve(surface_id, length, vlist, elist);
}

bool RulesProcessor::getCrossSection( size_t surface_id, size_t length, std::vector<double> &vlist, std::vector<size_t> &elist )
{
//    std::cout << "Getting cross section: " << surface_id << ":" << depth << "\n" << std::flush;

    return modeller_.getLengthCrossSectionCurve(surface_id, length, vlist, elist);
}

bool RulesProcessor::getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<float> &vlist, std::vector<size_t> &elist ) 
{
    return modeller_.getWidthCrossSectionCurve(surface_id, width, vlist, elist);
}

bool RulesProcessor::getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<double> &vlist, std::vector<size_t> &elist ) 
{
    return modeller_.getWidthCrossSectionCurve(surface_id, width, vlist, elist);
}

bool RulesProcessor::getLengthCrossSectionCurve( size_t surface_id, size_t length, std::vector<float> &vlist, std::vector<size_t> &elist )
{
    return modeller_.getLengthCrossSectionCurve(surface_id, length, vlist, elist);
}

bool RulesProcessor::getLengthCrossSectionCurve( size_t surface_id, size_t length, std::vector<double> &vlist, std::vector<size_t> &elist ) 
{
    return modeller_.getLengthCrossSectionCurve(surface_id, length, vlist, elist);
}

bool RulesProcessor::getExtrusionPath( size_t surface_index, std::vector<double> &path)
{
    SUtilitiesWrapper util(modeller_);

    bool status = util.getExtrusionPath(surface_index, path);

    return status;
}


bool RulesProcessor::saveFile( std::string filename )
{
    return modeller_.saveJSON(filename);
}

bool RulesProcessor::loadFile( std::string filename )
{
    bool status = modeller_.loadBinary(filename);

    if ( status == false )
    {
        status = modeller_.loadJSON(filename);
    }

    return status;
}

size_t RulesProcessor::getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
{
    SUtilitiesWrapper u(modeller_);

    return u.getLegacyMeshes(points, nu, nv, num_extrusion_steps);
}

bool RulesProcessor::getLeftBoundaryCrossSectionCurve( std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges  )
{
    SUtilitiesWrapper u(modeller_);

    return u.getLeftBoundary3DCurves(vertices, edges);
}

bool RulesProcessor::getRightBoundaryCrossSectionCurve(  std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges  )
{
    SUtilitiesWrapper u(modeller_);

    return u.getRightBoundary3DCurves(vertices, edges);
}

bool RulesProcessor::getFrontBoundaryCrossSectionCurve(  std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges  )
{
    SUtilitiesWrapper u(modeller_);

    return u.getFrontBoundary3DCurves(vertices, edges);
}

bool RulesProcessor::getBackBoundaryCrossSectionCurve(  std::vector< std::vector<double> >& vertices,  std::vector< std::vector<std::size_t> >& edges  )
{
    SUtilitiesWrapper u(modeller_);

    return u.getBackBoundary3DCurves(vertices, edges);
}

bool RulesProcessor::getTetrahedralMesh( std::vector<double> &vertex_coordinates, std::vector< std::vector<std::size_t> > &element_list )
{
    return (modeller_.getTetrahedralMesh(vertex_coordinates, element_list) > 0);
}

#include <iterator>

bool RulesProcessor::setPLCForSimulation( std::vector< TriangleMesh >& triangle_meshes,
        std::vector< CurveMesh >& left_curves,
        std::vector< CurveMesh >& right_curves,
        std::vector< CurveMesh > & front_curves,
        std::vector< CurveMesh >& back_curves, 
        size_t length_discretization, 
        size_t width_discretization 
        )
{
    //
    // Reduce resolution for simulation
    //

    /* modeller_.changeDiscretization(length_discretization, width_discretization); */

    // Change the following line to force a change in the models' discretization, also, see line above
    /* modeller_.changeDiscretization(16, 16); */
    
    // 
    // Get the PLC
    //

    std::vector< std::size_t > surfacesIDs = getSurfaces();

    for( auto it: surfacesIDs )
    {

        /* if( objects.findElement( it ) == false ) continue; */

        TriangleMesh t;
        /* std::vector< double > surface_vertices; */
        /* std::vector< std::size_t > surface_faces; */


        /* bool has_surface = getMesh( it, surface_vertices, surface_faces ); */
        bool has_surface = getMesh( it, t.vertex_list, t.face_list );

        if( has_surface  == false ) continue;


        /* t.face_list = surface_faces; */
        /* t.vertex_list = surface_vertices; */

        //
        // This loop changes the y-z coordinates of the vertices as RRM
        // understands the y coordinate as height and the z coordinate as
        // length, but Zhao's convention is the opposite.
        //
        double y, z;
        for ( size_t i = 0; i < t.vertex_list.size()/3; ++i )
        {
            y = t.vertex_list[3*i + 1];
            z = t.vertex_list[3*i + 2];

            t.vertex_list[3*i + 1] = z;
            t.vertex_list[3*i + 2] = y;
        }

        triangle_meshes.push_back( t );
    }

    std::vector< std::vector<double> > lb_vertex_lists, rb_vertex_lists, fb_vertex_lists, bb_vertex_lists;
    std::vector< std::vector<std::size_t> >lb_edge_lists, rb_edge_lists, fb_edge_lists, bb_edge_lists;

    getLeftBoundaryCrossSectionCurve(lb_vertex_lists, lb_edge_lists);
    getRightBoundaryCrossSectionCurve(rb_vertex_lists, rb_edge_lists);
    getFrontBoundaryCrossSectionCurve(fb_vertex_lists, fb_edge_lists);
    getBackBoundaryCrossSectionCurve(bb_vertex_lists, bb_edge_lists);

    for ( size_t i = 0; i < lb_vertex_lists.size(); ++i )
    {
        CurveMesh cm_lb, cm_rb, cm_fb, cm_bb;

        std::copy( lb_vertex_lists[i].begin(), lb_vertex_lists[i].end(), std::back_inserter(cm_lb.vertex_list) );
        std::copy( lb_edge_lists[i].begin(), lb_edge_lists[i].end(), std::back_inserter(cm_lb.edge_list) );

        std::copy( rb_vertex_lists[i].begin(), rb_vertex_lists[i].end(), std::back_inserter(cm_rb.vertex_list) );
        std::copy( rb_edge_lists[i].begin(), rb_edge_lists[i].end(), std::back_inserter(cm_rb.edge_list) );

        std::copy( fb_vertex_lists[i].begin(), fb_vertex_lists[i].end(), std::back_inserter(cm_fb.vertex_list) );
        std::copy( fb_edge_lists[i].begin(), fb_edge_lists[i].end(), std::back_inserter(cm_fb.edge_list) );

        std::copy( bb_vertex_lists[i].begin(), bb_vertex_lists[i].end(), std::back_inserter(cm_bb.vertex_list) );
        std::copy( bb_edge_lists[i].begin(), bb_edge_lists[i].end(), std::back_inserter(cm_bb.edge_list) );

        left_curves.push_back( cm_lb );
        right_curves.push_back( cm_rb );
        front_curves.push_back( cm_fb );
        back_curves.push_back( cm_bb );
    }


    // 
    // Return resolution to original state
    //

    /* switch( current_resolution_ ) */
    /* { */
	/* case LOW: */
    /*         modeller_.changeDiscretization(32, 32); */
    /*         break; */

	/* case MEDIUM: */
    /*         modeller_.changeDiscretization(64, 64); */
    /*         break; */

	/* case HIGH: */
    /*         modeller_.changeDiscretization(128, 128); */
    /*         break; */
    /* } */

    // Use the following to force a different discretization on the modeller
	/* modeller_.changeDiscretization(64, 64); */

    return true;
}

bool RulesProcessor::getRegionsForSimulationTetrahedralMesh( const std::vector<double> &vertex_coordinates, const std::vector<std::size_t> &element_list, std::vector<int> &regions )
{
    SUtilitiesWrapper u(modeller_);
    bool status = u.getTetrahedralMeshRegions( vertex_coordinates, element_list, regions);

    return status;
}

std::vector<int> RulesProcessor::getRegionsColor( std::size_t numColors )
{
    ColorMapWrapper cm;

    return cm.getSpectral(numColors);
}

bool RulesProcessor::getQuadMesh( std::size_t surface_id, std::vector<double> &points, std::vector<bool> &valid_points, std::size_t &num_width, std::size_t &num_length )
{
    SUtilitiesWrapper u(modeller_);
    bool status = u.getQuadMesh(surface_id, points, valid_points, num_width, num_length);

    return status;
}

void RulesProcessor::testSurfaceInsertion()
{
    testing_surface_insertion_ = true;

    if ( last_surface_inserted_is_a_test_ == true )
    {
        modeller_.undo();
        last_surface_inserted_is_a_test_ = false;
    }
}

bool RulesProcessor::processSurfaceCreationStatus( bool success )
{
    return true;
}

void RulesProcessor::finishTestSurfaceInsertion()
{
    /* testing_surface_insertion_ = false; */

    /* if ( last_surface_inserted_is_a_test_ == true ) */
    /* { */
    /*     modeller_.undo(); */
    /*     last_surface_inserted_is_a_test_ = false; */
    /* } */
}

bool RulesProcessor::testSurface( size_t surface_index, std::vector<double> &points )
{
    modeller_.disableGeologicRules();
    bool success = createSurface(surface_index, points);

    if ( success )
    {
        testing_surface_insertion_ = true;
    }

    return success;
}

bool RulesProcessor::createSurface( size_t surface_index, std::vector<double> &points )
{
    if ( testing_surface_insertion_ == true )
    {
        modeller_.undo();
        testing_surface_insertion_ = false;
    }

    /* std::vector<size_t> lbounds, ubounds; */
    /* std::vector<size_t> intersected_surfaces; */

    std::vector<double> surface = points;

    size_t first_index, second_index;

    bool status = false;

    status = modeller_.createSurface( surface_index, surface );

    testing_surface_insertion_ = false;

    if ( status == true )
    {
        std::vector<double> dummy_vertices;
        std::vector<size_t> dummy_edges;
        status &= (getMesh(surface_index, dummy_vertices, dummy_edges) > 0);

        if ( status == false )
        {
            modeller_.undo();
        }
    }

    return status;
}



//{} // namespace RRM
