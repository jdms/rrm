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

#include <chrono>
#include <iomanip>

#include "rules_processor.hpp"
#include "colorwrap.hpp"

/* #include "ode_solver_2d.hpp" */
/* #include "path_guided_surface.hpp" */

/* #include "qt_popup.hpp" */
void displayNotice( std::string, std::string ) {}

//namespace RRM
//{}

RulesProcessor::RulesProcessor(): modeller_(stratmod::SModeller::Instance())
{
    modeller_.useOpenGLCoordinateSystem();
    /* setMediumResolution(); */

    modeller_.getOrigin(origin_.x, origin_.y, origin_.z);
    modeller_.getSize(length_.x, length_.y, length_.z);
}

stratmod::SModeller& RulesProcessor::getSModeller()
{
    return static_cast<stratmod::SModeller&>(modeller_);
}

std::vector<size_t> RulesProcessor::filterActiveSurfaces( std::vector<std::size_t> &surface_ids )
{
    std::vector<size_t> active_surfaces = {};

    std::vector<double> dummy_vlist;
    std::vector<size_t> dummy_flist;

    for ( auto &sid : surface_ids )
    {
        dummy_flist.clear();
        getMesh(sid, dummy_vlist, dummy_flist);
        if ( dummy_flist.size() > 0 )
        {
            active_surfaces.push_back(sid);
        }
    }

    return active_surfaces;
}

std::vector<std::size_t> RulesProcessor::getActiveSurfaces()
{
    auto surface_ids = modeller_.getSurfacesIndices();

    return filterActiveSurfaces(surface_ids);
}
std::vector<std::size_t> RulesProcessor::getSurfaces()
{
    auto surfaces = modeller_.getSurfacesIndices();

    return surfaces;
}

std::vector<std::size_t> RulesProcessor::getOrderedSurfaces()
{
    return modeller_.getOrderedSurfacesIndices();
}

std::vector<std::size_t> RulesProcessor::getOrderedActiveSurfaces()
{
    auto surfaces_ids = modeller_.getOrderedSurfacesIndices();

    auto ordered_surfaces_ids = filterActiveSurfaces(surfaces_ids);
    /* std::cout << "\n\n Ordered active surfaces' ids in RulesProcessor: "; */
    /* for ( auto sid : ordered_surfaces_ids ) */
    /* { */
    /*     std::cout << sid << " "; */
    /* } */
    /* std::cout << "\n\n\n" << std::flush; */

    return ordered_surfaces_ids;
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
    return false;

    /* return modeller_.requestCreateAbove(eligible_surfaces); */
}

bool RulesProcessor::requestCreateBelow( std::vector<size_t> &eligible_surfaces )
{
    return false;

    /* return modeller_.requestCreateBelow(eligible_surfaces); */
}


void RulesProcessor::stopPreserveAbove()
{
    lower_model_ = {};
    pa_is_active_ = false;
    modeller_.stopPreserveAbove();
}

void RulesProcessor::stopPreserveBelow()
{
    upper_model_ = {};
    pb_is_active_ = false;
    modeller_.stopPreserveBelow();
}

void RulesProcessor::stopPreserveRegion()
{
    stopPreserveAbove();
    stopPreserveBelow();
}

bool RulesProcessor::preserveAboveIsActive()
{
    /* return modeller_.preserveAboveIsActive(lower_model_); */
    return pa_is_active_;
}

bool RulesProcessor::preserveBelowIsActive()
{
    /* return modeller_.preserveBelowIsActive(upper_model_); */
    return pb_is_active_;
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

    bool success;

    /* stopPreserveBelow(); */
    /* stopPreserveAbove(); */

    success = preserveBelow(upper_model);
    if ( lower_model.empty() )
        return true;

    success &= preserveAbove(lower_model);

    if ( !success )
    {
        /* stopPreserveBelow(); */
        /* stopPreserveAbove(); */

        success = preserveAbove(lower_model);
        if ( upper_model.empty() )
            return true;

        success &= preserveBelow(upper_model);
    }

    return success;
}

bool RulesProcessor::requestPreserveAbove( std::vector<double> &curve_points )
{
    std::vector<size_t> surfaces_indices, tmp;

    if ( getModelBelowSurface(curve_points, surfaces_indices) == false )
    {
        return false;
    }

    if ( !pa_is_active_ )
    {
        lower_model_ = getSurfaces();
    }

    /* std::sort(surfaces_indices.begin(), surfaces_indices.end()); */
    /* std::sort(lower_model_.begin(), lower_model_.end()); */

    /* std::set_intersection(surfaces_indices.begin(), surfaces_indices.end(), */
    /*         lower_model_.begin(), lower_model_.end(), */
    /*         std::back_inserter(tmp)); */

    tmp = surfaces_indices;

    bool success = preserveAbove(tmp);

    if (!success)
    {
        lower_model_ = {};
    }

    return success;
}

bool RulesProcessor::requestPreserveBelow( std::vector<double> &curve_points )
{
    std::vector<size_t> surfaces_indices, tmp;

    if ( getModelAboveSurface(curve_points, surfaces_indices) == false )
    {
        return false;
    }

    if ( !pb_is_active_ )
    {
        upper_model_ = getSurfaces();
    }

    /* std::sort(surfaces_indices.begin(), surfaces_indices.end()); */
    /* std::sort(upper_model_.begin(), upper_model_.end()); */

    /* std::set_intersection(surfaces_indices.begin(), surfaces_indices.end(), */
    /*         upper_model_.begin(), upper_model_.end(), */
    /*         std::back_inserter(tmp)); */

    tmp = surfaces_indices;

    bool success = preserveBelow(tmp);

    return success;
}

bool RulesProcessor::preserveAbove( std::vector<std::size_t> &lower_model )
{
    pa_is_active_ = modeller_.preserveAbove(lower_model);

    if (pa_is_active_)
    {
        lower_model_ = lower_model;
    }
    else
    {
        lower_model_ = {};
    }

    return pa_is_active_;
}

bool RulesProcessor::preserveBelow( std::vector<std::size_t> &upper_model )
{
    pb_is_active_ = modeller_.preserveBelow(upper_model);

    if (pb_is_active_)
    {
        upper_model_ = upper_model;
    }
    else
    {
        upper_model_ = {};
    }

    return pb_is_active_;
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
            std::sort(cur_ubounds.begin(), cur_ubounds.end());

            std::set_union(upper_boundary_.begin(), upper_boundary_.end(),
                    cur_ubounds.begin(), cur_ubounds.end(),
                    std::back_inserter(upper_model));

            break;
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
            std::sort(cur_lbounds.begin(), cur_lbounds.end());
            std::sort(lower_boundary_.begin(), lower_boundary_.end());

            std::set_union(lower_boundary_.begin(), lower_boundary_.end(),
                    cur_lbounds.begin(), cur_lbounds.end(),
                    std::back_inserter(lower_model));

            break;
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

bool RulesProcessor::getModelInfCurveAtLength( std::vector<std::size_t> &surface_indices, std::size_t length,
        std::vector<double> &vlist, std::vector<std::size_t> &elist )
{
    if ( surface_indices.empty() )
    {
        return false;
    }

    bool success = false;
    size_t index = 0;

    success = modeller_.getLengthCrossSectionCurve(surface_indices[0], length, vlist, elist);
    if (!success)
    {
        std::vector<double> vl{};
        std::vector<std::size_t> el{};
        for (std::size_t i = index + 1; i < surface_indices.size(); ++i)
        {
            success |= modeller_.getLengthCrossSectionCurve(surface_indices[i], length, vl, el);
            if (success)
                break;
        }
        if (!success)
            return false;
    }
    /* for ( index = 0; index < surface_indices.size(); ++index ) */
    /* { */
    /*     if ( modeller_.getLengthCrossSectionCurve(surface_indices[0], length, vlist, elist) ) */
    /*     { */
    /*         success = true; */
    /*         break; */
    /*     } */
    /* } */

    /* if ( !success ) */
    /* { */
    /*     return false; */
    /* } */

    std::vector<double> tmp_vlist;
    size_t sid;
    for ( index = 1; index < surface_indices.size(); ++index )
    {
        sid = surface_indices[index];

        modeller_.getLengthCrossSectionCurve(sid, length, tmp_vlist, elist);
        if ( true )
        {
            for ( size_t i = 0; i < vlist.size(); ++i )
            {
                if ( vlist[i] > tmp_vlist[i] )
                    vlist[i] = tmp_vlist[i];
            }
        }

    }

    elist.clear();
    for ( size_t i = 0; i < vlist.size()/2 -1; ++i )
    {
        elist.push_back(i);
        elist.push_back(i+1);
    }

    return true;

}

bool RulesProcessor::getModelSupCurveAtLength( std::vector<std::size_t> &surface_indices, std::size_t length,
        std::vector<double> &vlist, std::vector<std::size_t> &elist )
{
    if ( surface_indices.empty() )
    {
        return false;
    }

    bool success = false;
    size_t index = 0;

    success = modeller_.getLengthCrossSectionCurve(surface_indices[0], length, vlist, elist);
    if (!success)
    {
        std::vector<double> vl{};
        std::vector<std::size_t> el{};
        for (std::size_t i = index + 1; i < surface_indices.size(); ++i)
        {
            success |= modeller_.getLengthCrossSectionCurve(surface_indices[i], length, vl, el);
            if (success)
                break;
        }
        if (!success)
            return false;
    }
    /* for ( index = 0; index < surface_indices.size(); ++index ) */
    /* { */
    /*     if ( modeller_.getLengthCrossSectionCurve(surface_indices[0], length, vlist, elist) ) */
    /*     { */
    /*         success = true; */
    /*         break; */
    /*     } */
    /* } */

    /* if ( !success ) */
    /* { */
    /*     return false; */
    /* } */

    std::vector<double> tmp_vlist;
    size_t sid;
    for ( index = 1; index < surface_indices.size(); ++index )
    {
        sid = surface_indices[index];

        modeller_.getLengthCrossSectionCurve(sid, length, tmp_vlist, elist);
        if ( true )
        {
            for ( size_t i = 0; i < vlist.size(); ++i )
            {
                if ( vlist[i] < tmp_vlist[i] )
                    vlist[i] = tmp_vlist[i];
            }
        }

    }

    elist.clear();
    for ( size_t i = 0; i < vlist.size()/2 -1; ++i )
    {
        elist.push_back(i);
        elist.push_back(i+1);
    }

    return true;
}

bool RulesProcessor::getModelInfCurveAtWidth( std::vector<std::size_t> &surface_indices, std::size_t width,
        std::vector<double> &vlist, std::vector<std::size_t> &elist )
{
    if ( surface_indices.empty() )
    {
        return false;
    }

    bool success = false;
    size_t index = 0;

    success = modeller_.getWidthCrossSectionCurve(surface_indices[0], width, vlist, elist);
    if (!success)
    {
        std::vector<double> vl{};
        std::vector<std::size_t> el{};
        for (std::size_t i = index + 1; i < surface_indices.size(); ++i)
        {
            success |= modeller_.getWidthCrossSectionCurve(surface_indices[i], width, vlist, elist);
            if (success)
                break;
        }
        if (!success)
            return false;
    }
    /* for ( index = 0; index < surface_indices.size(); ++index ) */
    /* { */
    /*     if ( modeller_.getWidthCrossSectionCurve(surface_indices[0], width, vlist, elist) ) */
    /*     { */
    /*         success = true; */
    /*         break; */
    /*     } */
    /* } */

    /* if ( !success ) */
    /* { */
    /*     return false; */
    /* } */

    std::vector<double> tmp_vlist;
    size_t sid;
    for ( index = 1; index < surface_indices.size(); ++index )
    {
        sid = surface_indices[index];

        modeller_.getWidthCrossSectionCurve(sid, width, tmp_vlist, elist);
        if ( true )
        {
            for ( size_t i = 0; i < vlist.size(); ++i )
            {
                if ( vlist[i] > tmp_vlist[i] )
                    vlist[i] = tmp_vlist[i];
            }
        }

    }

    elist.clear();
    for ( size_t i = 0; i < vlist.size()/2 -1; ++i )
    {
        elist.push_back(i);
        elist.push_back(i+1);
    }

    return true;

}

bool RulesProcessor::getModelSupCurveAtWidth( std::vector<std::size_t> &surface_indices, std::size_t width,
        std::vector<double> &vlist, std::vector<std::size_t> &elist )
{
    if ( surface_indices.empty() )
    {
        return false;
    }

    bool success = false;
    size_t index = 0;

    success = modeller_.getWidthCrossSectionCurve(surface_indices[0], width, vlist, elist);
    if (!success)
    {
        std::vector<double> vl{};
        std::vector<std::size_t> el{};
        for (std::size_t i = index + 1; i < surface_indices.size(); ++i)
        {
            success |= modeller_.getWidthCrossSectionCurve(surface_indices[i], width, vlist, elist);
            if (success)
                break;
        }
        if (!success)
            return false;
    }
    /* for ( index = 0; index < surface_indices.size(); ++index ) */
    /* { */
    /*     if ( modeller_.getWidthCrossSectionCurve(surface_indices[0], width, vlist, elist) ) */
    /*     { */
    /*         success = true; */
    /*         break; */
    /*     } */
    /* } */

    /* if ( !success ) */
    /* { */
    /*     return false; */
    /* } */

    std::vector<double> tmp_vlist;
    size_t sid;
    for ( ++index; index < surface_indices.size(); ++index )
    {
        sid = surface_indices[index];

        modeller_.getWidthCrossSectionCurve(sid, width, tmp_vlist, elist);
        if (true )
        {
            for ( size_t i = 0; i < vlist.size(); ++i )
            {
                if ( vlist[i] < tmp_vlist[i] )
                    vlist[i] = tmp_vlist[i];
            }
        }

    }

    elist.clear();
    for ( size_t i = 0; i < vlist.size()/2 -1; ++i )
    {
        elist.push_back(i);
        elist.push_back(i+1);
    }

    return true;
}

template<typename T>
void convertEntireCurveToBox( std::vector<T> &vlist, std::vector<size_t> &flist, double box_boundary_height )
{
    size_t last_vertex_id = flist.back();

    // from last vertex to right lower bbox
    double last_abscissa = vlist[ 2*last_vertex_id ];
    vlist.push_back(last_abscissa);
    vlist.push_back(box_boundary_height);
    flist.push_back(last_vertex_id);
    flist.push_back(last_vertex_id+1);

    // from right lower bbox to left lower bbox
    double first_abscissa = vlist[ 0 ];
    vlist.push_back(first_abscissa);
    vlist.push_back(box_boundary_height);
    flist.push_back(last_vertex_id+1);
    flist.push_back(last_vertex_id+2);

    // from left lower bbox to first vertex
    flist.push_back(last_vertex_id+2);
    flist.push_back(0);

    // see what is going on
    std::cout << "\n\nBox' extremal points: \n"
        << "(" << vlist[0] << ", " << vlist[1] << ") -- "
        << "(" << vlist[ 2*last_vertex_id ] << ", " << vlist[ 2*last_vertex_id +1 ] << "); \n"

        << "(" << vlist[ 2*last_vertex_id ] << ", " << vlist[ 2*last_vertex_id +1 ] << ") -- "
        << "(" << vlist[ 2*(last_vertex_id+1) ] << ", " << vlist[ 2*(last_vertex_id+1) +1 ] << "); \n"

        << "(" << vlist[ 2*(last_vertex_id+1) ] << ", " << vlist[ 2*(last_vertex_id+1) +1 ] << ") -- "
        << "(" << vlist[ 2*(last_vertex_id+2) ] << ", " << vlist[ 2*(last_vertex_id+2) +1 ] << "); \n"

        << "(" << vlist[ 2*(last_vertex_id+2) ] << ", " << vlist[ 2*(last_vertex_id+2) +1 ] << ") -- \n"
        << "(" << vlist[ 0 ] << ", " << vlist[ 1 ] << "); \n\n" << std::flush;
}

bool RulesProcessor::getPreserveAboveCurveBoxAtLength( size_t length, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    if ( preserveAboveIsActive() == false )
    {
        return false;
    }

    bool success = getModelSupCurveAtLength(lower_model_, length, vlist, flist);

    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(vlist, flist, origin_.y+length_.y);

    return true;
}

bool RulesProcessor::getPreserveBelowCurveBoxAtLength( size_t length, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    if ( preserveBelowIsActive() == false )
    {
        return false;
    }

    bool success = getModelInfCurveAtLength(upper_model_, length, vlist, flist);

    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(vlist, flist, origin_.y);

    return true;
}

bool RulesProcessor::getPreserveAboveCurveBoxAtWidth( size_t width, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    std::vector<size_t> surface_indices;
    if ( modeller_.preserveAboveIsActive(surface_indices) == false )
    {
        return false;
    }


    bool success = getModelSupCurveAtWidth(surface_indices, width, vlist, flist);

    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(vlist, flist, origin_.y+length_.y);

    return true;
}

bool RulesProcessor::getPreserveBelowCurveBoxAtWidth( size_t width, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    std::vector<size_t> surface_indices;
    if ( modeller_.preserveBelowIsActive(surface_indices) == false )
    {
        return false;
    }

    bool success = getModelInfCurveAtWidth(surface_indices, width, vlist, flist);

    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(vlist, flist, origin_.y);

    return true;
}

bool RulesProcessor::getRegionCurveBoxesAtLength( std::size_t region_id, std::size_t length,
        std::vector<double> &lower_bound_box_vlist, std::vector<std::size_t> &lower_bound_box_elist,
        std::vector<double> &upper_bound_box_vlist, std::vector<std::size_t> &upper_bound_box_elist )
{
    stratmod::SUtilities u(modeller_);

    std::vector<size_t> lower_bound, upper_bound;

    bool success = u.getBoundingSurfacesFromRegionID(region_id, lower_bound, upper_bound);
    if ( !success )
    {
        return false;
    }

    success = getModelSupCurveAtLength(lower_bound, length, lower_bound_box_vlist, lower_bound_box_elist);
    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(lower_bound_box_vlist, lower_bound_box_elist, origin_.y + length_.y);

    success = getModelInfCurveAtLength(upper_bound, length, upper_bound_box_vlist, upper_bound_box_elist);
    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(upper_bound_box_vlist, upper_bound_box_elist, origin_.y);

    return success;
}

bool RulesProcessor::getRegionCurveBoxesAtWidth( std::size_t region_id, std::size_t width,
        std::vector<double> &lower_bound_box_vlist, std::vector<std::size_t> &lower_bound_box_elist,
        std::vector<double> &upper_bound_box_vlist, std::vector<std::size_t> &upper_bound_box_elist )
{
    stratmod::SUtilities u(modeller_);

    std::vector<size_t> lower_bound, upper_bound;

    bool success = u.getBoundingSurfacesFromRegionID(region_id, lower_bound, upper_bound);
    if ( !success )
    {
        return false;
    }

    success = getModelSupCurveAtWidth(lower_bound, width, lower_bound_box_vlist, lower_bound_box_elist);
    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(lower_bound_box_vlist, lower_bound_box_elist, origin_.y + length_.y);

    success = getModelInfCurveAtWidth(upper_bound, width, upper_bound_box_vlist, upper_bound_box_elist);
    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(upper_bound_box_vlist, upper_bound_box_elist, origin_.y);

    return success;
}

bool RulesProcessor::getUpperBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    std::vector<size_t> surface_indices;
    if ( modeller_.preserveBelowIsActive(surface_indices) == false )
    {
        return false;
    }

    bool success = getModelInfCurveAtLength(surface_indices, cross_sec, vlist, flist);

    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(vlist, flist, origin_.y);

    return true;
}

bool RulesProcessor::getUpperBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist )
{
    return false;

    /* std::cout << "\n\n\nInside getUpperBoundaryLengthwise...\n\n" << std::flush; */

    /* std::vector<size_t> surface_indices; */
    /* if ( modeller_.preserveBelowIsActive(surface_indices) == false ) */
    /* { */
    /*     std::cout << "preserveBelowIsActive() == false\n\n\n" << std::flush; */
    /*     return false; */
    /* } */

    /* bool success = false; */
    /* size_t index; */
    /* for ( index = 0; index < surface_indices.size(); ++index ) */
    /* { */
    /*     if ( modeller_.getLengthCrossSectionCurve(surface_indices[0], cross_sec, vlist, flist) ) */
    /*     { */
    /*         std::cout << "Got a first bounding curve at index: " << index */
    /*             << " of " << surface_indices.size() << "\n" << std::flush; */
    /*         std::cout << "vlist.size() == " << vlist.size() << "\n" << std::flush; */

    /*         success = true; */
    /*         break; */
    /*     } */
    /* } */

    /* if ( !success ) */
    /* { */
    /*     return false; */
    /* } */

    /* auto tmp_vlist = vlist; */
    /* size_t sid; */
    /* for ( ++index; index < surface_indices.size(); ++index ) */
    /* { */
    /*     std::cout << "Processing surface: " << index */
    /*         << " of " << surface_indices.size() -1 << "\n" << std::flush; */
    /*     sid = surface_indices[index]; */
    /*     std::cout << "sid == " << sid << "\n" << std::flush; */

    /*     if ( modeller_.getLengthCrossSectionCurve(sid, cross_sec, tmp_vlist, flist) ) */
    /*     { */
    /*         std::cout << "tmp_vlist.size() == " << tmp_vlist.size() << "\n" << std::flush; */
    /*         for ( size_t i = 0; i < vlist.size(); ++i ) */
    /*         { */
    /*             if ( vlist[i] > tmp_vlist[i] ) */
    /*                 vlist[i] = tmp_vlist[i]; */
    /*         } */
    /*     } */

    /* } */

    /* flist.clear(); */
    /* for ( size_t i = 0; i < vlist.size()/2 -1; ++i ) */
    /* { */
    /*     flist.push_back(i); */
    /*     flist.push_back(i+1); */
    /* } */

    /* std::cout << "flist.size() == " << flist.size() << "\n" << std::flush; */
    /* std::cout << "flist.back() == " << flist.back() << "\n" << std::flush; */

    /* convertEntireCurveToBox(vlist, flist, origin_.y); */

    /* std::cout << "Got boundary curve\n\n\n" << std::flush; */
    /* return true; */
}

bool RulesProcessor::getUpperBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    std::vector<size_t> surface_indices;
    if ( modeller_.preserveBelowIsActive(surface_indices) == false )
    {
        return false;
    }

    bool success = getModelInfCurveAtWidth(surface_indices, cross_sec, vlist, flist);

    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(vlist, flist, origin_.y);

    return true;
}

bool RulesProcessor::getUpperBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist )
{
    return false;

    /* std::cout << "\n\n\nInside getUpperBoundaryWidthwise...\n\n" << std::flush; */

    /* std::vector<size_t> surface_indices; */
    /* if ( modeller_.preserveBelowIsActive(surface_indices) == false ) */
    /* { */
    /*     std::cout << "preserveBelowIsActive() == false\n\n\n" << std::flush; */
    /*     return false; */
    /* } */

    /* bool success = false; */
    /* size_t index; */
    /* for ( index = 0; index < surface_indices.size(); ++index ) */
    /* { */
    /*     if ( modeller_.getWidthCrossSectionCurve(surface_indices[0], cross_sec, vlist, flist) ) */
    /*     { */
    /*         std::cout << "Got a first bounding curve at index: " << index */
    /*             << " of " << surface_indices.size() << "\n" << std::flush; */
    /*         std::cout << "vlist.size() == " << vlist.size() << "\n" << std::flush; */

    /*         success = true; */
    /*         break; */
    /*     } */
    /* } */

    /* if ( !success ) */
    /* { */
    /*     return false; */
    /* } */

    /* auto tmp_vlist = vlist; */
    /* size_t sid; */
    /* for ( ++index; index < surface_indices.size(); ++index ) */
    /* { */
    /*     std::cout << "Processing surface: " << index */
    /*         << " of " << surface_indices.size() -1 << "\n" << std::flush; */
    /*     sid = surface_indices[index]; */
    /*     std::cout << "sid == " << sid << "\n" << std::flush; */

    /*     if ( modeller_.getWidthCrossSectionCurve(sid, cross_sec, tmp_vlist, flist) ) */
    /*     { */
    /*         std::cout << "tmp_vlist.size() == " << tmp_vlist.size() << "\n" << std::flush; */
    /*         for ( size_t i = 0; i < vlist.size(); ++i ) */
    /*         { */
    /*             if ( vlist[i] > tmp_vlist[i] ) */
    /*                 vlist[i] = tmp_vlist[i]; */
    /*         } */
    /*     } */

    /* } */

    /* flist.clear(); */
    /* for ( size_t i = 0; i < vlist.size()/2 -1; ++i ) */
    /* { */
    /*     flist.push_back(i); */
    /*     flist.push_back(i+1); */
    /* } */

    /* std::cout << "flist.size() == " << flist.size() << "\n" << std::flush; */
    /* std::cout << "flist.back() == " << flist.back() << "\n" << std::flush; */

    /* convertEntireCurveToBox(vlist, flist, origin_.y); */

    /* std::cout << "Got boundary curve\n\n\n" << std::flush; */
    /* return true; */
}

bool RulesProcessor::getLowerBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    std::vector<size_t> surface_indices;
    if ( modeller_.preserveAboveIsActive(surface_indices) == false )
    {
        return false;
    }

    bool success = getModelSupCurveAtLength(surface_indices, cross_sec, vlist, flist);

    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(vlist, flist, origin_.y+length_.y);

    return true;
}

bool RulesProcessor::getLowerBoundaryLengthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist )
{
    return false;

    /* std::cout << "\n\n\nInside getLowerBoundaryLengthwise...\n\n" << std::flush; */

    /* std::vector<size_t> surface_indices; */
    /* if ( modeller_.preserveAboveIsActive(surface_indices) == false ) */
    /* { */
    /*     std::cout << "preserveAboveIsActive() == false\n\n\n" << std::flush; */
    /*     return false; */
    /* } */

    /* bool success = false; */
    /* size_t index; */
    /* for ( index = 0; index < surface_indices.size(); ++index ) */
    /* { */
    /*     if ( modeller_.getLengthCrossSectionCurve(surface_indices[0], cross_sec, vlist, flist) ) */
    /*     { */
    /*         std::cout << "Got a first bounding curve at index: " << index */
    /*             << " of " << surface_indices.size() << "\n" << std::flush; */
    /*         std::cout << "vlist.size() == " << vlist.size() << "\n" << std::flush; */

    /*         success = true; */
    /*         break; */
    /*     } */
    /* } */

    /* if ( !success ) */
    /* { */
    /*     return false; */
    /* } */

    /* auto tmp_vlist = vlist; */
    /* size_t sid; */
    /* for ( ++index; index < surface_indices.size(); ++index ) */
    /* { */
    /*     std::cout << "Processing surface: " << index */
    /*         << " of " << surface_indices.size() -1 << "\n" << std::flush; */
    /*     sid = surface_indices[index]; */
    /*     std::cout << "sid == " << sid << "\n" << std::flush; */

    /*     if ( modeller_.getLengthCrossSectionCurve(sid, cross_sec, tmp_vlist, flist) ) */
    /*     { */
    /*         std::cout << "tmp_vlist.size() == " << tmp_vlist.size() << "\n" << std::flush; */
    /*         for ( size_t i = 0; i < vlist.size(); ++i ) */
    /*         { */
    /*             if ( vlist[i] < tmp_vlist[i] ) */
    /*                 vlist[i] = tmp_vlist[i]; */
    /*         } */
    /*     } */

    /* } */

    /* flist.clear(); */
    /* for ( size_t i = 0; i < vlist.size()/2 -1; ++i ) */
    /* { */
    /*     flist.push_back(i); */
    /*     flist.push_back(i+1); */
    /* } */

    /* std::cout << "flist.size() == " << flist.size() << "\n" << std::flush; */
    /* std::cout << "flist.back() == " << flist.back() << "\n" << std::flush; */

    /* convertEntireCurveToBox(vlist, flist, origin_.y+length_.y); */

    /* std::cout << "Got boundary curve\n\n\n" << std::flush; */
    /* return true; */
}

bool RulesProcessor::getLowerBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    std::vector<size_t> surface_indices;
    if ( modeller_.preserveAboveIsActive(surface_indices) == false )
    {
        return false;
    }


    bool success = getModelSupCurveAtWidth(surface_indices, cross_sec, vlist, flist);

    if ( !success )
    {
        return false;
    }

    convertEntireCurveToBox(vlist, flist, origin_.y+length_.y);

    return true;
}

bool RulesProcessor::getLowerBoundaryWidthwiseCrossSection( size_t cross_sec, std::vector<float> &vlist, std::vector<size_t> &flist )
{
    return false;

    /* std::cout << "\n\n\nInside getUpperBoundaryWidthwise...\n\n" << std::flush; */

    /* std::vector<size_t> surface_indices; */
    /* if ( modeller_.preserveAboveIsActive(surface_indices) == false ) */
    /* { */
    /*     std::cout << "preserveAboveIsActive() == false\n\n\n" << std::flush; */
    /*     return false; */
    /* } */

    /* bool success = false; */
    /* size_t index; */
    /* for ( index = 0; index < surface_indices.size(); ++index ) */
    /* { */
    /*     if ( modeller_.getWidthCrossSectionCurve(surface_indices[0], cross_sec, vlist, flist) ) */
    /*     { */
    /*         std::cout << "Got a first bounding curve at index: " << index */
    /*             << " of " << surface_indices.size() << "\n" << std::flush; */
    /*         std::cout << "vlist.size() == " << vlist.size() << "\n" << std::flush; */

    /*         success = true; */
    /*         break; */
    /*     } */
    /* } */

    /* if ( !success ) */
    /* { */
    /*     return false; */
    /* } */

    /* auto tmp_vlist = vlist; */
    /* size_t sid; */
    /* for ( ++index; index < surface_indices.size(); ++index ) */
    /* { */
    /*     std::cout << "Processing surface: " << index */
    /*         << " of " << surface_indices.size() -1 << "\n" << std::flush; */
    /*     sid = surface_indices[index]; */
    /*     std::cout << "sid == " << sid << "\n" << std::flush; */

    /*     if ( modeller_.getWidthCrossSectionCurve(sid, cross_sec, tmp_vlist, flist) ) */
    /*     { */
    /*         std::cout << "tmp_vlist.size() == " << tmp_vlist.size() << "\n" << std::flush; */
    /*         for ( size_t i = 0; i < vlist.size(); ++i ) */
    /*         { */
    /*             if ( vlist[i] < tmp_vlist[i] ) */
    /*                 vlist[i] = tmp_vlist[i]; */
    /*         } */
    /*     } */

    /* } */

    /* flist.clear(); */
    /* for ( size_t i = 0; i < vlist.size()/2 -1; ++i ) */
    /* { */
    /*     flist.push_back(i); */
    /*     flist.push_back(i+1); */
    /* } */

    /* std::cout << "flist.size() == " << flist.size() << "\n" << std::flush; */
    /* std::cout << "flist.back() == " << flist.back() << "\n" << std::flush; */

    /* convertEntireCurveToBox(vlist, flist, origin_.y+length_.y); */

    /* std::cout << "Got boundary curve\n\n\n" << std::flush; */
    /* return true; */
}


/* Change the model's properties */


bool RulesProcessor::setLowResolution()
{
    /* bool status = modeller_.tryChangeDiscretization(16, 16); */
    bool status = modeller_.changeDiscretization(16, 16);
    if ( status == false )
    {
        return false;
    }

    current_resolution_ = LOW;

    return status;
}

bool RulesProcessor::setMediumResolution()
{
    /* bool status = modeller_.tryChangeDiscretization(64, 64); */
    bool status = modeller_.changeDiscretization(64, 64);
    if ( status == false )
    {
        return false;
    }

    current_resolution_ = MEDIUM;

    return status;
}

bool RulesProcessor::setHighResolution()
{
    /* bool status = modeller_.tryChangeDiscretization(64, 64); */
    bool status = modeller_.changeDiscretization(64, 64);
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
    size_t num_depth = getLengthResolution();

    if ( (num_width == 16) && (num_depth == 16) )
    {
        return true;
    }

    return false;
}

bool RulesProcessor::isMediumResolution()
{
    size_t num_width = getWidthResolution();
    size_t num_depth = getLengthResolution();

    if ( (num_width == 64) && (num_depth == 64) )
    {
        return true;
    }

    return false;
}

bool RulesProcessor::isHighResolution()
{
    size_t num_width = getWidthResolution();
    size_t num_depth = getLengthResolution();

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
    /* bool status = modeller_.tryChangeDiscretization(width, length); */
    bool status = modeller_.changeDiscretization(width, length);
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
    /* bool status = modeller_.tryChangeDiscretization(width, length); */
    bool status = modeller_.changeDiscretization(width, length);
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
    modeller_.getOrigin(origin_.x, origin_.y, origin_.z);
}

void RulesProcessor::getLenght( double &opengl_x, double &opengl_y, double &opengl_z )
{
    modeller_.getSize(opengl_x, opengl_y, opengl_z);
    modeller_.getSize(length_.x, length_.y, length_.z);
}


/* Begin methods to interface with GUI */


/* Clean up */
void RulesProcessor::clear()
{
    modeller_.clear();

    upper_model_ = {};
    pb_is_active_ = false;
    lower_model_ = {};
    pa_is_active_ = false;
    modelling_length_discretization_ = 64;
    modelling_width_discretization_ = 64;

    diagnostics_length_discretization_ = 32;
    diagnostics_width_discretization_ = 32;

    testing_surface_insertion_ = false;
    last_surface_inserted_is_a_test_ = false;

    modeller_.getOrigin(origin_.x, origin_.y, origin_.z);
    modeller_.getSize(length_.x, length_.y, length_.z);
}


/* Query or modify the automatum state */


//
// brief:
// Define new input region above surface which index is `surface_index`.
//
// DEPRECATED
bool RulesProcessor::defineAbove( size_t surface_index )
{
    displayNotice("Warning", "Inside defineAbove( size_t )");
    return false;

    /* return modeller_.createAbove(surface_index); */
}

bool RulesProcessor::defineAbove( std::vector<size_t> &surface_indices )
{
    displayNotice("Warning", "Inside defineAbove( vector<size_t> & )");
    return false;

    /* if ( surface_indices.empty() ) */
    /* { */
    /*     return false; */
    /* } */

    /* size_t surface_index = surface_indices.front(); */
    /* return modeller_.createAbove(surface_index); */
}

//
// brief:
// Clear any previous `defineAbove()` call.
// Safe to call anytime.
//
void RulesProcessor::stopDefineAbove()
{
    displayNotice("Warning", "Inside stopDefineAbove()");
    return;

    /* return modeller_.stopCreateAbove(); */
}

//
// brief:
// Define new input region below surface which index is `surface_index`.
//
//DEPRECATED
bool RulesProcessor::defineBelow( size_t surface_index )
{
    displayNotice("Warning", "Inside defineBelow( size_t )");
    return false;

    /* return modeller_.createBelow(surface_index); */
}

bool RulesProcessor::defineBelow( std::vector<size_t> surface_indices )
{
    displayNotice("Warning", "Inside defineBelow( vector<size_t> & )");
    return false;

    /* if ( surface_indices.empty() ) */
    /* { */
    /*     return false; */
    /* } */

    /* size_t surface_index = surface_indices.front(); */
    /* return modeller_.createBelow(surface_index); */
}
//
// brief:
// Clear any previous `defineBelow()` call.
// Safe to call anytime.
//
void RulesProcessor::stopDefineBelow()
{
    displayNotice("Warning", "Inside stopDefineBelow()");
    return;

    /* modeller_.stopCreateBelow(); */
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
    displayNotice("Warning", "Inside defineAboveIsActive()");
    return false;

    /* size_t dummy_index; */
    /* return modeller_.createAboveIsActive(dummy_index); */
}

bool RulesProcessor::defineBelowIsActive()
{
    displayNotice("Warning", "Inside defineBelowIsActive()");
    return false;

    /* size_t dummy_index; */
    /* return modeller_.createBelowIsActive(dummy_index); */
}

// DEPRECATED
bool RulesProcessor::defineAboveIsActive( size_t &boundary_index )
{
    displayNotice("Warning", "Inside defineAboveIsActive(size_t &)");
    return false;

    /* return modeller_.createAboveIsActive(boundary_index); */
}

// DEPRECATED
bool RulesProcessor::defineBelowIsActive( size_t &boundary_index )
{
    displayNotice("Warning", "Inside defineBelowIsActive(size_t &)");
    return false;

    /* return modeller_.createBelowIsActive(boundary_index); */
}

bool RulesProcessor::defineAboveIsActive( std::vector<size_t> &boundary_indices )
{
    displayNotice("Warning", "Inside defineAboveIsActive(vector<size_t> &)");
    return false;

    /* if ( boundary_indices.empty() ) */
    /* { */
    /*     return false; */
    /* } */

    /* return modeller_.createAboveIsActive(boundary_indices.front()); */
}

bool RulesProcessor::defineBelowIsActive( std::vector<size_t> &boundary_indices )
{
    displayNotice("Warning", "Inside defineBelowIsActive(vector<size_t> &)");
    return false;

    /* if ( boundary_indices.empty() ) */
    /* { */
    /*     return false; */
    /* } */

    /* return modeller_.createBelowIsActive(boundary_indices.front()); */
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

    bool status = modeller_.undo();

    enforcePreserveRegion();

    return status;
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

    bool status = modeller_.redo();

    enforcePreserveRegion();

    return status;
}


bool RulesProcessor::getMesh( size_t surface_id, std::vector<float> &vlist, std::vector<size_t> &flist )
{
    return false;

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

    /* return modeller_.getMesh(surface_id, vlist, flist); */
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
    return false;

    /* return modeller_.getLengthCrossSectionCurve(surface_id, length, vlist, elist); */
}

bool RulesProcessor::getCrossSection( size_t surface_id, size_t length, std::vector<double> &vlist, std::vector<size_t> &elist )
{
    //    std::cout << "Getting cross section: " << surface_id << ":" << depth << "\n" << std::flush;

    SUtilitiesWrapper u(modeller_);
    return u.getAdaptedLengthCrossSectionCurve(surface_id, length, vlist, elist);
}

bool RulesProcessor::getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<float> &vlist, std::vector<size_t> &elist )
{
    return false;

    /* return modeller_.getWidthCrossSectionCurve(surface_id, width, vlist, elist); */
}

bool RulesProcessor::getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<double> &vlist, std::vector<size_t> &elist )
{

    /* std::cout << "\n\nInside getWidthCrossSectionCurve() [double]\n\n" << std::flush; */
    /* if ( (width % 2) == 0 ) */
    /* { */
    /*     return getUpperBoundaryLengthwiseCrossSection(width, vlist, elist); */
    /* } */
    /* else */
    /* { */
    /*     return getLowerBoundaryLengthwiseCrossSection(width, vlist, elist); */
    /* } */

    /* return modeller_.getWidthCrossSectionCurve(surface_id, width, vlist, elist); */
    SUtilitiesWrapper u(modeller_);
    return u.getAdaptedWidthCrossSectionCurve(surface_id, width, vlist, elist);
}

bool RulesProcessor::getLengthCrossSectionCurve( size_t surface_id, size_t length, std::vector<float> &vlist, std::vector<size_t> &elist )
{
    return false;

    /* return modeller_.getLengthCrossSectionCurve(surface_id, length, vlist, elist); */
}

bool RulesProcessor::getLengthCrossSectionCurve( size_t surface_id, size_t length, std::vector<double> &vlist, std::vector<size_t> &elist )
{
    /* return modeller_.getLengthCrossSectionCurve(surface_id, length, vlist, elist); */
    SUtilitiesWrapper u(modeller_);
    return u.getAdaptedLengthCrossSectionCurve(surface_id, length, vlist, elist);
}

bool RulesProcessor::getExtrusionPath( size_t surface_index, std::vector<double> &path)
{
    SUtilitiesWrapper util(modeller_);

    bool status = util.getExtrusionPath(surface_index, path);

    return status;
}


bool RulesProcessor::saveFile( std::string filename )
{
    bool redo_last_surface = false;
    /* std::cout << "\n\nLast surface inserted is a test? "; */
    if ( last_surface_inserted_is_a_test_ )
    {
        /* std::cout << "yes"; */
        if ( modeller_.canUndo() )
        {
            /* std::cout << " -- undo!"; */

            /* modeller_.undo(); */
            /* modeller_.popUndoStack(); */
            modeller_.destroyLastSurface(); // equivalent to undo() + popUndoStack()

            last_surface_inserted_is_a_test_ = false;
            redo_last_surface = true;
        }
        else
        {
            /* std::cout << " -- ops, can't undo?"; */
        }
    }
    else
    {
        /* std::cout << "no."; */
    }

    bool success = modeller_.saveJSON(filename);

    if ( redo_last_surface )
    {
        /* std::cout << " What to do?"; */
        /* std::cout << " Redoing..."; */
        /* modeller_.redo(); */
    }
    /* std::cout << "\n\n\n" << std::flush; */

    return success;
}

bool RulesProcessor::loadFile( std::string filename )
{
bool status = modeller_.loadBinary(filename) || modeller_.loadJSON(filename);
/* bool status = modeller_.loadJSON(filename); */

    if ( status )
    {
        modeller_.useOpenGLCoordinateSystem();
        pa_is_active_ = modeller_.preserveAboveIsActive(lower_model_);
        pb_is_active_ = modeller_.preserveBelowIsActive(upper_model_);
        modeller_.getOrigin(origin_.x, origin_.y, origin_.z);
        modeller_.getSize(length_.x, length_.y, length_.z);
        modelling_length_discretization_ = modeller_.getLengthDiscretization();
        modelling_width_discretization_ = modeller_.getWidthDiscretization();
        // enforcePreserveRegion();
    }
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

#include <fstream>

bool RulesProcessor::getTetrahedralMesh( std::vector< std::vector<double> > &vertex_list, std::vector< std::vector<std::size_t> > &element_list )
{
    stratmod::SUtilities u(modeller_);
    std::vector<double> vertex_coordinates;
    bool success = (u.getTetrahedralMesh(vertex_coordinates, element_list) > 0);

    auto vertex_map = [](const std::vector<std::size_t>& elements) -> std::map<std::size_t, std::size_t> {
        std::map<std::size_t, std::size_t> dict;
        std::size_t j = 0;
        for (auto e : elements)
        {
            if (auto iter = dict.find(e); iter == dict.end())
            {
                dict[e] = j;
                ++j;
            }
        }

        return dict;
    };

    if (success)
    {
        vertex_list.resize(element_list.size());
        for (std::size_t i = 0; i < element_list.size(); ++i)
        {
            auto& region_faces = element_list[i];
            auto dict = vertex_map(region_faces);
            std::vector<double> vertices(dict.size()*3);

            for (auto [vid, new_vid] : dict)
            {
                vertices[3*new_vid + 0] = vertex_coordinates[3*vid + 0];
                vertices[3*new_vid + 1] = vertex_coordinates[3*vid + 1];
                vertices[3*new_vid + 2] = vertex_coordinates[3*vid + 2];

            }
            for (auto& vid : region_faces)
            {
                vid = dict[vid];
            }
            vertex_list[i] = std::move(vertices);
        }
    }

    /* if ( success ) */
    /* { */
        /* std::vector<double> volumes; */
        /* SUtilitiesWrapper u(modeller_); */
        /* u.getRegionVolumeList(volumes); */

        /* double total = 0; */
        /* for ( size_t i = 0; i < volumes.size(); ++i ) */
        /* { */
            /* total += volumes[i]; */
        /* } */

        /* std::ofstream ofs( "current_model_volumes.txt" ); */
        /* ofs << "Gross rock volumes: \n\n"; */

        /* std::cout << "\n\n\nGross rock volumes: \n\n"; */

        /* for ( int i = static_cast<int>(volumes.size()) -1; i >=0; --i ) */
        /* { */
            /* /1* std::cout.unsetf(std::ios_base::floatfield); *1/ */
            /* std::cout << "Volume(" << std::setw(3) << i << ") = " */
            /*     << std::scientific << std::setprecision(2) << std::setw(9) */
            /*     << volumes[i] << " m^3" */
            /*     << " ~ " << std::fixed << std::setprecision(1) << std::setw(4) */
            /*     << (total > 1E-6 ? 100*volumes[i]/total : 0 ) << "% of reservoir \n"; */

            /* /1* ofs.unsetf(std::ios_base::floatfield); *1/ */
            /* ofs << "Volume(" << std::setw(3) << i << ") = " */
            /*     << std::scientific << std::setprecision(2) << std::setw(9) */
            /*     << volumes[i] << " m^3" */
            /*     << " ~ " << std::fixed << std::setprecision(1) << std::setw(4) */
            /*     << (total > 1E-6 ? 100*volumes[i]/total : 0 ) << "% of reservoir \n"; */

            /* /1* ofs << "Volume(" << i << ") = " << volumes[i] << " m^3\n"; *1/ */
            /* /1* total += volumes[i]; *1/ */
        /* } */
        /* std::cout.unsetf(std::ios_base::floatfield); */
        /* std::cout << "\nTotal volume = " << std::setprecision(3) << total << " m^3\n\n\n" <<std::flush; */

        /* ofs.unsetf(std::ios_base::floatfield); */
        /* ofs << "\nTotal volume = " << std::setprecision(3) << total << " m^3\n\n\n" <<std::flush; */
    /* } */

    /* auto sids = modeller_.getOrderedSurfacesIndices(); */
    /* auto sids = getOrderedSurfaces(); */
    /* std::cout << "\n Ordered surfaces' ids in RulesProcessor: "; */
    /* for ( auto sid : sids ) */
    /* { */
        /* std::cout << sid << " "; */
    /* } */
    /* std::cout << "\n\n" << std::flush; */

    /* auto active_sids = filterActiveSurfaces(sids); */
    /* auto active_sids = getOrderedActiveSurfaces(); */
    /* std::cout << "\n Ordered active surfaces' ids in RulesProcessor: "; */
    /* for ( auto sid : active_sids ) */
    /* { */
        /* std::cout << sid << " "; */
    /* } */
    /* std::cout << "\n\n\n" << std::flush; */

    return success;
}

void RulesProcessor::getRegionVolumeList( std::vector<double>& volumes )
{
    SUtilitiesWrapper u(modeller_);
    u.getRegionVolumeList(volumes);
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
    // Zhao: function "adaptDiscretization" below controls the base
    // discretization that will be used to create the piecewise linear complex.
    // Notice that every "block" is comprised of 8 triangles.
    //
    // "adaptDiscretization" will use the suggested value "max_width_disc"
    // ("max_length_disc") to discretize the model's width (length) if the
    // model's width size is bigger than its length size (and vice-versa for
    // length).  The other dimension will be discretized with blocks as close
    // to a square as possible.
    //

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
    //

    /* diagnostics_width_discretization_ = 16; */
    /* diagnostics_length_discretization_ = 16; */

    auto adaptDiscretization = [] (
            double model_width, double model_length,
            size_t &output_width_disc, size_t &output_length_disc,
            size_t max_width_disc /*= 16*/, size_t max_length_disc /*= 16*/ ) -> bool
    {


        if ( model_width >= model_length )
        {
            // will have "max_width_disc" blocks in width
            output_width_disc = max_width_disc;
            auto block_size = model_width/static_cast<double>(output_width_disc);

            // how many blocks fit in the length dimension of the model
            auto num_blocks = model_length/block_size;

            if ( num_blocks < 1 )
            {
                output_length_disc = 1;
            }
            else
            {
                output_length_disc = std::round(num_blocks);
            }
        }
        else
        {
            // will have "max_length_disc" blocks in length
            output_length_disc = max_length_disc;
            auto block_size = model_length/static_cast<double>(output_length_disc);

            // how many blocks fit in the width dimension of the model
            auto num_blocks = model_width/block_size;

            if ( num_blocks < 1 )
            {
                output_width_disc = 1;
            }
            else
            {
                output_width_disc = std::round(num_blocks);
            }
        }

        return true;

    };

    double model_width, model_length, height;
    getLenght(model_width, height, model_length );

    // I would suggest to not reduce the values of max_width_disc and max_length_disc
    size_t max_width_disc = 16, max_length_disc = 16;

    adaptDiscretization(model_width, model_length,
            diagnostics_width_discretization_, diagnostics_length_discretization_,
            max_width_disc, max_length_disc);

    modeller_.changeDiscretization(diagnostics_width_discretization_, diagnostics_length_discretization_);


    //
    // Get the PLC
    //

    std::vector< std::size_t > surfacesIDs = getSurfaces();
    SUtilitiesWrapper u(modeller_);

    for( auto it: surfacesIDs )
    {

        /* if( objects.findElement( it ) == false ) continue; */

        TriangleMesh t;
        /* std::vector< double > surface_vertices; */
        /* std::vector< std::size_t > surface_faces; */


        /* bool has_surface = getMesh( it, surface_vertices, surface_faces ); */
        bool has_surface = true;
        has_surface &= u.getVertexList( it, t.vertex_list );
        has_surface &= u.getUniqueFacesList(it, t.face_list ) > 0;

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
    return Colorwrap::Greys(numColors);
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
        modeller_.destroyLastSurface();
        last_surface_inserted_is_a_test_ = false;
    }

    if ( testing_surface_insertion_ == true )
    {
        modeller_.disableGeologicRules();
    }

    enforcePreserveRegion();
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
            modeller_.destroyLastSurface();
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
        double fill_distance = -1; // force modeller to pick a default smoothing factor based on discretization
        return this->modeller_.createSurface(s_id, pts, fill_distance);
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
        double fill_distance = -1; // force modeller to pick a default smoothing factor based on discretization
        return this->modeller_.createLengthwiseExtrudedSurface(s_id, cross_sec_pts, fill_distance);
    };

    bool success = processSurfaceCreation(surfaceCreator, surface_id, cross_section_curve_point_data);

    return success;
}

bool RulesProcessor::createLengthwiseExtrudedSurface( size_t surface_id,
        const std::vector<double> &cross_section_curve_point_data, double cross_section_length,
        const std::vector<double> &path_curve_point_data
        )
{
    auto surfaceCreator = [this](
            size_t s_id, const std::vector<double> &cross_sec_pts,
            double cross_sec, const std::vector<double> &path_pts ) -> bool
    {
        double fill_distance = -1; // force modeller to pick a default smoothing factor based on discretization
        return this->modeller_.createLengthwiseExtrudedSurface(s_id, cross_sec_pts, cross_sec, path_pts, fill_distance);
    };

    bool success = processSurfaceCreation(surfaceCreator, surface_id, cross_section_curve_point_data,
            cross_section_length, path_curve_point_data);

    return success;
}
/* { */
/*     auto surfaceCreator = [this]( size_t s_id, const std::vector<double> &pts ) -> bool */
/*     { */
/*         const double fill_distance_factor = 1.0/(10.0*std::sqrt(2)); */
/*         auto sqr = [](double x) -> double { return x*x; }; */
/*         double fill_distance = std::sqrt(sqr(length_.x-origin_.x) + sqr(length_.z-origin_.z))*fill_distance_factor; */

/*         return this->modeller_.createSurface(s_id, pts, fill_distance); */
/*     }; */

/*     PathGuidedSurface surface; */

/*     // Set boundary */
/*     double m = 0.2; */
/*     surface.setOrigin(origin_.x - m*length_.x, origin_.z - m*length_.z); */
/*     surface.setSize(length_.x*(1.0 + 2*m), length_.z*(1.0 + 2*m)); */

/*     // Input path */
/*     size_t path_max_disc = 256; */
/*     double path_disc_per_unit_of_length = std::max(length_.x, length_.z)/static_cast<double>(path_max_disc); */
/*     surface.addGuidingPathTangentVectors(path_curve_point_data, path_disc_per_unit_of_length, path_max_disc); */
/*     surface.setGuidingPaths(); */

/*     // Compute orbits */
/*     size_t orbit_max_disc = 72; */
/*     double orbit_disc_per_unit_of_length = std::max(length_.x, length_.z)/static_cast<double>(orbit_max_disc); */
/*     double width, length = cross_section_length, height; */
/*     for (size_t i = 0; i < cross_section_curve_point_data.size()/2; ++i) */
/*     { */
/*         width = cross_section_curve_point_data[2*i + 0]; */
/*         height = cross_section_curve_point_data[2*i + 1]; */

/*         surface.addOrbitToSurfaceSamples(width, length, height, orbit_disc_per_unit_of_length, orbit_max_disc); */
/*     } */

/*     // Create final surface */
/*     /1* std::cout << "---> Creating final surface\n"; *1/ */
/*     auto surface_points = surface.getSurfaceSamples(); */
/*     bool success = processSurfaceCreation(surfaceCreator, surface_id, surface_points); */

/*     return success; */
/* } */
/* { */
/*     auto surfaceCreator = [this]( size_t s_id, const std::vector<double> &pts ) -> bool */
/*     { */
/*         const double fill_distance_factor = 1.0/(10.0*std::sqrt(2)); */
/*         auto sqr = [](double x) -> double { return x*x; }; */
/*         double fill_distance = std::sqrt(sqr(length_.x-origin_.x) + sqr(length_.z-origin_.z))*fill_distance_factor; */

/*         return this->modeller_.createSurface(s_id, pts, fill_distance); */
/*     }; */

/*     std::cout << "Trying to create path guided surface:\n"; */
/*     std::cout << "---> Setting solver properties\n"; */
/*     odeSolver2D S; */

/*     double tol_x = 0.05 * length_.x; */
/*     double tol_z = 0.05 * length_.z; */
/*     S.setDomainOrigin(origin_.x-tol_x, origin_.z-tol_z); */
/*     S.setDomainSize(length_.x+2*tol_x, length_.z+2*tol_z); */

/*     auto curve_size = path_curve_point_data.size()/2; */
/*     std::vector<double> path_w(curve_size), path_l(curve_size); */

/*     std::cout << "---> Setting solver input\n"; */
/*     auto getCurve = [] (const std::vector<double>& curve) -> odeSolver2D::Curve { */
/*         size_t num_points = curve.size()/2; */
/*         std::vector<double> path_x(num_points); */
/*         std::vector<double> path_y(num_points); */

/*         for (size_t i = 0; i < num_points; ++i ) */
/*         { */
/*             path_x[i] = curve[2*i + 0]; */
/*             path_y[i] = curve[2*i + 1]; */
/*             /1* std::cout << "::: Adding point: (" << path_w[i] << ", " << path_l[i] << ")\n"; *1/ */
/*         } */

/*         return std::make_tuple(path_x, path_y); */
/*     }; */
/*     std::tie(path_w, path_l) = getCurve(path_curve_point_data); */
/*     /1* std::tie(path_w, path_l) = odeSolver2D::sampleCurve(path_w, path_l, 32); *1/ */
/*     S.inputCurveTangentVectors(path_w, path_l, 256); */

/*     std::cout << "---> Interpolating vector field\n"; */
/*     if ( !S.interpolateVectorField() ) */
/*     { */
/*         std::cout << "---> ---> Could not interpolate vector field\n"; */
/*         return false; */
/*     } */

/*     auto get_curve_len = [](const odeSolver2D::XCoordinates &xcoords, const odeSolver2D::YCoordinates &ycoords) -> double { */
/*         double len = 0.0; */
/*         auto numel = xcoords.size(); */
/*         if ( (numel < 2) || (xcoords.size() != ycoords.size()) ) */
/*         { */
/*             return len; */
/*         } */

/*         auto dist = [](const odeSolver2D::XCoordinates &xcoords, const odeSolver2D::YCoordinates &ycoords, size_t i0, size_t i1) -> double { */
/*             double x = xcoords[i0] - xcoords[i1]; */
/*             double y = ycoords[i0] - ycoords[i1]; */
/*             double d = std::sqrt(x*x + y*y); */

/*             return d; */
/*         }; */

/*         for (size_t i = 0; i < numel-1; ++i) */
/*         { */
/*             len += dist(xcoords, ycoords, i, i+1); */
/*         } */

/*         return len; */
/*     }; */

/*     std::vector<double> surface_points, cross_w, cross_h; */
/*     double wi, li = cross_section_length, hi; */
/*     std::cout << "---> Sampling final surface: " << std::flush; */
/*     auto t0 = std::chrono::high_resolution_clock::now(); */
/*     odeSolver2D::XCoordinates orbit_w; */
/*     odeSolver2D::YCoordinates orbit_l; */
/*     std::tie(cross_w, cross_h) = getCurve(cross_section_curve_point_data); */
/*     double disc_x = 72, disc_z = 72; */
/*     double disc = std::sqrt(length_.x*length_.x + length_.z*length_.z)/std::sqrt(disc_x*disc_x + disc_z*disc_z); */
/*     double orbit_len; */
/*     size_t num_samples; */
/*     for ( size_t i = 0; i < cross_w.size(); ++i ) */
/*     { */
/*         /1* wi = cross_section_curve_point_data[2*i + 0]; *1/ */
/*         /1* hi = cross_section_curve_point_data[2*i + 1]; *1/ */
/*         wi = cross_w[i]; */
/*         hi = cross_h[i]; */

/*         /1* std::tie(orbit_w, orbit_l) = odeSolver2D::sampleCurve(S.getOrbit(wi, li), 128); *1/ */
/*         std::tie(orbit_w, orbit_l) = S.getOrbit(wi, li); */
/*         std::cout << "---> ---> iteration " << i << "; orbit points: " << orbit_l.size() << "; orbit height: " << hi << "\n"; */
/*         if (orbit_l.size() > 1) */
/*         { */
/*             orbit_len = get_curve_len(orbit_w, orbit_l); */
/*             num_samples = std::ceil(orbit_len/disc); */
/*             std::tie(orbit_w, orbit_l) = odeSolver2D::sampleCurve(orbit_w, orbit_l, num_samples); */
/*             std::cout << "---> ---> iteration " << i << "; sampled points: " << orbit_l.size() << "; orbit height: " << hi << "\n"; */
/*             for (size_t j = 0; j < orbit_l.size(); ++j) */
/*             { */
/*                 surface_points.push_back(orbit_w[j]); */
/*                 surface_points.push_back(hi); */
/*                 surface_points.push_back(orbit_l[j]); */
/*             } */
/*         } */
/*     } */
/*     auto t1 = std::chrono::high_resolution_clock::now(); */
/*     auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count(); */
/*     std::cout << dt << " milliseconds, for " << surface_points.size()/3 << "points\n" << std::flush; */

/*     std::cout << "---> Creating final surface\n"; */
/*     bool success = processSurfaceCreation(surfaceCreator, surface_id, surface_points); */

/*     return success; */
/* } */



bool RulesProcessor::createWidthwiseExtrudedSurface( size_t surface_id,
        const std::vector<double> &cross_section_curve_point_data
        )
{
    auto surfaceCreator = [this]( size_t s_id, const std::vector<double> &cross_sec_pts ) -> bool
    {
        double fill_distance = -1; // force modeller to pick a default smoothing factor based on discretization
        return this->modeller_.createWidthwiseExtrudedSurface(s_id, cross_sec_pts, fill_distance);
    };

    bool success = processSurfaceCreation(surfaceCreator, surface_id, cross_section_curve_point_data);

    return success;
}

bool RulesProcessor::createWidthwiseExtrudedSurface( size_t surface_id,
        const std::vector<double> &cross_section_curve_point_data, double cross_section_width,
        const std::vector<double> &path_curve_point_data
        )
{
    auto surfaceCreator = [this](
            size_t s_id, const std::vector<double> &cross_sec_pts,
            double cross_sec, const std::vector<double> &path_pts ) -> bool
    {
        double fill_distance = -1; // force modeller to pick a default smoothing factor based on discretization
        return this->modeller_.createWidthwiseExtrudedSurface(s_id, cross_sec_pts, cross_sec, path_pts, fill_distance);
    };

    bool success = processSurfaceCreation(surfaceCreator, surface_id, cross_section_curve_point_data,
            cross_section_width, path_curve_point_data);

    return success;
}
/* { */
/*     auto surfaceCreator = [this]( size_t s_id, const std::vector<double> &pts ) -> bool */
/*     { */
/*         const double fill_distance_factor = 1.0/(10.0*std::sqrt(2)); */
/*         auto sqr = [](double x) -> double { return x*x; }; */
/*         double fill_distance = std::sqrt(sqr(length_.x-origin_.x) + sqr(length_.z-origin_.z))*fill_distance_factor; */

/*         return this->modeller_.createSurface(s_id, pts, fill_distance); */
/*     }; */

/*     PathGuidedSurface surface; */

/*     // Set boundary */
/*     double m = 0.2; */
/*     surface.setOrigin(origin_.x - m*length_.x, origin_.z - m*length_.z); */
/*     surface.setSize(length_.x*(1.0 + 2*m), length_.z*(1.0 + 2*m)); */

/*     // Input path */
/*     size_t path_max_disc = 256; */
/*     double path_disc_per_unit_of_length = std::max(length_.x, length_.z)/static_cast<double>(path_max_disc); */
/*     std::vector<double> path_curve(path_curve_point_data.size()); */
/*     for (size_t i = 0; i < cross_section_curve_point_data.size()/2; ++i) */
/*     { */
/*         path_curve[2*i + 0] = path_curve_point_data[2*i + 1]; */
/*         path_curve[2*i + 1] = path_curve_point_data[2*i + 0]; */
/*     } */
/*     surface.addGuidingPathTangentVectors(path_curve, path_disc_per_unit_of_length, path_max_disc); */
/*     surface.setGuidingPaths(); */

/*     // Compute orbits */
/*     size_t orbit_max_disc = 72; */
/*     double orbit_disc_per_unit_of_length = std::max(length_.x, length_.z)/static_cast<double>(orbit_max_disc); */
/*     double width = cross_section_width, length, height; */
/*     for (size_t i = 0; i < cross_section_curve_point_data.size()/2; ++i) */
/*     { */
/*         length = cross_section_curve_point_data[2*i + 0]; */
/*         height = cross_section_curve_point_data[2*i + 1]; */

/*         surface.addOrbitToSurfaceSamples(width, length, height, orbit_disc_per_unit_of_length, orbit_max_disc); */
/*     } */

/*     // Create final surface */
/*     /1* std::cout << "---> Creating final surface\n"; *1/ */
/*     auto surface_points = surface.getSurfaceSamples(); */
/*     bool success = processSurfaceCreation(surfaceCreator, surface_id, surface_points); */

/*     return success; */
/* } */
/* { */
    /* auto surfaceCreator = [this]( size_t s_id, const std::vector<double> &pts ) -> bool */
    /* { */
    /*     double fill_distance = 50.0; // force modeller to pick a default smoothing factor based on discretization */
    /*     return this->modeller_.createSurface(s_id, pts, fill_distance); */
    /* }; */

    /* std::cout << "Trying to create path guided surface:\n"; */
    /* std::cout << "---> Setting solver properties\n"; */
    /* odeSolver2D S; */
    /* S.setDomainOrigin(origin_.x, origin_.z); */
    /* S.setDomainSize(length_.x, length_.z); */

    /* auto curve_size = path_curve_point_data.size()/2; */
    /* std::vector<double> path_w(curve_size), path_l(curve_size); */

    /* std::cout << "---> Setting solver input\n"; */
    /* for (size_t i = 0; i < curve_size; ++i ) */
    /* { */
    /*     path_w[i] = path_curve_point_data[2*i + 0]; */
    /*     path_l[i] = path_curve_point_data[2*i + 1]; */
    /*     std::cout << "::: Adding point: (" << path_w.back() << ", " << path_l.back() << ")\n"; */
    /* } */
    /* S.inputCurveTangentVectors(path_l, path_w, -1); */

    /* std::cout << "---> Interpolating vector field\n"; */
    /* S.interpolateVectorField(); */

    /* std::vector<double> surface_points; */
    /* double wi = cross_section_depth, li, hi; */
    /* std::cout << "---> Sampling final surface\n"; */
    /* for ( size_t i = 0; i < cross_section_curve_point_data.size()/2; ++i ) */
    /* { */
    /*     li = cross_section_curve_point_data[2*i + 0]; */
    /*     hi = cross_section_curve_point_data[2*i + 1]; */

    /*     auto [orbit_w, orbit_l] = odeSolver2D::sampleCurve(S.getOrbit(wi, li), 64); */
    /*     std::cout << "---> ---> iteration " << i << "; sampled points: " << orbit_l.size() << "\n"; */
    /*     for (size_t j = 0; j < orbit_l.size(); ++j) */
    /*     { */
    /*         surface_points.push_back(orbit_w[j]); */
    /*         surface_points.push_back(hi); */
    /*         surface_points.push_back(orbit_l[j]); */
    /*     } */
    /* } */

    /* std::cout << "---> Creating final surface\n"; */
    /* bool success = processSurfaceCreation(surfaceCreator, surface_id, surface_points); */

    /* return success; */
/* } */

void RulesProcessor::enforcePreserveRegion()
{
    /* std::cout << "\nenforcePreserveRegion(): "; */
    if ( pa_is_active_ )
    {
        /* std::cout << "\n     --> pa_is_active_ -- lower_model_.size() == " << lower_model_.size(); */
        preserveAbove(lower_model_);
    }

    if ( pb_is_active_ )
    {
        /* std::cout << "\n     --> pb_is_active_ -- upper_model_.size() == " << upper_model_.size(); */
        preserveBelow(upper_model_);
    }

    std::cout << "\n\n" << std::flush;
}

std::pair<RulesProcessor::Att2RegMap, RulesProcessor::Reg2AttMap> RulesProcessor::getI2VRegionMaps()
{
    Att2RegMap att2reg;
    Reg2AttMap reg2att;

    /* std::map<std::size_t, std::map<int, int>> i2v_region_map; */
    SUtilities u(modeller_);
    auto i2v_region_map = u.getI2VRegionMap();
    std::size_t max_num_regions = 0;
    auto sids = modeller_.getSurfacesIndices();
    if (!sids.empty())
    {
        max_num_regions = sids.size() -1;
    }

    reg2att.resize(max_num_regions);
    for (std::size_t i = 0; i < reg2att.size(); ++i)
    {
        reg2att[i] = static_cast<int>(i);

        int votes = 0;
        for (auto& [att, rmap] : i2v_region_map)
        {
            for (auto& [rid, counter] : rmap)
            {
                if ((rid == static_cast<int>(i)) && (counter > votes))
                {
                    reg2att[i] = att;
                }
            }
        }
    }

    att2reg.resize(i2v_region_map.size(), -1);
    for (auto& [att, rmap] : i2v_region_map)
    {
        int votes = 0;
        for(auto iter = rmap.begin(); iter != rmap.end(); ++iter)
        {
            auto& [rid, counter] = *rmap.begin();
            if (counter > votes)
            {
                att2reg[att] = rid;
                votes = counter;
            }
        }
    }

    return std::make_pair(att2reg, reg2att);
}

//{} // namespace RRM
