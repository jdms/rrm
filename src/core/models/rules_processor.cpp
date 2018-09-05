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

#include <QWidget>
#include <QMessageBox>

template<typename BOOL = bool>
void displayNotice( std::string header, std::string info )
{
    class Widget : public QWidget {
        public:
            Widget(std::string &header, std::string &info) {
                QMessageBox::information( 
                        this, 
                        tr(header.c_str()),
                        tr(info.c_str()) ); 
            }
    } w(header, info);
}

void deprecationNotice( std::string deprecated_method, std::string new_method, bool &popup_is_enabled )
{
    std::string header, info;

    header = "Deprecation notice:";
    info   = "The following method is deprecated: \n >>> RulesProcessor::" + deprecated_method + "\n\n";
    info  += "Please, use method: \n >>> RulesProcessor::" + new_method + " instead.\n\n";
    info  += "This message appears only once per run.";

    if ( popup_is_enabled )
    {
        displayNotice(header, info);
    }

    popup_is_enabled = false;

    /* if ( !enable_popup ) */
    /* { */
    /*     class Widget : public QWidget { */
    /*         public: */
    /*             Widget(std::string &header, std::string &info) { */
    /*                 QMessageBox::information( */ 
    /*                         this, */ 
    /*                         tr(header.c_str()), */
    /*                         tr(info.c_str()) ); */ 
    /*             } */
    /*     } w(header, info); */
    /* } */

    /* enable_popup = true; */
}

#include "rules_processor.hpp"

#include "colormap/color_map_wrapper.hpp"
//namespace RRM
//{}

RulesProcessor::RulesProcessor()
{
    modeller_.useOpenGLCoordinateSystem();
    setMediumResolution();
}

std::vector<std::size_t> RulesProcessor::getActiveSurfaces()
{
    std::vector<size_t> active_surfaces = {};
    auto surfaces = modeller_.getSurfacesIndices();

    std::vector<float> dummy_vlist;
    std::vector<size_t> dummy_flist;

    for ( auto &sid : surfaces )
    {
        getMesh(sid, dummy_vlist, dummy_flist);
        if ( dummy_flist.size() > 0 )
        {
            active_surfaces.push_back(sid);
        }
    }

    return active_surfaces;
}

std::vector<std::size_t> RulesProcessor::getSurfaces()
{
    auto surfaces = modeller_.getSurfacesIndices();

    return surfaces;
}


std::size_t RulesProcessor::getWidthResolution() const
{
    return modeller_.getWidthDiscretization();
}


// DEPRECATED
std::size_t RulesProcessor::getDepthResolution()
{
    return modeller_.getLengthDiscretization();
}

std::size_t RulesProcessor::getLengthResolution() const
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


void RulesProcessor::stopPreserveAbove()
{
    modeller_.stopPreserveAbove();
}

void RulesProcessor::stopPreserveBelow()
{
    modeller_.stopPreserveBelow();
}

void RulesProcessor::stopPreserveRegion()
{
    modeller_.stopPreserveAbove();
    modeller_.stopPreserveBelow();
}

bool RulesProcessor::preserveAboveIsActive()
{
    std::vector<size_t> unused_variable;
    return modeller_.preserveAboveIsActive(unused_variable);
}

bool RulesProcessor::preserveBelowIsActive()
{
    std::vector<size_t> unused_variable;
    return modeller_.preserveBelowIsActive(unused_variable);
}

/* bool RulesProcessor::preserveRegionIsActive() */
/* { */
/*     return false; */
/* } */

bool RulesProcessor::requestPreserveRegion( std::vector<double> &point )
{
    std::cout << "\n" << point[0] << ", " << point[1] << ", " << point[2] << "\n\n" << std::flush;
	SUtilitiesWrapper u(modeller_);
	auto upper_model = u.getSurfacesIndicesAbovePoint(point[0], point[1], point[2]);
	auto lower_model = u.getSurfacesIndicesBelowPoint(point[0], point[1], point[2]);
	
	bool success = preserveBelow(upper_model);
	success &= preserveAbove(lower_model);
	
	return success;
}

bool RulesProcessor::requestPreserveAbove( std::vector<double> &curve_points )
{
    std::vector<size_t> surfaces_indices;

    if ( getModelAboveSurface(curve_points, surfaces_indices) == false )
    {
        return false;
    }

    return preserveAbove(surfaces_indices);
}

bool RulesProcessor::requestPreserveBelow( std::vector<double> &curve_points )
{
    std::vector<size_t> surfaces_indices;

    if ( getModelBelowSurface(curve_points, surfaces_indices) == false )
    {
        return false;
    }

    return preserveBelow(surfaces_indices);
}

bool RulesProcessor::preserveAbove( std::vector<std::size_t> &lower_model )
{
    return modeller_.preserveAbove(lower_model);
}

bool RulesProcessor::preserveBelow( std::vector<std::size_t> &upper_model )
{
    return modeller_.preserveBelow(upper_model);
}

bool RulesProcessor::getModelAboveSurface( std::vector<double> &curve_points, std::vector<size_t> &upper_model )
{
    struct Point { double x, y, z; } p;

    auto getPoint = []( size_t i, std::vector<double> &v, Point &p ) -> bool
    {
        if ( i >= v.size()/3 )
        {
            return false;
        }


        p.x = v[3*i + 0];
        p.y = v[3*i + 1];
        p.z = v[3*i + 2];

        return true;
    };

    std::vector<size_t> lower_boundary_ = {};
    std::vector<size_t> upper_boundary_ = {};

    if ( curve_points.size() < 3*2 )
    {
        return false;
    }


    std::vector<size_t> cur_region, region, cur_lbounds, cur_ubounds;
    //bool first_cross_ = false, second_cross_ = false;

    SUtilitiesWrapper u(modeller_);

    getPoint(0, curve_points, p);
    lower_boundary_ = u.getSurfacesIndicesBelowPoint(p.x, p.y, p.z);
    upper_boundary_ = u.getSurfacesIndicesAbovePoint(p.x, p.y, p.z);

    for ( size_t i = 1; i < curve_points.size()/3; ++i )
    {
        std::cout << "Trying point: " << i << " of " << curve_points.size()/3 << std::endl << std::flush;
        getPoint(i, curve_points, p);

        cur_lbounds = u.getSurfacesIndicesBelowPoint(p.x, p.y, p.z);
        cur_ubounds = u.getSurfacesIndicesAbovePoint(p.x, p.y, p.z);

        if ( (cur_lbounds != lower_boundary_) || (cur_ubounds != upper_boundary_) )
        {
            std::sort(upper_boundary_.begin(), upper_boundary_.end());
            std::sort(lower_boundary_.begin(), upper_boundary_.end());

            std::set_union(upper_boundary_.begin(), upper_boundary_.end(),
                    cur_ubounds.begin(), cur_ubounds.end(), 
                    std::back_inserter(upper_model));
        }
    }

    return true;
}

bool RulesProcessor::getModelBelowSurface( std::vector<double> &curve_points, std::vector<size_t> &lower_model )
{
    struct Point { double x, y, z; } p;

    auto getPoint = []( size_t i, std::vector<double> &v, Point &p ) -> bool
    {
        if ( i >= v.size()/3 )
        {
            return false;
        }


        p.x = v[3*i + 0];
        p.y = v[3*i + 1];
        p.z = v[3*i + 2];

        return true;
    };

    std::vector<size_t> lower_boundary_ = {};
    std::vector<size_t> upper_boundary_ = {};

    if ( curve_points.size() < 3*2 )
    {
        return false;
    }


    std::vector<size_t> cur_region, region, cur_lbounds, cur_ubounds;
    //bool first_cross_ = false, second_cross_ = false;

    SUtilitiesWrapper u(modeller_);

    getPoint(0, curve_points, p);
    lower_boundary_ = u.getSurfacesIndicesBelowPoint(p.x, p.y, p.z);
    upper_boundary_ = u.getSurfacesIndicesAbovePoint(p.x, p.y, p.z);

    for ( size_t i = 1; i < curve_points.size()/3; ++i )
    {
        std::cout << "Trying point: " << i << " of " << curve_points.size()/3 << std::endl << std::flush;
        getPoint(i, curve_points, p);

        cur_lbounds = u.getSurfacesIndicesBelowPoint(p.x, p.y, p.z);
        cur_ubounds = u.getSurfacesIndicesAbovePoint(p.x, p.y, p.z);

        if ( (cur_lbounds != lower_boundary_) || (cur_ubounds != upper_boundary_) )
        {
            std::sort(upper_boundary_.begin(), upper_boundary_.end());
            std::sort(lower_boundary_.begin(), upper_boundary_.end());

            std::set_union(lower_boundary_.begin(), lower_boundary_.end(),
                    cur_lbounds.begin(), cur_lbounds.end(), 
                    std::back_inserter(lower_model));
        }
    }

    return true;
}

bool RulesProcessor::getUpperBoundaryMesh( std::vector<float> &vlist, std::vector<size_t> &flist )
{
    return false;
}

bool RulesProcessor::getLowerBoundaryMesh( std::vector<float> &vlist, std::vector<size_t> &flist )
{
    return false;
}

bool RulesProcessor::getUpperBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist )
{
    std::vector<size_t> surface_indices;
    if ( modeller_.preserveBelowIsActive(surface_indices) == false )
    {
        return false;
    }

    getLengthCrossSectionCurve(surface_indices[0], cross_sec, vlist, flist);
    auto tmp_vlist = vlist;
    for ( auto &sid : surface_indices )
    {
        if ( getLengthCrossSectionCurve(sid, cross_sec, tmp_vlist, flist) )
        {
            for ( size_t i = 0; i < vlist.size(); ++i )
            {
                if ( vlist[i] < tmp_vlist[i] )
                    vlist[i] = tmp_vlist[i];
            }
        }

    }

    flist.clear();
    for ( size_t i = 0; i < vlist.size() - 1; ++i )
    {
        flist.push_back(i);
        flist.push_back(i+1);
    }

    return true;
}

bool RulesProcessor::getUpperBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    return false;
}

bool RulesProcessor::getUpperBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist )
{
    std::vector<size_t> surface_indices;
    if ( modeller_.preserveBelowIsActive(surface_indices) == false )
    {
        return false;
    }

    getWidthCrossSectionCurve(surface_indices[0], cross_sec, vlist, flist);
    auto tmp_vlist = vlist;
    for ( auto &sid : surface_indices )
    {
        if ( getWidthCrossSectionCurve(sid, cross_sec, tmp_vlist, flist) )
        {
            for ( size_t i = 0; i < vlist.size(); ++i )
            {
                if ( vlist[i] < tmp_vlist[i] )
                    vlist[i] = tmp_vlist[i];
            }
        }

    }

    flist.clear();
    for ( size_t i = 0; i < vlist.size() - 1; ++i )
    {
        flist.push_back(i);
        flist.push_back(i+1);
    }

    return true;
}

bool RulesProcessor::getUpperBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    return false;
}

bool RulesProcessor::getLowerBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist )
{
    std::vector<size_t> surface_indices;
    if ( modeller_.preserveAboveIsActive(surface_indices) == false )
    {
        return false;
    }

    getLengthCrossSectionCurve(surface_indices[0], cross_sec, vlist, flist);
    auto tmp_vlist = vlist;
    for ( auto &sid : surface_indices )
    {
        if ( getLengthCrossSectionCurve(sid, cross_sec, tmp_vlist, flist) )
        {
            for ( size_t i = 0; i < vlist.size(); ++i )
            {
                if ( vlist[i] > tmp_vlist[i] )
                    vlist[i] = tmp_vlist[i];
            }
        }

    }

    flist.clear();
    for ( size_t i = 0; i < vlist.size() - 1; ++i )
    {
        flist.push_back(i);
        flist.push_back(i+1);
    }

    return true;
}

bool RulesProcessor::getLowerBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    return false;
}

bool RulesProcessor::getLowerBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist )
{
    std::vector<size_t> surface_indices;
    if ( modeller_.preserveAboveIsActive(surface_indices) == false )
    {
        return false;
    }

    getWidthCrossSectionCurve(surface_indices[0], cross_sec, vlist, flist);
    auto tmp_vlist = vlist;
    for ( auto &sid : surface_indices )
    {
        if ( getWidthCrossSectionCurve(sid, cross_sec, tmp_vlist, flist) )
        {
            for ( size_t i = 0; i < vlist.size(); ++i )
            {
                if ( vlist[i] > tmp_vlist[i] )
                    vlist[i] = tmp_vlist[i];
            }
        }

    }

    flist.clear();
    for ( size_t i = 0; i < vlist.size() - 1; ++i )
    {
        flist.push_back(i);
        flist.push_back(i+1);
    }

    return true;
}

bool RulesProcessor::getLowerBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    return false;
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
    bool status = modeller_.tryChangeDiscretization(64, 64);
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

    if ( (num_width == 64) && (num_depth == 64) )
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

bool RulesProcessor::setModellingResolution( std::size_t width, std::size_t length )
{
    bool status = modeller_.tryChangeDiscretization(width, length);
    if ( status == false )
    {
        return false;
    }

    std::size_t modelling_width_discretization_ = width;
    std::size_t modelling_length_discretization_ = length;

    return status;
}

bool RulesProcessor::setDiagnosticsResolution( std::size_t width, std::size_t length)
{
    bool status = modeller_.tryChangeDiscretization(width, length);
    if ( status == false )
    {
        return false;
    }

    std::size_t diagnostics_width_discretization_ = width;
    std::size_t diagnostics_length_discretization_ = length;

    return status;
}


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

    length_.x = opengl_x;
    length_.y = opengl_y;
    length_.z = opengl_z;

    std::cout << "updated " << std::endl << std::flush;

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
// DEPRECATED
bool RulesProcessor::defineAbove( size_t surface_index )
{
    return modeller_.createAbove(surface_index);
}

bool RulesProcessor::defineAbove( std::vector<size_t> &surface_indices )
{
    if ( surface_indices.empty() )
    {
        return false;
    }

    size_t surface_index = surface_indices.front();
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
//DEPRECATED
bool RulesProcessor::defineBelow( size_t surface_index )
{
    return modeller_.createBelow(surface_index);
}

bool RulesProcessor::defineBelow( std::vector<size_t> surface_indices ) 
{
    if ( surface_indices.empty() )
    {
        return false;
    }

    size_t surface_index = surface_indices.front();
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

// DEPRECATED
bool RulesProcessor::defineAboveIsActive( size_t &boundary_index )
{
    return modeller_.createAboveIsActive(boundary_index);
}

// DEPRECATED
bool RulesProcessor::defineBelowIsActive( size_t &boundary_index )
{
    return modeller_.createBelowIsActive(boundary_index);
}

bool RulesProcessor::defineAboveIsActive( std::vector<size_t> &boundary_indices )
{
    if ( boundary_indices.empty() )
    {
        return false;
    }

    return modeller_.createAboveIsActive(boundary_indices.front());
}

bool RulesProcessor::defineBelowIsActive( std::vector<size_t> &boundary_indices )
{
    if ( boundary_indices.empty() )
    {
        return false;
    }

    return modeller_.createBelowIsActive(boundary_indices.front());
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
    /* class Widget : public QWidget { */
    /*     public: */
    /*         Widget() { */
    /*             QMessageBox::information( */ 
    /*                     this, */ 
    /*                     tr("Application Name"), */ 
    /*                     tr("An information message.") ); */
    /*         } */
    /* } w; */

    /* QMessageBox::information( */
    /*         this, */
    /*         tr("This method is deprecated") ); */
    /* QMessageBox w; */
    /* w.setInformativeText("This method is deprecated"); */
    /* int ret = w.exec(); */

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
    /* return getUpperBoundaryLengthwiseCrossSection(length, vlist, elist); */
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
    bool status = modeller_.loadBinary(filename) || modeller_.loadJSON(filename);

    /* if ( status == false ) */
    /* { */
    /*     status = modeller_.loadJSON(filename); */
    /* } */

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

    //
    // Zhao: the following two lines control the base discretization that will be used
    // to create the piecewise linear complex.  Notice that every "block" is comprised of 
    // 8 triangles, so that the output triangulation satisfies both the 4-8 and the Delaunay 
    // (for uniformilly sampled vertices) mesh criterions.  
    //
    // Example: 
    // 
    // o*******o*******o*******o*******o  -
    // *\v6    |v7    /*v8             *  |
    // * \  t5 | t7  / *               *  |
    // *  \    |    /  *               *  |
    // *   \   |   /   *               *  |
    // * t4 \  |  / t6 *               *  |
    // *     \ | /     *               *  |
    // *      \|/      *               *  |
    // o-------o-------o       o       o  | length_discretization = 1
    // *v3    /|\v4    *v5             *  |
    // *     / | \     *               *  |
    // * t1 /  |  \ t3 *               *  |
    // *   /   |   \   *               *  |
    // *  /    |    \  *               *  |
    // * /  t0 | t2  \ *               *  |
    // */      |      \*               *  |
    // o*******o*******o*******o*******o  -
    //  v0      v1      v2
    //
    // |-------------------------------|
    //    width_discretization = 2
    //
    // Legend: 
    //      blocks' boundaries are market with: '*'
    //      triangles' boundaries are marked with: '|', '\', '/', '-'
    //      vertices are marked with: 'o'
    // 
    // You can have any number here, but I would strongly suggest to keep these 
    // greater than 16 x 16 and to only pick numbers that are powers of 2.  
    //
    
    diagnostics_width_discretization_ = 16; 
    diagnostics_length_discretization_ = 16; 

    modeller_.changeDiscretization(diagnostics_width_discretization_, diagnostics_length_discretization_);


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

    // Use the following to return to the modeller original discretization

    modeller_.changeDiscretization(modelling_width_discretization_, modelling_length_discretization_);

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
}

void RulesProcessor::stopTestSurfaceInsertion()
{
    testing_surface_insertion_ = false;

    if ( last_surface_inserted_is_a_test_ == true )
    {
        modeller_.undo();
        last_surface_inserted_is_a_test_ = false;
    }
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

    template<typename FunctionType, typename... Args>
bool RulesProcessor::processSurfaceCreation( FunctionType &&surfaceCreator, size_t surface_index, Args&&... args )
{
    if ( last_surface_inserted_is_a_test_ == true )
    {
        modeller_.undo();
        last_surface_inserted_is_a_test_ = false;
    }

    if ( testing_surface_insertion_ == true )
    {
        modeller_.disableGeologicRules();
    }

    bool success = surfaceCreator( surface_index, std::forward<Args>(args)... );

    // Without truncate there is no reson to expect a surface won't be correctly created
    // but we test for vacuous surfaces regardless
    if ( success )
    {
        std::vector<double> dummy_vertices;
        std::vector<size_t> dummy_edges;
        success &= getMesh(surface_index, dummy_vertices, dummy_edges);

        if ( success == false )
        {
            modeller_.undo();
        }
    }

    if ( success )
    {
        if ( testing_surface_insertion_ )
        {
            last_surface_inserted_is_a_test_ = true;
            /* testing_surface_insertion_ = false; */
        }
    }

    return success; 
}

bool RulesProcessor::createSurface( size_t surface_index, std::vector<double> &points )
{
    /* if ( testing_surface_insertion_ == true ) */
    /* { */
    /*     modeller_.undo(); */
    /*     testing_surface_insertion_ = false; */
    /* } */

    /* std::vector<size_t> lbounds, ubounds; */
    /* std::vector<size_t> intersected_surfaces; */

    /* std::vector<double> surface = points; */

    /* size_t first_index, second_index; */

    /* bool status = false; */

    auto surfaceCreator = [this]( size_t s_id, const std::vector<double> &pts ) -> bool 
    {
        return this->modeller_.createSurface(s_id, pts);
    };

    bool success = processSurfaceCreation(surfaceCreator, surface_index, points);
    /* success = surfaceCreator(surface_index, surface);//modeller_.createSurface( surface_index, surface ); */

    /* testing_surface_insertion_ = false; */

    /* if ( success == true ) */
    /* { */
    /*     std::vector<double> dummy_vertices; */
    /*     std::vector<size_t> dummy_edges; */
    /*     success &= (getMesh(surface_index, dummy_vertices, dummy_edges) > 0); */

    /*     if ( success == false ) */
    /*     { */
    /*         modeller_.undo(); */
    /*     } */
    /* } */

    return success;
}

bool RulesProcessor::createLengthwiseExtrudedSurface( size_t surface_id, 
        const std::vector<double> &cross_section_curve_point_data
        )
{
    auto surfaceCreator = [this]( size_t s_id, const std::vector<double> &cross_sec_pts ) -> bool 
    {
        return this->modeller_.createLengthwiseExtrudedSurface(s_id, cross_sec_pts);
    };

    bool success = processSurfaceCreation(surfaceCreator, surface_id, cross_section_curve_point_data);

    return success;
}

bool RulesProcessor::createLengthwiseExtrudedSurface( size_t surface_id, 
        const std::vector<double> &cross_section_curve_point_data, double cross_section_depth, 
        const std::vector<double> &path_curve_point_data 
        )
{
    auto surfaceCreator = [this]( 
            size_t s_id, const std::vector<double> &cross_sec_pts, 
            double cross_sec, const std::vector<double> &path_pts ) -> bool 
    {
        return this->modeller_.createLengthwiseExtrudedSurface(s_id, cross_sec_pts, cross_sec, path_pts);
    };

    bool success = processSurfaceCreation(surfaceCreator, surface_id, cross_section_curve_point_data, 
            cross_section_depth, path_curve_point_data);

    return success;
}

bool RulesProcessor::createWidthwiseExtrudedSurface( size_t surface_id,
        const std::vector<double> &cross_section_curve_point_data
        )
{
    auto surfaceCreator = [this]( size_t s_id, const std::vector<double> &cross_sec_pts ) -> bool 
    {
        return this->modeller_.createWidthwiseExtrudedSurface(s_id, cross_sec_pts);
    };

    bool success = processSurfaceCreation(surfaceCreator, surface_id, cross_section_curve_point_data);

    return success;
}

bool RulesProcessor::createWidthwiseExtrudedSurface( size_t surface_id,
        const std::vector<double> &cross_section_curve_point_data, double cross_section_depth, 
        const std::vector<double> &path_curve_point_data 
        )
{
    auto surfaceCreator = [this]( 
            size_t s_id, const std::vector<double> &cross_sec_pts, 
            double cross_sec, const std::vector<double> &path_pts ) -> bool 
    {
        return this->modeller_.createWidthwiseExtrudedSurface(s_id, cross_sec_pts, cross_sec, path_pts);
    };

    bool success = processSurfaceCreation(surfaceCreator, surface_id, cross_section_curve_point_data, 
            cross_section_depth, path_curve_point_data);

    return success;
}



//{} // namespace RRM
