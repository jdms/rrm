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
    tolerance = 0.9 * std::max( 1.0/static_cast<double>(nX_), 1.0/static_cast<double>(nY_) ); 
    /* std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(8) << "\n --> tolerance: " << tolerance << std::endl; */ 

    /* if ( this_discretization_state_ < global_discretization_state_ ) */ 
    if ( surfaceIsSet() == true ) 
    { 
        updateCache(); 
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

PlanarSurface::PlanarSurface( bool extruded_surface ) : id_(num_instances_) { 
    /* std::cout << "My id: " << id_ << std::endl; */ 
    ++num_instances_; 

    this_discretization_state_ = 0; 
    updateDiscretization(); 

    /* If Visual Studio 2013 complains with the following line yielding "error C2783" be advised that IT IS A BUG IN VS2013's COMPILER. */ 
    /* The "Visual Studio 2013 update 5" for VS2013 is known to have a fix to the problem. */ 
    f = std::make_shared<InterpolatedGraph>( extruded_surface );
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

    double height; 
    double lb = origin.z;  
    double ub = origin.z + lenght.z;  

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for firstprivate(num_vertices_omp, lb, ub) private(height, status) default(none) reduction(&&: isEntireSurface) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
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
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    bool has_lower_boundary = false; 
    bool has_upper_boundary = false; 

    if ( ( lower_surface != nullptr ) && lower_surface->surfaceIsSet() ) { 
        has_lower_boundary = true; 
        /* std::cout << "Lower boundary got here!\n"; */ 
    }

    if ( ( upper_surface != nullptr ) && upper_surface->surfaceIsSet() ) { 
        has_upper_boundary = true; 
        /* std::cout << "Upper boundary got here!\n"; */ 
    }

    bool isEntireSurface = true; 
    bool status, lstatus, ustatus; 

    updateDiscretization(); 
    double height, lheight, uheight; ; 

    double lb = origin.z;  
    double ub = origin.z + lenght.z;  

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(lower_surface, upper_surface) firstprivate(ub, lb, num_vertices_omp, has_lower_boundary, has_upper_boundary) private(status, lstatus, ustatus, height, lheight, uheight) default(none) reduction(&&: isEntireSurface) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
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

        if ( status == false ) 
        { 
            if ( has_lower_boundary ) 
            { 
                lstatus = lower_surface->getHeight(i, lheight); 
                /* std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(8) << "lstatus value: " << lstatus; */ 
                if ( lstatus && ( std::fabs(lheight - height) <= tolerance ) ) {
                    status |= true; 
                }
                else { 
                    /* std::cout << " missed index: " << i << ", distance: " << height - lheight << ", tolerance: " << tolerance << std::endl; */ 
                }
            }
            if ( has_upper_boundary ) 
            {
                ustatus = upper_surface->getHeight(i, uheight); 
                if ( ustatus && ( std::fabs(height - uheight) <= tolerance ) ) { 
                    status |= true; 
                }
            }
        }

        isEntireSurface = isEntireSurface && status; 
    }

    return isEntireSurface; 
    
    return false;
}

bool PlanarSurface::weakBoundedEntireSurfaceCheck( PlanarSurface::WeakPtr lower_surface, PlanarSurface::WeakPtr upper_surface ) 
{ 
    PlanarSurface::Ptr lptr = lower_surface.lock(); 
    PlanarSurface::Ptr uptr = upper_surface.lock();  

    return weakBoundedEntireSurfaceCheck(lptr, uptr); 
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
    double height, s_height; 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp) private(height, s_height) default(none) reduction(||: lies_above, lies_below) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        if ( getHeight(i, height) && sp->getHeight(i, s_height) ) {  
            lies_above = lies_above || (height >= s_height); 
            lies_below = lies_below || (height <= s_height); 
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
    double height, s_height; 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp) private(height, s_height) default(none) reduction(||: lies_above, lies_below) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        getHeight(i, height); 
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
    double height, s_height; 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp) private(height, s_height) default(none) reduction(&&: lies_above) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        getHeight(i, height); 
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
    double height, s_height; 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp) private(height, s_height) default(none) reduction(&&: lies_below) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        getHeight(i, height); 
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
    double height, s_height; 
    double tolerance = getTolerance(); 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp, tolerance) private(height, s_height) default(none) reduction(&&: lies_above) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        getHeight(i, height); 
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
    double height, s_height; 
    double tolerance = getTolerance(); 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(sp) firstprivate(num_vertices_omp, tolerance) private(height, s_height) default(none) reduction(&&: lies_below) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        getHeight(i, height); 
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
    bool valid_vertex = getHeight(index, v.z); 
    /* std::cout << "Problem!\n\n"; */ 

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
    
    bool status = f->getHeight(p, height); 
    if ( status == false ) { 
        return false; 
    }

    double lb = origin.z;  
    double ub = origin.z + lenght.z;  
    
    if ( height > ub ) { 
        height = ub; 
        status &= false; 
    }
    else if ( height < lb ) { 
        height = lb; 
        status &= false; 
    }

    return status; 
}

bool PlanarSurface::getHeight( Natural vertex_index, double &height ) { 
    if ( interpolant_is_set_ == false ) { 
        return false; 
    }

    updateDiscretization(); 
    height = heights[vertex_index];  
    double lb = origin.z;  
    double ub = origin.z + lenght.z;  
    bool status = true; 
    
    if ( height > ub ) { 
        height = ub; 
        status &= false; 
    }
    else if ( height < lb ) { 
        height = lb; 
        status &= false; 
    }

    /* #pragma omp critical */
    /* std::cout << "Trying: " << vertex_index << "... "; */  

    /* std::cout << "I'm here! \n\n"; */ 

    for ( auto &p_upper_bound_ : upper_bound_ )
        if ( auto upper_surface = p_upper_bound_.lock() ) { 
            /* if ( upper_surface->getHeight(vertex_index, ub) ) */
            if ( upper_surface->surfaceIsSet() ) { 
                upper_surface->getHeight(vertex_index, ub); 
                if ( height > ub ) { 
                    /* #pragma omp critical */
                    /* std::cout << "failed above! \n"; */ 
                    height = ub; 
                    status &= false; 
                    /* return false; */ 
                }
            }
        }

    for ( auto &p_lower_bound_ : lower_bound_ )
        if ( auto lower_surface = p_lower_bound_.lock() ) { 
            /* if ( lower_surface->getHeight(vertex_index, lb) ) */  
            if ( lower_surface->surfaceIsSet() ) { 
                lower_surface->getHeight(vertex_index, lb); 
                if ( height < lb ) { 
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

bool PlanarSurface::getHeight( Natural i, Natural j, double &height )
{
    return getHeight( getVertexIndex(i, j), height );
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
    return f->getHeight(p, height);
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

    updateCache(); 

    return true; 
}

bool PlanarSurface::updateCache() 
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
    mesh_is_set_ = true; 

    return true; 
}

/* bool getMesh( BoundBox bbox, MeshType *mesh ); */ 
/* bool getMesh( std::vector<BoundBox> &bboxes, MeshType *mesh ); */ 

bool PlanarSurface::liesAbove( const Point3 &p ) { 
    return f->liesAbove(p);
}

bool PlanarSurface::liesAbove( Point3 &&p ) { 
    return f->liesAbove(p); 
}

bool PlanarSurface::liesBelow( const Point3 &p ) { 
    return f->liesBelow(p); 
}

bool PlanarSurface::liesBelow( Point3 &&p ) { 
    return f->liesBelow(p); 
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

    upper_bound_.push_back( std::weak_ptr<PlanarSurface>(s) ); 
    s->getLowerBoundList(lower_bound_); 

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

    lower_bound_.push_back( std::weak_ptr<PlanarSurface>(s) ); 
    s->getUpperBoundList(upper_bound_); 

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
    return f->project(p); 
}

void PlanarSurface::pruneBoundingLists() 
{
    auto itu = upper_bound_.begin();
    while ( itu != upper_bound_.end() )
    {
        if ( itu->expired() ) 
        {
            itu = upper_bound_.erase(itu);
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
        }
        else
        {
            ++itl;
        }
    }

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

PlanarSurface::Natural PlanarSurface::getVertexIndex( Natural i, Natural j ) { 
    return ( j + i*nY_ ); 
}

bool PlanarSurface::getVertexIndices( Natural v, IndicesType &indices ) { 
    if ( rangeCheck(v) == false ) { 
        return false; 
    }

    indices[1] = v % nY_; 
    indices[0] = ( v - indices[1] )/nY_;  

    return true; 
}

bool PlanarSurface::isExtrudedSurface() 
{ 
    return extruded_surface_; 
} 

bool PlanarSurface::isPathExtrudedSurface()
{
    return f->isPathExtrudedSurface();
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

