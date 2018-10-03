/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 3 of the License, or (at your option) any later version.           */
/*                                                                            */
/* PlanInLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/* Lesser General Public License for more details.                            */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public           */
/* License along with PlanInLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,        */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                  */
/*                                                                            */
/******************************************************************************/


#include "planar_surface.hpp" 
#include <cmath>
#include <stdexcept> 
/* #include <iostream> */ 
/* #include <iomanip> */

unsigned long int PlanarSurface::num_instances_ = 1; 
unsigned long int PlanarSurface::global_discretization_state_ = 1; 

PlanarSurface::Natural PlanarSurface::discretization_X = 32; 
PlanarSurface::Natural PlanarSurface::discretization_Y = 32; 

Point3 PlanarSurface::origin = {{{ 0.0, 0.0, 0.0, 0.0 }}}; 
Point3 PlanarSurface::lenght = {{{ 1.0, 1.0, 1.0, 1.0 }}}; 

auto getCoordinate( PlanarSurface::Coordinate c ) -> std::underlying_type<PlanarSurface::Coordinate>::type 
{ 
    return static_cast< std::underlying_type<PlanarSurface::Coordinate>::type >(c); 
}

std::array<PlanarSurface::Natural, 3> PlanarSurface::coordinates_map_ = {{ 
    getCoordinate(PlanarSurface::Coordinate::WIDTH), 
        getCoordinate(PlanarSurface::Coordinate::HEIGHT), 
        getCoordinate(PlanarSurface::Coordinate::DEPTH) 
}}; 

void PlanarSurface::updateDiscretization() 
{
    if ( this_discretization_state_ == global_discretization_state_ ) { 
        return; 
    }

    nX_ = 2*discretization_X + 1; 
    nY_ = 2*discretization_Y + 1; 
    num_vertices_ = nX_ * nY_; 
    tolerance = 0.00002 * std::max( 1.0/static_cast<double>(nX_), 1.0/static_cast<double>(nY_) ); 
    /* std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(8) << "\n --> tolerance: " << tolerance << std::endl; */ 

    /* if ( this_discretization_state_ < global_discretization_state_ ) */ 
    if ( surfaceIsSet() == true ) 
    { 
        updateRawCache(); 
    }

    this_discretization_state_ = global_discretization_state_; 
}

bool PlanarSurface::requestChangeDiscretization( const Natural num_X, const Natural num_Y ) 
{
    bool has_changed = false; 

    if ( ( num_X > 0 ) && ( num_X != discretization_X ) ) { 
        discretization_X = num_X; 
        has_changed |= true; 
    }

    if ( ( num_Y > 0 ) && ( num_Y != discretization_Y ) ) { 
        discretization_Y = num_Y; 
        has_changed |= true; 
    }

    if ( has_changed ) { 
        ++global_discretization_state_; 
    }

    return has_changed; 
}

bool PlanarSurface::setOutputCoordinatesOrdering( 
        PlanarSurface::Coordinate first, 
        PlanarSurface::Coordinate second, 
        PlanarSurface::Coordinate third 
        ) 
{
    if ( (first == second) || (first == third) || (second == third) ) { 
        return false; 
    }

    coordinates_map_[0] = getCoordinate(first); 
    coordinates_map_[1] = getCoordinate(second); 
    coordinates_map_[2] = getCoordinate(third); 

    return true; 
}

PlanarSurface::Natural PlanarSurface::getDiscretizationX()
{
    return discretization_X;
}

PlanarSurface::Natural PlanarSurface::getDiscretizationY()
{
    return discretization_Y;
}

PlanarSurface::PlanarSurface( bool extruded_surface, bool orthogonally_oriented ) : id_(num_instances_) { 
    /* std::cout << "My id: " << id_ << std::endl; */ 
    ++num_instances_; 

    this_discretization_state_ = 0; 
    updateDiscretization(); 

    /* If Visual Studio 2013 complains with the following line yielding "error C2783" be advised that IT IS A BUG IN VS2013's COMPILER. */ 
    /* The "Visual Studio 2013 update 5" for VS2013 is known to have a fix to the problem. */ 
    f = std::make_shared<InterpolatedGraph>( extruded_surface, orthogonally_oriented );
    extruded_surface_ = extruded_surface; 

    /* If updating VS2013 is not possible, change the "make_shared" call to an explicit call to operator "new". */ 
    /* f = std::shared_ptr<InterpolatedGraph>( new InterpolatedGraph() ); */ 


    /* origin.x = 0.0; */ 
    /* origin.y = 0.0; */ 
    /* origin.z = 0.0; */ 

    /* lenght.x = 1.0; */ 
    /* lenght.y = 1.0; */ 
    /* lenght.z = 1.0; */ 

    dependency_list_.insert(id_); 
}

PlanarSurface::PlanarSurface( const PlanarSurface &rhs ) : id_(num_instances_) 
{
    /* std::cout << "My id (copy): " << id_ << std::endl; */ 
    ++num_instances_; 

    this_discretization_state_ = rhs.this_discretization_state_; 
    nX_ = rhs.nX_; 
    nY_ = rhs.nY_; 
    num_vertices_ = rhs.num_vertices_; 
    tolerance = rhs.tolerance; 

    f = std::make_shared<InterpolatedGraph>(*rhs.f); 

    /* origin = rhs.origin; */ 
    /* lenght = rhs.lenght; */ 

    heights = rhs.heights; 
    normals = rhs.normals;
    interpolant_is_set_ = rhs.interpolant_is_set_; 
    mesh_is_set_ = rhs.mesh_is_set_; 

    upper_bound_ = rhs.upper_bound_; 
    lower_bound_ = rhs.lower_bound_; 

    dependency_list_ = rhs.dependency_list_; 
    auto iter = dependency_list_.find(rhs.id_); 
    dependency_list_.erase(iter); 
    dependency_list_.insert(id_); 

    extruded_surface_ = rhs.extruded_surface_; 

    cached_heights_ = rhs.cached_heights_;
    cached_valid_heights_ = rhs.cached_valid_heights_;
    cache_is_fresh_ = rhs.cache_is_fresh_;

    /* unprocessed_upper_bound_ = rhs.unprocessed_upper_bound_; */
    /* unprocessed_lower_bound_ = rhs.unprocessed_lower_bound_; */

    /* std::cout << "My dependency_list_: "; */ 
    /* for ( auto i : dependency_list_ ) { */ 
    /*     std::cout << i << " "; */ 
    /* } */
    /* std::cout << "\n"; */ 
}

PlanarSurface::PlanarSurface( PlanarSurface &&rhs ) : id_( rhs.id_ ) 
{
    f = std::make_shared<InterpolatedGraph>( std::move(*rhs.f) ); 

    this_discretization_state_ = rhs.this_discretization_state_; 
    nX_ = rhs.nX_; 
    nY_ = rhs.nY_; 
    num_vertices_ = rhs.num_vertices_; 
    tolerance = rhs.tolerance; 

    /* origin = std::move(rhs.origin); */ 
    /* lenght = std::move(rhs.lenght); */ 

    heights = std::move(rhs.heights); 
    normals = std::move(rhs.normals);
    interpolant_is_set_ = rhs.interpolant_is_set_; 
    mesh_is_set_ = rhs.mesh_is_set_; 

    upper_bound_ = std::move(rhs.upper_bound_); 
    lower_bound_ = std::move(rhs.lower_bound_); 
    dependency_list_ = std::move(rhs.dependency_list_); 

    extruded_surface_ = rhs.extruded_surface_; 

    cached_heights_ = std::move(rhs.cached_heights_);
    cached_valid_heights_ = std::move(rhs.cached_valid_heights_);
    cache_is_fresh_ = rhs.cache_is_fresh_;

    /* unprocessed_upper_bound_ = std::move(rhs.unprocessed_upper_bound_); */
    /* unprocessed_lower_bound_ = std::move(rhs.unprocessed_lower_bound_); */

}

void PlanarSurface::setOrigin( const Point3 &o ) 
{
    origin.x = o.x; 
    origin.y = o.y; 
    origin.z = o.z; 
} 

void PlanarSurface::setOrigin( Point3 &&o ) 
{
    setOrigin(o); 
} 

bool PlanarSurface::setLenght( const Point3 &l ) 
{ 
    if ( l.x <= 0 ) { 
        return false; 
    }

    if ( l.y <= 0 ) { 
        return false; 
    }

    if ( l.z <= 0 ) { 
        return false; 
    }

    lenght.x = l.x; 
    lenght.y = l.y; 
    lenght.z = l.z; 

    return true; 
}

bool PlanarSurface::setLenght( Point3 &&l ) 
{
    return setLenght(l); 
}

Point3 PlanarSurface::getOrigin() 
{
    return origin; 
}

Point3 PlanarSurface::getLenght() 
{
    return lenght; 
}

bool PlanarSurface::rangeCheck( Natural vertex ) { 
    return ( vertex < num_vertices_ ); 
}

bool PlanarSurface::weakEntireSurfaceCheck() 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    bool isEntireSurface = true; 
    bool status; 

    updateDiscretization(); 

    double height{}; 
    double lb = origin.z;  
    double ub = origin.z + lenght.z;  

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for firstprivate(num_vertices_omp, lb, ub, height) private(status) default(none) reduction(&&: isEntireSurface) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        /* status = getCachedHeight(i, height); */ 
        status = getHeight(i, height); 
        if ( status == false )
        { 
            if ( std::fabs(lb - height) <= tolerance ) {
                status |= true; 
            }

            else if ( std::fabs(height - ub) <= tolerance ) {
                status |= true; 
            }
        }

        isEntireSurface = isEntireSurface && status; 
    }

    return isEntireSurface; 
}

bool PlanarSurface::weakBoundedEntireSurfaceCheck( PlanarSurface::Ptr &lower_surface, PlanarSurface::Ptr &upper_surface ) 
{
    std::vector<PlanarSurface::Ptr> lsurfaces, usurfaces;
    lsurfaces.push_back(lower_surface);
    usurfaces.push_back(upper_surface);

    return weakBoundedEntireSurfaceCheck(lsurfaces, usurfaces);
}
/* { */ 
/*     if ( surfaceIsSet() == false ) { */ 
/*         return false; */ 
/*     } */

/*     bool has_lower_boundary = false; */ 
/*     bool has_upper_boundary = false; */ 

/*     if ( ( lower_surface != nullptr ) && lower_surface->surfaceIsSet() ) { */ 
/*         has_lower_boundary = true; */ 
/*         /1* std::cout << "Lower boundary got here!\n"; *1/ */ 
/*     } */

/*     if ( ( upper_surface != nullptr ) && upper_surface->surfaceIsSet() ) { */ 
/*         has_upper_boundary = true; */ 
/*         /1* std::cout << "Upper boundary got here!\n"; *1/ */ 
/*     } */

/*     bool isEntireSurface = true; */ 
/*     bool status, lstatus, ustatus; */ 

/*     updateDiscretization(); */ 
/*     double height{}, lheight{}, uheight{}; */

/*     double lb = origin.z; */  
/*     double ub = origin.z + lenght.z; */  

/*     auto num_vertices_omp = num_vertices_; */ 

    /* /1* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*1/ */ 
/*     #pragma omp parallel for shared(lower_surface, upper_surface) firstprivate(ub, lb, num_vertices_omp, has_lower_boundary, has_upper_boundary, height, lheight, uheight) private(status, lstatus, ustatus) default(none) reduction(&&: isEntireSurface) */ 
/*     for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) */ 
/*     { */
/*         /1* status = getCachedHeight(i, height); *1/ */ 
/*         status = getHeight(i, height); */ 

/*         if ( status == false ) */
/*         { */ 
/*             if ( std::fabs(lb - height) <= tolerance ) { */
/*                 status |= true; */ 
/*             } */

/*             else if ( std::fabs(height - ub) <= tolerance ) { */ 
/*                 status |= true; */ 
/*             } */
/*         } */

/*         if ( status == false ) */ 
/*         { */ 
/*             if ( has_lower_boundary ) */ 
/*             { */ 
/*                 /1* lstatus = lower_surface->getCachedHeight(i, lheight); *1/ */ 
/*                 lstatus = lower_surface->getHeight(i, lheight); */ 
/*                 /1* std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(8) << "lstatus value: " << lstatus; *1/ */ 
/*                 if ( lstatus && ( std::fabs(lheight - height) <= tolerance ) ) { */
/*                     status |= true; */ 
/*                 } */
/*                 else { */ 
/*                     /1* std::cout << " missed index: " << i << ", distance: " << height - lheight << ", tolerance: " << tolerance << std::endl; *1/ */ 
/*                 } */
/*             } */
/*             if ( has_upper_boundary ) */ 
/*             { */
/*                 /1* ustatus = upper_surface->getCachedHeight(i, uheight); *1/ */ 
/*                 ustatus = upper_surface->getHeight(i, uheight); */ 
/*                 if ( ustatus && ( std::fabs(height - uheight) <= tolerance ) ) { */ 
/*                     status |= true; */ 
/*                 } */
/*             } */
/*         } */

/*         isEntireSurface = isEntireSurface && status; */ 
/*     } */

/*     return isEntireSurface; */ 
/* } */

bool PlanarSurface::weakBoundedEntireSurfaceCheck( PlanarSurface::WeakPtr lower_surface, PlanarSurface::WeakPtr upper_surface ) 
{ 
    PlanarSurface::Ptr lptr = lower_surface.lock(); 
    PlanarSurface::Ptr uptr = upper_surface.lock();  

    return weakBoundedEntireSurfaceCheck(lptr, uptr); 
}

// The following has a conceptual error in which one can't use the vertices
// compare whether a weak intersection happened or not, the triangles must be
// used instead
/* bool PlanarSurface::weakIntersectionCheck( PlanarSurface::Ptr &sp ) */ 
/* { */
/*     if ( surfaceIsSet() == false ) { */ 
/*         return false; */ 
/*     } */

/*     if ( ( sp == nullptr ) || ( sp->surfaceIsSet() == false ) ) { */ 
/*         return false; */ 
/*     } */

/*     bool lies_above = false; */ 
/*     bool lies_below = false; */ 

/*     updateDiscretization(); */ 
/*     double height, s_height; */ 

/*     auto num_vertices_omp = num_vertices_; */ 

/* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
/*     #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp) private(height, s_height) default(none) reduction(||: lies_above, lies_below) */ 
/*     for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) */ 
/*     { */
/*         if ( getHeight(i, height) && sp->getHeight(i, s_height) ) { */  
/*             lies_above = lies_above || (height >= s_height); */ 
/*             lies_below = lies_below || (height <= s_height); */ 
/*         } */
/*     } */

/*     return lies_above && lies_below; */ 
/* } */

bool PlanarSurface::weakBoundedEntireSurfaceCheck( std::vector<PlanarSurface::Ptr> &lower_surfaces, std::vector<PlanarSurface::Ptr> &upper_surfaces )
{ 
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    bool has_lower_boundary = false; 
    bool has_upper_boundary = false; 

    for ( auto &lower_surface : lower_surfaces )
        if ( ( lower_surface != nullptr ) && lower_surface->surfaceIsSet() ) { 
            has_lower_boundary = true; 
            /* std::cout << "Lower boundary got here!\n"; */ 
        }

    for ( auto &upper_surface : upper_surfaces )
        if ( ( upper_surface != nullptr ) && upper_surface->surfaceIsSet() ) { 
            has_upper_boundary = true; 
            /* std::cout << "Upper boundary got here!\n"; */ 
        }

    bool isEntireSurface = true; 
    bool status, lstatus, ustatus; 

    updateDiscretization(); 
    double height, lheight, uheight, inf_ubound, sup_lbound; 

    double lb = origin.z;  
    double ub = origin.z + lenght.z;  

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(lower_surfaces, upper_surfaces) firstprivate(ub, lb, num_vertices_omp, has_lower_boundary, has_upper_boundary) private(status, lstatus, ustatus, height, lheight, uheight, inf_ubound, sup_lbound) default(none) reduction(&&: isEntireSurface) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        status = getHeight(i, height);
        inf_ubound = ub;
        sup_lbound = lb;
        lstatus = false;
        ustatus = false;

        if ( status == false )
        { 
            if ( std::fabs(lb - height) <= tolerance ) {
                status |= true; 
            }

            else if ( std::fabs(height - ub) <= tolerance ) { 
                status |= true; 
            }
        }

        // Intentional BUG: this code allows truncated boundaries to modify surfaces 
        if ( status == false ) 
        { 
            if ( has_lower_boundary ) 
            { 
                for (auto &lower_surface : lower_surfaces)
                {
                    lstatus |= lower_surface->getHeight(i, lheight);
                    if (sup_lbound < lheight)
                        sup_lbound = lheight;
                    /* std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(8) << "lstatus value: " << lstatus; */
                }

                lstatus = true;
                if ( lstatus && ( std::fabs(sup_lbound - height) <= tolerance ) ) 
                {
                    status |= true; 
                }
                else 
                { 
                    /* std::cout << " missed index: " << i << ", distance: " << height - lheight << ", tolerance: " << tolerance << std::endl; */ 
                }
            }

            if ( has_upper_boundary ) 
            {
                for (auto &upper_surface : upper_surfaces)
                {
                    ustatus |= upper_surface->getHeight(i, uheight);
                    if (inf_ubound > uheight)
                        inf_ubound = uheight;
                }

                ustatus = true;
                if ( ustatus && ( std::fabs(height - inf_ubound) <= tolerance ) ) 
                { 
                    status |= true; 
                }
            }
        }

        isEntireSurface = isEntireSurface && status; 
    }

    return isEntireSurface; 
}

bool PlanarSurface::weakBoundedEntireSurfaceCheck( std::vector<PlanarSurface::WeakPtr> &wptr_lower_surfaces, std::vector<PlanarSurface::WeakPtr> &wptr_upper_surfaces )
{
    std::vector<PlanarSurface::Ptr> sptr_lower_surfaces, sptr_upper_surfaces;

    for ( auto wptr : wptr_lower_surfaces )
    {
        if ( auto sptr = wptr.lock() )
        {
            sptr_lower_surfaces.push_back(sptr);
        }
    }

    for ( auto wptr : wptr_upper_surfaces )
    {
        if ( auto sptr = wptr.lock() )
        {
            sptr_upper_surfaces.push_back(sptr);
        }
    }

    return weakBoundedEntireSurfaceCheck(sptr_lower_surfaces, sptr_upper_surfaces);
}

bool PlanarSurface::weakBoundedEntireSurfaceCheck( std::vector<PlanarSurface::WeakPtr> &&wptr_lower_surfaces, std::vector<PlanarSurface::WeakPtr> &&wptr_upper_surfaces )
{
    return weakBoundedEntireSurfaceCheck(wptr_lower_surfaces, wptr_upper_surfaces);
}

bool PlanarSurface::weakIntersectionCheck( PlanarSurface::Ptr &sp ) 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    if ( ( sp == nullptr ) || ( sp->surfaceIsSet() == false ) ) { 
        return false; 
    }

    bool lies_above = false; 
    bool lies_below = false; 

    updateDiscretization(); 
    TriangleHeights theights, sp_theights;

    size_t num_blocks = discretization_X*discretization_Y;

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_blocks) private(theights, sp_theights) default(none) reduction(||: lies_above, lies_below) 
    for ( long int b = 0; b < static_cast<long int>(num_blocks); ++b ) 
    {
        for ( size_t tpos = 0; tpos < 8; ++tpos)
        {
            /* std::cout << "Processing triangle " << tpos << " in block " << b << ": "; */
            theights = getTriangleHeightsFromPositionInBlock(tpos, b);
            sp_theights = sp->getTriangleHeightsFromPositionInBlock(tpos, b);

            if ( theights.isValid() && sp_theights.isValid() )
            {
                lies_above = lies_above || (theights >= sp_theights);
                /* if ( theights >= sp_theights ) */
                /* std::cout << "t lies above sp_t"; */

                lies_below = lies_below || (theights <= sp_theights);
                /* if ( theights <= sp_theights ) */
                /* std::cout << "t lies below sp_t"; */
            }
            /* else */
            /* { */
            /* std::cout << "at least one triangle was not valid"; */
            /* } */

            /* std::cout << std::endl << std::flush; */

            /* if ( getHeight(b, height) && sp->getHeight(b, s_height) ) { */  
            /*     lies_above = lies_above || (height >= s_height); */ 
            /*     lies_below = lies_below || (height <= s_height); */ 
            /* } */
        }
    }

    return lies_above && lies_below; 
}
bool PlanarSurface::weakIntersectionCheck( PlanarSurface::WeakPtr s ) 
{
    PlanarSurface::Ptr sp = s.lock(); 

    return weakIntersectionCheck(sp); 
}

bool PlanarSurface::weakCompleteIntersectionCheck( PlanarSurface::Ptr &sp ) 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    if ( ( sp == nullptr ) || ( sp->surfaceIsSet() == false ) ) { 
        return false; 
    }

    bool lies_above = false; 
    bool lies_below = false; 

    updateDiscretization(); 
    double height{}, s_height{}; 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp, height, s_height) default(none) reduction(||: lies_above, lies_below) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        /* getCachedHeight(i, height); */ 
        getHeight(i, height); 
        /* sp->getCachedHeight(i, s_height); */ 
        sp->getHeight(i, s_height); 

        lies_above = lies_above || (height >= s_height); 
        lies_below = lies_below || (height <= s_height); 
    }

    return lies_above && lies_below; 
}

bool PlanarSurface::weakCompleteIntersectionCheck( PlanarSurface::WeakPtr s ) 
{
    PlanarSurface::Ptr sp = s.lock(); 

    return weakCompleteIntersectionCheck(sp); 
}

bool PlanarSurface::weakLiesAboveCheck( PlanarSurface::Ptr &sp ) 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    if ( ( sp == nullptr ) || ( sp->surfaceIsSet() == false ) ) { 
        return false; 
    }

    bool lies_above = true; 

    updateDiscretization(); 
    double height{}, s_height{}; 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp, height, s_height) default(none) reduction(&&: lies_above) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        /* getCachedHeight(i, height); */ 
        getHeight(i, height); 
        /* sp->getCachedHeight(i, s_height); */ 
        sp->getHeight(i, s_height); 

        lies_above = lies_above && (height > s_height); 
    }

    return lies_above; 
}

bool PlanarSurface::weakLiesAboveCheck( PlanarSurface::WeakPtr s ) 
{
    PlanarSurface::Ptr sp = s.lock(); 

    return weakLiesAboveCheck(sp); 
}

bool PlanarSurface::weakLiesBelowCheck( PlanarSurface::Ptr &sp ) 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    if ( ( sp == nullptr ) || ( sp->surfaceIsSet() == false ) ) { 
        return false; 
    }

    bool lies_below = true; 

    updateDiscretization(); 
    double height{}, s_height{}; 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp, height, s_height) default(none) reduction(&&: lies_below) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        /* getCachedHeight(i, height); */ 
        getHeight(i, height); 
        /* sp->getCachedHeight(i, s_height); */ 
        sp->getHeight(i, s_height); 

        lies_below = lies_below && (height < s_height); 
    }

    return lies_below; 
}

bool PlanarSurface::weakLiesBelowCheck( PlanarSurface::WeakPtr s ) 
{
    PlanarSurface::Ptr sp = s.lock(); 

    return weakLiesBelowCheck(sp); 
}

bool PlanarSurface::weakLiesAboveOrEqualsCheck( PlanarSurface::Ptr &sp ) 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    if ( ( sp == nullptr ) || ( sp->surfaceIsSet() == false ) ) { 
        return false; 
    }

    bool lies_above = true; 

    updateDiscretization(); 
    double height{}, s_height{}; 
    double tolerance = getTolerance(); 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp, tolerance, height, s_height) default(none) reduction(&&: lies_above) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        /* getCachedHeight(i, height); */ 
        getHeight(i, height); 
        /* sp->getCachedHeight(i, s_height); */ 
        sp->getHeight(i, s_height); 

        lies_above = lies_above && (height > s_height - tolerance); 
    }

    return lies_above; 
}

bool PlanarSurface::weakLiesAboveOrEqualsCheck( PlanarSurface::WeakPtr s ) 
{
    PlanarSurface::Ptr sp = s.lock(); 

    return weakLiesAboveOrEqualsCheck(sp); 
}

bool PlanarSurface::weakLiesAboveOrEqualsCheck( std::vector<PlanarSurface::Ptr> &surfaces )
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    bool has_bounding_surfaces = false;

    for ( auto &sptr : surfaces )
        if ( ( sptr != nullptr ) && ( sptr->surfaceIsSet() ) ) { 
            has_bounding_surfaces = true;
        }

    if ( has_bounding_surfaces == false )
    {
        return false;
    }

    bool lies_above = true; 

    updateDiscretization(); 
    double height, tmp_height, s_height, lb_height = origin.z; 
    double tolerance = getTolerance(); 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(surfaces) firstprivate(num_vertices_omp, tolerance, lb_height) private(height, tmp_height, s_height ) default(none) reduction(&&: lies_above) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        getHeight(i, height); 

        s_height = lb_height;
        for ( auto &sptr : surfaces )
        {
            sptr->getHeight(i, tmp_height); 
            if ( tmp_height > s_height )
                s_height = tmp_height;
        }

        lies_above = lies_above && (height > s_height - tolerance); 
    }

    return lies_above; 
}

bool PlanarSurface::weakLiesAboveOrEqualsCheck( std::vector<PlanarSurface::WeakPtr> &wptr_surfaces )
{
    std::vector<PlanarSurface::Ptr> sptr_surfaces;
    for ( auto &wptr : wptr_surfaces )
    {
        if ( auto sptr = wptr.lock() )
            sptr_surfaces.push_back( sptr );
    }

    return weakLiesAboveOrEqualsCheck(sptr_surfaces);
} 

bool PlanarSurface::weakLiesBelowOrEqualsCheck( PlanarSurface::Ptr &sp ) 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    if ( ( sp == nullptr ) || ( sp->surfaceIsSet() == false ) ) { 
        return false; 
    }

    bool lies_below = true; 

    updateDiscretization(); 
    double height{}, s_height{}; 
    double tolerance = getTolerance(); 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp, tolerance, height, s_height) default(none) reduction(&&: lies_below) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        /* getCachedHeight(i, height); */ 
        getHeight(i, height); 
        /* sp->getCachedHeight(i, s_height); */ 
        sp->getHeight(i, s_height); 

        lies_below = lies_below && (height < s_height + tolerance ); 
    }

    return lies_below; 
}

bool PlanarSurface::weakLiesBelowOrEqualsCheck( PlanarSurface::WeakPtr s ) 
{
    PlanarSurface::Ptr sp = s.lock(); 

    return weakLiesBelowOrEqualsCheck(sp); 
}

bool PlanarSurface::weakLiesBelowOrEqualsCheck( std::vector<PlanarSurface::Ptr> &surfaces )
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    bool has_bounding_surfaces = false;

    for ( auto &sptr : surfaces )
        if ( ( sptr != nullptr ) && ( sptr->surfaceIsSet() ) ) { 
            has_bounding_surfaces = true;
        }

    if ( has_bounding_surfaces == false )
    {
        return false;
    }

    bool lies_below = true; 

    updateDiscretization(); 
    double height, tmp_height, s_height, ub_height = origin.z + lenght.z; 
    double tolerance = getTolerance(); 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(surfaces) firstprivate(num_vertices_omp, tolerance, ub_height) private(height, tmp_height, s_height) default(none) reduction(&&: lies_below) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        getHeight(i, height); 

        s_height = ub_height;
        for ( auto &sptr : surfaces )
        {
            sptr->getHeight(i, tmp_height); 
            if ( tmp_height < s_height )
                s_height = tmp_height;
        }

        lies_below = lies_below && (height < s_height + tolerance ); 
    }

    return lies_below; 
}

bool PlanarSurface::weakLiesBelowOrEqualsCheck( std::vector<PlanarSurface::WeakPtr> &wptr_surfaces )
{
    std::vector<PlanarSurface::Ptr> sptr_surfaces;
    for ( auto &wptr : wptr_surfaces )
    {
        if ( auto sptr = wptr.lock() )
            sptr_surfaces.push_back( sptr );
    }

    return weakLiesBelowOrEqualsCheck(sptr_surfaces);
} 

bool PlanarSurface::getVertex2D( Natural index, Point2 &v ) { 
    if ( rangeCheck(index) == false ) { 
        return false; 
        /* std::cout << "Problem!\n\n"; */ 
    }

    IndicesType indices; 
    getVertexIndices(index, indices); 

    auto cast = []( Natural n ) -> double { return static_cast<double>(n); }; 
    v.x = origin[0] + cast(indices[0]) * lenght[0]/cast(nX_ - 1); 
    v.y = origin[1] + cast(indices[1]) * lenght[1]/cast(nY_ - 1); 

    /* std::cout << v.x << " " << v.y << std::endl; */ 
    /* std::cout << indices[0] << " " << indices[1] << std::endl; */ 
    return true; 
}

bool PlanarSurface::getVertex3D( Natural index, Point3 &v ) {
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    Point2 p2; 
    if ( getVertex2D(index, p2) == false ) { 
        return false; 
    }

    v.x = p2.x; 
    v.y = p2.y; 
    /* bool valid_vertex = getCachedHeight(index, v.z); */ 
    bool valid_vertex = getHeight(index, v.z); 
    /* std::cout << "Problem!\n\n"; */ 

    return valid_vertex; 
}

bool PlanarSurface::getMappedVertex3D( Natural index, Point3 &v ) {
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    Point2 p2; 
    if ( getVertex2D(index, p2) == false ) { 
        return false; 
    }

    Point3 w;

    w.x = p2.x; 
    w.y = p2.y; 
    /* bool valid_vertex = getCachedHeight(index, w.z); */ 
    bool valid_vertex = getHeight(index, w.z); 
    /* std::cout << "Problem!\n\n"; */ 

    v.x = w[coordinates_map_[0]];
    v.y = w[coordinates_map_[1]];
    v.z = w[coordinates_map_[2]];

    return valid_vertex; 
}

double PlanarSurface::getTolerance() 
{
    updateDiscretization();

    return tolerance; 
}

InterpolatedGraph::WeakPtr PlanarSurface::get_interpolant() { 
    return InterpolatedGraph::WeakPtr(f); 
}

bool PlanarSurface::surfaceIsSet() { 
    /* return (interpolant_is_set_ && mesh_is_set_); */ 
    return interpolant_is_set_; 
}

unsigned long int PlanarSurface::getID() const 
{
    return id_; 
}

bool PlanarSurface::checkIfDependsOn( unsigned long int surface_id ) { 

    if ( dependency_list_.find(surface_id) == dependency_list_.end() ) { 
        return false; 
    }

    return true;
}

bool PlanarSurface::getHeight( const Point2 &p, double &height ) { 

    /* double lb = origin.z; */  
    /* double ub = origin.z + lenght.z; */  

    /* bool status = f->getBoundedHeight(p, height, ub, lb); */ 
    /* if ( status == false ) { */ 
        /* return false; */ 
    /* } */

    /* if ( height > ub ) { */ 
    /*     height = ub; */ 
    /*     /1* status &= false; *1/ */ 
    /* } */
    /* else if ( height < lb ) { */ 
    /*     height = lb; */ 
    /*     /1* status &= false; *1/ */ 
    /* } */

    /* return status; */ 

    bool status = f->getRawHeight(p, height);  

    if ( status == false ) { 
        return false; 
    }

    double lb = origin.z;  
    double ub = origin.z + lenght.z;  

    if ( height > ub ) { 
        height = ub; 
        /* status &= false; */ 
    }
    else if ( height < lb ) { 
        height = lb; 
        /* status &= false; */ 
    }

    /* #pragma omp critical */
    /* std::cout << "Trying: " << vertex_index << "... "; */  

    /* std::cout << "I'm here! \n\n"; */ 

    for ( auto &p_upper_bound_ : upper_bound_ )
    /* for ( auto &p_upper_bound_ : ub_list ) */
        if ( auto upper_surface = p_upper_bound_.lock() ) { 
            /* if ( upper_surface->getHeight(p, ub) ) */
            if ( upper_surface->surfaceIsSet() ) { 
                upper_surface->getHeight(p, ub); 
                if ( height >= ub ) { 
                    /* #pragma omp critical */
                    /* std::cout << "failed above! \n"; */ 
                    height = ub; 
                    status &= false; 
                    /* return false; */ 
                }
            }
        }

    for ( auto &p_lower_bound_ : lower_bound_ )
    /* for ( auto &p_lower_bound_ : lb_list ) */
        if ( auto lower_surface = p_lower_bound_.lock() ) { 
            /* if ( lower_surface->getHeight(p, lb) ) */  
            if ( lower_surface->surfaceIsSet() ) { 
                lower_surface->getHeight(p, lb); 
                if ( height <= lb ) { 
                    /* #pragma omp critical */
                    /* std::cout << "failed below! \n"; */ 
                    height = lb; 
                    status &= false; 
                    /* return false; */ 
                }
            }
        }

    /* #pragma omp critical */
    /* std::cout << "worked! \n"; */  

    return status; 
}

bool PlanarSurface::getHeight( Natural vertex_index, double &height, SurfaceId &bounding_surface_id ) 
{ 
    return getHeight(vertex_index, height, heights, upper_bound_, lower_bound_, bounding_surface_id);
}

bool PlanarSurface::getHeight( Natural i, Natural j, double &height, SurfaceId &bounding_surface_id ) 
{
    return getHeight( getVertexIndex(i, j), height, bounding_surface_id );
}

bool PlanarSurface::getRawHeight( Natural vertex_index, double &height )
{
    if ( interpolant_is_set_ == false ) { 
        return false; 
    }

    updateDiscretization(); 

    height = heights[vertex_index];

    return true;
}

bool PlanarSurface::getRawHeight( Natural i, Natural j, double &height )
{
    return getRawHeight( getVertexIndex(i,j), height);
}


// getRelativeHeight
bool PlanarSurface::getHeight( Natural vertex_index, double &height, std::vector<double> &base_heights, 
        std::list<std::weak_ptr<PlanarSurface>> &ub_list,std::list<std::weak_ptr<PlanarSurface>> &lb_list, 
        SurfaceId &bounding_surface_id  )
{
    if ( interpolant_is_set_ == false ) { 
        return false; 
    }

    updateDiscretization(); 

    bool status = true; 
    if ( cache_is_fresh_ )
    {
        height = cached_heights_[vertex_index];
        status = cached_valid_heights_[vertex_index];
        bounding_surface_id = cached_bounding_surface_ids_[vertex_index];

        return status;
    }

    /* height = heights[vertex_index]; */  
    height = base_heights[vertex_index];  
    double lb = origin.z;  
    double ub = origin.z + lenght.z;  

    double sup_lbound = lb;
    double inf_ubound = ub;
    /* bool sup_lbound_status, inf_ubound_status; */

    bool ustatus, lstatus;
    SurfaceId ubounding_surface_id = id_;
    SurfaceId lbounding_surface_id = id_;
    bounding_surface_id = id_; //std::numeric_limits<SurfaceId>::max();

    if ( height > ub ) { 
        height = ub; 
        /* status &= false; */ 
    }
    else if ( height < lb ) { 
        height = lb; 
        /* status &= false; */ 
    }

    /* #pragma omp critical */
    /* std::cout << "Trying: " << vertex_index << "... "; */  

    /* std::cout << "I'm here! \n\n"; */ 

    for ( auto &p_upper_bound_ : ub_list )
    {
        if ( auto upper_surface = p_upper_bound_.lock() )
        {
            if ( upper_surface->surfaceIsSet() )
            {
                ustatus = upper_surface->getHeight(vertex_index, ub);
                if ( ub < inf_ubound )
                {
                    inf_ubound = ub;
                    ubounding_surface_id = upper_surface->getID();
                }
                else if ( (abs(ub - inf_ubound) < tolerance) && ustatus )
                {
                    inf_ubound = ub;
                    ubounding_surface_id = upper_surface->getID();
                }
            }
        }
    }

    for ( auto &p_lower_bound_ : lb_list )
    {
        if ( auto lower_surface = p_lower_bound_.lock() )
        {
            lstatus = lower_surface ->getHeight(vertex_index, lb);
            if ( lb > sup_lbound )
            {
                sup_lbound = lb;
                lbounding_surface_id = lower_surface->getID();
            }
            else if ( (abs(lb - sup_lbound) < tolerance) && lstatus )
            {
                sup_lbound = lb;
                lbounding_surface_id = lower_surface->getID();
            }

        }
    }

    if ( height > inf_ubound )
    {
        height = inf_ubound;
        bounding_surface_id = ubounding_surface_id;
        status = false;
    }
    
    if ( height < sup_lbound )
    {
        height = sup_lbound;
        bounding_surface_id = lbounding_surface_id;
        status = false;
    }

    /* std::cout << "Surface: " << getID() << ", bid: " << bounding_surface_id << ", height: " << height << ", inf_ubound: " << inf_ubound << ", sup_lbound: " << sup_lbound << std::endl << std::flush; */

    /* /1* for ( auto &p_upper_bound_ : upper_bound_ ) *1/ */
    /* for ( auto &p_upper_bound_ : ub_list ) */
    /*     if ( auto upper_surface = p_upper_bound_.lock() ) { */ 
    /*         /1* if ( upper_surface->getHeight(vertex_index, ub) ) *1/ */
    /*         if ( upper_surface->surfaceIsSet() ) { */ 
    /*             ustatus = upper_surface->getHeight(vertex_index, ub, ubounding_surface_id); */ 
    /*             if ( height >= ub ) { */ 
    /*                 /1* #pragma omp critical *1/ */
    /*                 /1* std::cout << "failed above! \n"; *1/ */ 
    /*                 height = ub; */ 
    /*                 if ( ustatus ) */
    /*                     /1* ubounding_surface_id = upper_surface->getID(); *1/ */
    /*                     bounding_surface_id = ubounding_surface_id; */
    /*                 status &= false; */ 
    /*                 /1* return false; *1/ */ 
    /*             } */
    /*         } */
    /*     } */

    /* /1* for ( auto &p_lower_bound_ : lower_bound_ ) *1/ */
    /* for ( auto &p_lower_bound_ : lb_list ) */
    /*     if ( auto lower_surface = p_lower_bound_.lock() ) { */ 
    /*         /1* if ( lower_surface->getHeight(vertex_index, lb) ) *1/ */  
    /*         if ( lower_surface->surfaceIsSet() ) { */ 
    /*             lstatus = lower_surface->getHeight(vertex_index, lb, lbounding_surface_id); */ 
    /*             if ( height <= lb ) { */ 
    /*                 /1* #pragma omp critical *1/ */
    /*                 /1* std::cout << "failed below! \n"; *1/ */ 
    /*                 height = lb; */ 
    /*                 if ( lstatus ) */
    /*                     /1* lbounding_surface_id = lower_surface->getID(); *1/ */
    /*                     bounding_surface_id = lbounding_surface_id; */
    /*                 status &= false; */ 
    /*                 /1* return false; *1/ */ 
    /*             } */
    /*         } */
    /*     } */

    /* if ( status ) */
    /* { */
    /*     bounding_surface_id = id_; */
    /* } */
    /* else */ 
    /* { */
        /* auto error = std::numeric_limits<SurfaceId>::max(); */
        /* if ( (lbounding_surface_id != id_) && (ubounding_surface_id != id_) ) */
        /* { */
        /*     bounding_surface_id = error; */
        /* } */
        /* else if ( lbounding_surface_id != id_ ) */
        /* { */
        /*     bounding_surface_id = lbounding_surface_id; */
        /* } */
        /* else if ( ubounding_surface_id != id_ ) */
        /* { */
        /*     bounding_surface_id = ubounding_surface_id; */
        /* } */
    /* } */
    /* #pragma omp critical */
    /* std::cout << "worked! \n"; */  

    return status; 
}

bool PlanarSurface::getHeight( Natural i, Natural j, double &height )
{
    return getHeight( getVertexIndex(i, j), height );
}

bool PlanarSurface::getHeight( Natural vertex_index, double &height )
{
    SurfaceId dummy_var;
    return getHeight(vertex_index, height, dummy_var);
}

bool PlanarSurface::getCachedHeight( Natural vertex_index, double &height )
{
    if ( interpolant_is_set_ == false ) { 
        return false; 
    }

    if ( vertex_index >= num_vertices_ )
    {
        return false;
    }

    bool valid_vertex;
    /* auto has_unprocessed_bounds = []( std::list<PlanarSurface::WeakPtr> &unprocessed_list ) -> bool */ 
    /* { */
    /*     bool is_empty = unprocessed_list.empty(); */

    /*     if ( !is_empty ) */
    /*     { */
    /*         is_empty = true; */
    /*         for ( auto &wprt : unprocessed_list ) */
    /*         { */
    /*             if ( auto sptr = wprt.lock() ) */
    /*             { */
    /*                 is_empty = false; */
    /*             } */
    /*         } */
    /*     } */

    /*     return is_empty; */
    /* }; */

    if ( cache_is_fresh_ )
    {
        height = cached_heights_[vertex_index];
        valid_vertex = cached_valid_heights_[vertex_index];
        /* bounding_surface_id = cached_bounding_surface_ids_[vertex_index]; */
    }
    else
    {
        /* // this may create a logical inconsistency if surfaces are created with a different logic than */ 
        /* // what is expected from SRules: i.e. new surfaces that modify the model don't have any dependencies, */ 
        /* // or new surfaces that were modified by the model do not modify other surfaces */
        /* if ( has_unprocessed_bounds(unprocessed_lower_bound_) || has_unprocessed_bounds(unprocessed_upper_bound_) ) */
        /* { */
        /*     valid_vertex = getHeight(vertex_index, height, cached_heights_, unprocessed_upper_bound_, unprocessed_lower_bound_); */
        /* } */
        /* else */
        /* { */
        /*     valid_vertex = getHeight(vertex_index, height); */
        /* } */

        valid_vertex = getHeight(vertex_index, height);
    }

    return valid_vertex;
}

bool PlanarSurface::getCachedHeight( Natural i, Natural j, double &height )
{
    return getCachedHeight( getVertexIndex(i, j), height );
}

bool PlanarSurface::getCachedBoundingSurfacesIDs( std::vector<SurfaceId> &bounding_surface_ids )
{
    if ( cache_is_fresh_ == false )
    {
        return false;
    }

    bounding_surface_ids = cached_bounding_surface_ids_;
    return true;
}

std::size_t PlanarSurface::getNumX()
{
    updateDiscretization(); 

    return nX_; 
}

std::size_t PlanarSurface::getNumY()
{
    updateDiscretization(); 
    return nY_; 
}

std::size_t PlanarSurface::getNumVertices()
{
    updateDiscretization(); 

    return num_vertices_;
}

bool PlanarSurface::getHeight( Point2 &&p, double &height ) { 
    return getHeight(p, height);
}

bool PlanarSurface::addPoint( const Point3 &p ) { 
    return f->addPoint(p); 
}

bool PlanarSurface::addPoint( Point3 &&p ) { 
    return f->addPoint(p); 
}

bool PlanarSurface::addPoints( const std::vector<Point3> &points ) { 
    return f->addPoints(points); 
}

bool PlanarSurface::addExtrusionPathPoint( double abscissa, double ordinate )
{
    return f->addExtrusionPathPoint(abscissa, ordinate);
}

void PlanarSurface::setPathOrigin( double abscissa, double ordinate )
{
    f->setPathOrigin(abscissa, ordinate);
}

bool PlanarSurface::generateSurface() { 
    if ( f->generateSurface() == false ) { 
        interpolant_is_set_ = false; 
        /* std::cout << "No interpolant today...\n\n"; */ 
        return false; 
    }
    interpolant_is_set_ = true; 
    /* std::cout << "Got interpolant!\n\n"; */ 

    updateRawCache();

    return true; 
}

bool PlanarSurface::updateRawCache() 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    bool is_smooth = ( f->isSmooth() >= 1 );
    if ( is_smooth )
    {
        normals.resize(3 * num_vertices_);
    }

    heights.resize(num_vertices_); 

    Point2 v {};  
    Point3 n {};


    auto &heights_omp = heights; 
    auto &f_omp = f; 
    auto &nlist_omp = normals;
    /* auto &cmap_omp = coordinates_map_; */ 


    /* TODO: revisit this logic to see whether it is possible to exploit the loop for the linear extrusion */
    /*       The lower loop was ignored because we now can extrude along a path */

    /* if ( isExtrudedSurface() == false ) */ 
    if ( true )
    { 
        auto num_vertices_omp = num_vertices_; 
        /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
        #pragma omp parallel for shared(heights_omp, nlist_omp, f_omp) firstprivate(num_vertices_omp, v, n, is_smooth) default(none) 
        for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
        {
            getVertex2D(i, v); 
            heights_omp[i] = f_omp->getRawHeight(v);  

            if ( is_smooth )
            {
                getNormal(v, n); 
                nlist_omp[3*i + 0] = n[0]; 
                nlist_omp[3*i + 1] = n[1]; 
                nlist_omp[3*i + 2] = n[2];
            }

        }
    }
    else 
    { 
        auto nX = nX_;
        auto nY = nY_; 
        long int index = 0; 
        double height = 0; 

        // The following loops do not use the cache optimally. 

        /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
        #pragma omp parallel for shared(heights_omp, f_omp) firstprivate(nX, nY, v, height, index) default(none) 
        for ( long int i = 0; i < static_cast<long int>(nX); ++i )  
        {
            index = getVertexIndex(i, 0); 
            getVertex2D(index, v); 
            height = f_omp->getRawHeight(v); 
            for ( long int j = 0; j < static_cast<long int>(nY); ++j ) 
            {
                index = getVertexIndex(i,j);
                heights_omp[index] = height;  
            }
        }
    }
    /* std::cout << "Got a mesh!\n\n"; */ 

    /* cached_heights_.resize(heights.size()); */
    cached_heights_.clear();
    // VS2013 cannot understand the following lambda:
    /* cached_valid_heights_.resize( heights.size(), []() -> bool { return true; } ); */
    cached_valid_heights_.resize( heights.size(), true );

    std::copy( heights.begin(), heights.end(), std::back_inserter(cached_heights_) );

    cached_bounding_surface_ids_.clear();
    cached_bounding_surface_ids_.resize( heights.size(), id_ );

    cache_is_fresh_ = false;

    mesh_is_set_ = true; 

    std::cout << "Raw cache is built for surface: " << id_ << std::endl << std::flush;

    return true; 
}

/* bool getMesh( BoundBox bbox, MeshType *mesh ); */ 
/* bool getMesh( std::vector<BoundBox> &bboxes, MeshType *mesh ); */ 

bool PlanarSurface::updateCache() 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    if ( cache_is_fresh_ )
    {
        return true;
    }

    std::cout << "Processing cache for surface: " << id_ << std::endl << std::flush;

    for ( auto &wptr: upper_bound_ )
    {
        if ( auto sptr = wptr.lock() )
        {
            sptr->updateCache();
        }
    }

    for ( auto &wptr: lower_bound_ )
    {
        if ( auto sptr = wptr.lock() )
        {
            sptr->updateCache();
        }
    }

    double height{};
    bool valid_vertex{};
    PlanarSurface::SurfaceId bounding_surface_id;
    
    surface_is_empty_ = true;

    for ( size_t i = 0; i < num_vertices_; ++i )
    {
        /* if ( !unprocessed_lower_bound_.empty() || !unprocessed_upper_bound_.empty() ) */
        /* { */
        /*     valid_vertex = getHeight(i, height, cached_heights_, unprocessed_upper_bound_, unprocessed_lower_bound_); */
        /* } */
        /* else */
        /* { */
        /*     valid_vertex = getCachedHeight(i, height); */
        /* } */

        /* valid_vertex = getCachedHeight(i, height); */
        valid_vertex = getHeight(i, height, bounding_surface_id);

        cached_heights_[i] = height;
        cached_valid_heights_[i] = valid_vertex;
        cached_bounding_surface_ids_[i] = bounding_surface_id;

        surface_is_empty_ &= !valid_vertex;
    }

    /* unprocessed_lower_bound_.clear(); */
    /* unprocessed_upper_bound_.clear(); */

    cache_is_fresh_ = true;
    std::cout << "Cache is built for surface: " << id_ << std::endl << std::flush;

    return true;
}

bool PlanarSurface::liesAbove( const Point3 &p ) 
{ 
 
    Point2 p2; 
    p2.x = p.x; 
    p2.y = p.y; 
    double height; 

    getHeight(p2, height );

    if ( p.z < height ) { 
        return false; 
    }

    return true; 
}

bool PlanarSurface::liesAbove( Point3 &&p ) 
{ 
    return liesAbove(p); 
}

bool PlanarSurface::liesBelow( const Point3 &p ) 
{ 
    Point2 p2; 
    p2.x = p.x; 
    p2.y = p.y; 
    double height; 

    getHeight(p2, height );

    if ( p.z > height ) { 
        return false; 
    }

    return true; 
}

bool PlanarSurface::liesBelow( Point3 &&p ) 
{ 
    return liesBelow(p); 
}

bool PlanarSurface::liesAboveRawSurface( const Point3 &p ) { 
    return f->liesAboveRawSurface(p);
}

bool PlanarSurface::liesAboveRawSurface( Point3 &&p ) { 
    return f->liesAboveRawSurface(p); 
}

bool PlanarSurface::liesBelowRawSurface( const Point3 &p ) { 
    return f->liesBelowRawSurface(p); 
}

bool PlanarSurface::liesBelowRawSurface( Point3 &&p ) { 
    return f->liesBelowRawSurface(p); 
}

bool PlanarSurface::removeAbove( PlanarSurface::Ptr &s ) { 
    if ( s->surfaceIsSet() == false ) { 
        return false; 
    }

    bool status_ra_interpolant = f->removeAbove( s->get_interpolant() ); 
    if ( status_ra_interpolant == false ) { 
        return false; 
    }

    unsigned long int my_id = getID(); 
    bool has_cyclic_dependency = s->checkIfDependsOn(my_id); 

    if ( has_cyclic_dependency == true ) { 
        // Shall never happen.
        throw std::runtime_error("\nDependency lists of planar mesh and its included interpolated graph differed.\n "); 
    }

    s->getDependencyList( dependency_list_ ); 

    /* if ( surfaceIsSet() == true ) { */ 
    /*     mesh_is_set_ = false; */ 
    /* } */

    upper_bound_.push_back(std::weak_ptr<PlanarSurface>(s));
    auto itl = lower_bound_.begin();
    while (itl != lower_bound_.end())
    {
        if (compareSurfaceWptr(*itl, std::weak_ptr<PlanarSurface>(s)) == true)
        {
            itl = lower_bound_.erase(itl);
        }
        else
        {
            ++itl;
        }
    }
    /* unprocessed_upper_bound_.push_back(std::weak_ptr<PlanarSurface>(s)); */
    cache_is_fresh_ = false;

    pruneBoundingLists();

    std::list< std::weak_ptr<PlanarSurface> > lbound{};
    s->getLowerBoundList(lbound);

    bool new_bound = true;

    for ( auto &lb : lbound )
    {
        new_bound = true;

        for (auto this_lb : lower_bound_)
            if (compareSurfaceWptr(lb, this_lb) == true)
                new_bound &= false;

        for ( auto &ub : upper_bound_ )
        {
            new_bound &= (compareSurfaceWptr(lb, ub) == false);
        }

        if ( new_bound )
        {
            lower_bound_.push_back(lb);
            /* unprocessed_lower_bound_.push_back(lb); */
        }

    }

    return true; 
}

bool PlanarSurface::removeAbove( PlanarSurface::WeakPtr s ) 
{ 
    auto sptr = s.lock(); 
    if ( s.expired() ) { 
        return false; 
    }

    return removeAbove(sptr); 
}

bool PlanarSurface::removeBelow( PlanarSurface::Ptr &s ) 
{ 
    if ( s->surfaceIsSet() == false ) { 
        return false; 
    }

    bool status_rb_interpolant = f->removeBelow( s->get_interpolant() ); 
    if ( status_rb_interpolant == false ) { 
        return false; 
    }

    unsigned long int my_id = getID(); 
    bool has_cyclic_dependency = s->checkIfDependsOn(my_id); 

    if ( has_cyclic_dependency == true ) { 
        // Shall never happen.
        throw std::runtime_error("\nDependency lists of planar mesh and its included interpolated graph differed.\n "); 
    }

    s->getDependencyList( dependency_list_ ); 

    /* if ( surfaceIsSet() == true ) { */ 
    /*     mesh_is_set_ = false; */ 
    /* } */

    // lower_bound_.push_back( std::weak_ptr<PlanarSurface>(s) ); 
    // s->getUpperBoundList(upper_bound_); 

    lower_bound_.push_back(std::weak_ptr<PlanarSurface>(s));
    auto itu = upper_bound_.begin();
    while (itu != upper_bound_.end())
    {
        if (compareSurfaceWptr(*itu, std::weak_ptr<PlanarSurface>(s)) == true)
        {
            itu = upper_bound_.erase(itu);
        }
        else
        {
            ++itu;
        }
    }
    /* unprocessed_lower_bound_.push_back(std::weak_ptr<PlanarSurface>(s)); */
    cache_is_fresh_ = false;

    pruneBoundingLists();

    std::list< std::weak_ptr<PlanarSurface> > ubound{};
    s->getUpperBoundList(ubound);

    bool new_bound = true;

    for (auto &ub : ubound)
    {
        new_bound = true;

        for (auto this_ub: upper_bound_)
            if (compareSurfaceWptr(ub, this_ub) == true)
                new_bound &= false;

        for (auto &lb : lower_bound_)
        {
            new_bound &= (compareSurfaceWptr(ub, lb) == false);
        }

        if (new_bound)
        {
            upper_bound_.push_back(ub);
            /* unprocessed_upper_bound_.push_back(ub); */
        }

    }

    return true; 
}

bool PlanarSurface::removeBelow( PlanarSurface::WeakPtr s ) 
{ 
    auto sptr = s.lock(); 
    if ( s.expired() ) { 
        return false; 
    }

    return removeBelow(sptr); 
}

bool PlanarSurface::project( Point3 &p ) { 
    Point2 p2; 
    p2.x = p.x; 
    p2.y = p.y; 

    if ( getHeight(p2, p.z) == false ) { 
        return false; 
    }

    return true; 
}

void PlanarSurface::pruneBoundingLists() 
{
    auto itu = upper_bound_.begin();
    while ( itu != upper_bound_.end() )
    {
        if ( itu->expired() ) 
        {
            itu = upper_bound_.erase(itu);
            cache_is_fresh_ = false;
            std::cout << "Cache was marked unfresh for surface: " << id_ << std::endl << std::flush;
        }
        else
        {
            ++itu;
        }
    }

    auto itl = lower_bound_.begin();
    while ( itl != lower_bound_.end() )
    {
        if ( itl->expired() ) 
        {
            itl = lower_bound_.erase(itl);
            cache_is_fresh_ = false;
            std::cout << "Cache was marked unfresh for surface: " << id_ << std::endl << std::flush;
        }
        else
        {
            ++itl;
        }
    }

    /* // now for unprocessed surfaces, whenever something is not found all must be cleared */
    /* bool unprocessed_list_is_clean = true; */
    /* itu = unprocessed_upper_bound_.begin(); */
    /* while ( itu != unprocessed_upper_bound_.end() ) */
    /* { */
    /*     if ( itu->expired() ) */ 
    /*     { */
    /*         /1* itu = upper_bound_.erase(itu); *1/ */
    /*         unprocessed_list_is_clean = false; */
    /*     } */
    /*     else */
    /*     { */
    /*         ++itu; */
    /*     } */
    /* } */

    /* itl = unprocessed_lower_bound_.begin(); */
    /* while ( itl != unprocessed_lower_bound_.end() ) */
    /* { */
    /*     if ( itl->expired() ) */ 
    /*     { */
    /*         /1* itl = unprocessed_lower_bound_.erase(itl); *1/ */
    /*         unprocessed_list_is_clean = false; */
    /*     } */
    /*     else */
    /*     { */
    /*         ++itl; */
    /*     } */
    /* } */

    /* if ( unprocessed_list_is_clean == false ) */
    /* { */
    /*     unprocessed_lower_bound_.clear(); */
    /*     unprocessed_upper_bound_.clear(); */
    /* } */

    f->pruneBoundingLists(); 
}

void PlanarSurface::clearBoundingLists() 
{
    upper_bound_.clear(); 
    lower_bound_.clear(); 

    f->clearBoundingLists(); 
}

/* Methods */ 
bool PlanarSurface::rangeCheck( Natural i, Natural j ) { 
    /*TODO: Range should be <, not <=; verify if this is a bug. */
    return ( (i <= nX_) && (j <= nY_) ); 
}

// PlanarSurface and TetrahedralMeshBuilder disagree on the ordering of the vertices
PlanarSurface::Natural PlanarSurface::getVertexIndex( Natural i, Natural j ) const { 
    /* return ( j + i*nY_ ); */ 
    return ( i + j*nX_ ); 
}

bool PlanarSurface::getVertexIndices( Natural v, IndicesType &indices ) { 
    if ( rangeCheck(v) == false ) { 
        return false; 
    }

    indices[0] = v % nX_; 
    indices[1] = ( v - indices[0] )/nX_;  

    return true; 
}

size_t PlanarSurface::getBlockIndex( size_t i, size_t j ) const
{
    return i + j*discretization_X;
}

PlanarSurface::IndicesType PlanarSurface::getBlockIndices( size_t b ) const
{
    IndicesType indices;

    indices[0] = b % discretization_X;
    indices[1] = (b - indices[0])/discretization_X;

    return indices;
};

size_t PlanarSurface::getVertexIndexFromPositionInBlock( size_t vpos,  size_t bindex ) const
{
    IndicesType indices = getBlockIndices(bindex);;

    size_t i = vpos % 3;
    size_t j = (vpos - i)/3;

    size_t iv = 2*indices[0] + i;
    size_t jv = 2*indices[1] + j;

    return getVertexIndex(iv, jv);
}

#include "mesh/polyhedra.hpp"

TriangleHeights PlanarSurface::getTriangleHeightsFromPositionInBlock( size_t tpos, size_t bindex ) 
{
    // Range check?

    auto setTriangle = [&] ( size_t i0_pos, size_t i1_pos, size_t i2_pos,  TriangleHeights &t ) -> void 
    {
        bool b0, b1, b2;
        size_t i0, i1, i2;
        double h0, h1, h2;
        /* Point3 v0, v1, v2; */

        i0 = getVertexIndexFromPositionInBlock(i0_pos, bindex);
        i1 = getVertexIndexFromPositionInBlock(i1_pos, bindex);
        i2 = getVertexIndexFromPositionInBlock(i2_pos, bindex);

        /* b0 = getCachedHeight(i0, h0); */
        b0 = getHeight(i0, h0);
        /* b1 = getCachedHeight(i1, h1); */
        b1 = getHeight(i1, h1);
        /* b2 = getCachedHeight(i2, h2); */
        b2 = getHeight(i2, h2);

        /* t.setVertex(0, v0, i0, b0); */
        /* t.setVertex(1, v1, i1, b1); */
        /* t.setVertex(2, v2, i2, b2); */
        t.setVertex(0, h0, b0);
        t.setVertex(1, h1, b1);
        t.setVertex(2, h2, b2);
    };

    TriangleHeights t;

    switch( tpos )
    {
        case 0:
            setTriangle(0, 1, 4, t);
            break;

        case 1:
            setTriangle(0, 4, 3, t);
            break;

        case 2:
            setTriangle(1, 2, 4, t);
            break;

        case 3:
            setTriangle(2, 5, 4, t);
            break;

        case 4:
            setTriangle(3, 4, 6, t);
            break;

        case 5:
            setTriangle(4, 7, 6, t);
            break;

        case 6:
            setTriangle(4, 5, 8, t);
            break;

        case 7:
            setTriangle(4, 8, 7, t);
            break;

        default:
            break;
    }

    return t;
}

size_t PlanarSurface::getTriangleIndexFromPositionInBlock( size_t tpos, size_t bindex ) const
{
    size_t numBlocks = PlanarSurface::discretization_X * PlanarSurface::discretization_Y;
    size_t numTrianglesPerBlock = 8;
    size_t numTriangles = numTrianglesPerBlock * numBlocks;

    if ( (tpos > numTriangles) || (bindex > numBlocks) )
        return numTriangles;

    return tpos + bindex * numTrianglesPerBlock;
}

std::vector<size_t> PlanarSurface::getTriangleConnectivityFromIndex(size_t tindex ) const
{
    size_t numTrianglesPerBlock = 8;
    size_t numBlocks = discretization_X * discretization_Y;
    size_t numTriangles = numTrianglesPerBlock * numBlocks;

    std::vector<size_t> triangle;
    if ( tindex >= numTriangles )
    {
        return triangle;
    }

    size_t tpos = tindex % numTrianglesPerBlock;
    size_t bindex = (tindex - tpos) / numTrianglesPerBlock;

    auto getTriangle = [&] ( size_t v0_pos, size_t v1_pos, size_t v2_pos ) -> std::vector<size_t> 
    {
        std::vector<size_t> triangle;
        size_t v0, v1, v2;

        v0 = getVertexIndexFromPositionInBlock(v0_pos, bindex);
        v1 = getVertexIndexFromPositionInBlock(v1_pos, bindex);
        v2 = getVertexIndexFromPositionInBlock(v2_pos, bindex);

        triangle = {v0, v1, v2};

        return triangle;
    };

    switch( tpos )
    {
        case 0:
            triangle = getTriangle(0, 1, 4);
            break;

        case 1:
            triangle = getTriangle(0, 4, 3);
            break;

        case 2:
            triangle = getTriangle(1, 2, 4);
            break;

        case 3:
            triangle = getTriangle(2, 5, 4);
            break;

        case 4:
            triangle = getTriangle(3, 4, 6);
            break;

        case 5:
            triangle = getTriangle(4, 7, 6);
            break;

        case 6:
            triangle = getTriangle(4, 5, 8);
            break;

        case 7:
            triangle = getTriangle(4, 8, 7);
            break;

        default:
            break;
    }

    return triangle;
}

size_t PlanarSurface::getTriangleIndexFromConnectivity( std::vector<size_t> t_connectivity )
{
    auto baseConnectivity = [] ( size_t tpos ) -> std::vector<size_t> 
    {
        std::vector<size_t> triangle; 

        switch( tpos )
        {
            case 0:
                triangle = {0, 1, 4};
                break;

            case 1:
                triangle = {0, 4, 3};
                break;

            case 2:
                triangle = {1, 2, 4};
                break;

            case 3:
                triangle = {2, 5, 4};
                break;

            case 4:
                triangle = {3, 4, 6};
                break;

            case 5:
                triangle = {4, 7, 6};
                break;

            case 6:
                triangle = {4, 5, 8};
                break;

            case 7:
                triangle = {4, 8, 7};
                break;

            default:
                break;
        }

        return triangle;
    };

    /* size_t PlanarSurface::getVertexIndexFromPositionInBlock( size_t vpos,  size_t bindex ) const */
    /* { */
    /*     IndicesType indices = getBlockIndices(bindex);; */

    /*     size_t i = vpos % 3; */
    /*     size_t j = (vpos - i)/3; */

    /*     size_t iv = 2*indices[0] + i; */
    /*     size_t jv = 2*indices[1] + j; */

    /*     return getVertexIndex(iv, jv); */
    /* } */

    size_t numBlocks = PlanarSurface::discretization_X * PlanarSurface::discretization_Y;

    auto bindexFromVindexAndVpos = [&] ( size_t vindex, size_t vpos ) -> size_t
    {
        size_t ib, jb;
        IndicesType vindices; 

        bool success = getVertexIndices(vindex, vindices);
        if ( !success )
            return numBlocks;

        ib = (vindices[0] - vpos % 3);
        jb = (vindices[1] - ( vpos - (vpos%3) ));

        if ( ( (ib % 2) != 0 ) || ( (jb % 2) != 0 ) )
            return numBlocks;

        return getBlockIndex(ib/2, jb/2);
    };

    //
    // Given tpos get bindex
    //
    size_t numTrianglesPerBlock = 8;

    size_t b0, b1, b2;
    size_t tpos = numTrianglesPerBlock;
    size_t bindex = numBlocks;
    std::vector<size_t> base_connectivity;

    for ( size_t i = 0; i < numTrianglesPerBlock; ++i )
    {
        base_connectivity = baseConnectivity(i);

        b0 = bindexFromVindexAndVpos(t_connectivity[0], base_connectivity[0]);
        b1 = bindexFromVindexAndVpos(t_connectivity[1], base_connectivity[1]);
        b2 = bindexFromVindexAndVpos(t_connectivity[2], base_connectivity[2]);

        if ( (b0 == b1) && (b1 == b2) && (b0 < numBlocks) )
        {
            bindex = b0;
            tpos = i;
            break;
        }
    }

    // Get tindex from tpos & bindex
    return getTriangleIndexFromPositionInBlock(bindex, tpos);
}

bool PlanarSurface::isExtrudedSurface() 
{ 
    return f->isExtrudedSurface(); 
} 

bool PlanarSurface::isPathExtrudedSurface()
{
    return f->isPathExtrudedSurface();
}

bool PlanarSurface::isOrthogonallyOrientedSurface()
{
    return f->isOrthogonallyOrientedSurface();
}

bool PlanarSurface::compareSurfaceWptr( const PlanarSurface::WeakPtr &left, const PlanarSurface::WeakPtr &right ) const
{
    auto sleft = left.lock(); 
    auto sright = right.lock(); 

    if ( (sleft == nullptr) || (sright == nullptr) ) { 
        return false; 
    }

    if ( sleft->getID() == sright->getID() ) { 
        return true; 
    }

    return false; 
}

bool PlanarSurface::getRawData( std::vector<Point2> &points, std::vector<double> &fevals )
{
    return f->getRawData(points, fevals);
}

