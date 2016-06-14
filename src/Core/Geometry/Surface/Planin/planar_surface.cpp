/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 2.1 of the License, or (at your option) any later version.         */
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

unsigned long int PlanarSurface::num_instances_ = 0; 
unsigned long int PlanarSurface::global_discretization_state_ = 1; 

PlanarSurface::Natural PlanarSurface::discretization_X = 32;
PlanarSurface::Natural PlanarSurface::discretization_Y = 32;

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
        updateHeights(); 
    }
    
    this_discretization_state_ = global_discretization_state_; 
}

void PlanarSurface::requestChangeDiscretization( const Natural num_X, const Natural num_Y ) 
{
    if ( discretization_X > 0 ) { 
        discretization_X = num_X; 
    }

    if ( discretization_Y > 0 ) { 
        discretization_Y = num_Y; 
    }

    ++global_discretization_state_; 
}


PlanarSurface::PlanarSurface() : id_(num_instances_) { 
    ++num_instances_; 

    this_discretization_state_ = 0; 
    updateDiscretization(); 

    /* If Visual Studio 2013 complains with the following line yielding "error C2783" be advised that IT IS A BUG IN VS2013's COMPILER. */ 
    /* The "Visual Studio 2013 update 5" for VS2013 is known to have a fix to the problem. */ 
    f = std::make_shared<InterpolatedGraph>(); 

    /* If updating VS2013 is not possible, change the "make_shared" call to an explicit call to operator "new". */ 
    /* f = std::shared_ptr<InterpolatedGraph>( new InterpolatedGraph() ); */ 


    origin.x = 0.0; 
    origin.y = 0.0; 

    lenght.x = 1.0; 
    lenght.y = 1.0; 

    dependency_list_.insert(id_); 
}

PlanarSurface::PlanarSurface( const PlanarSurface &rhs ) : id_(num_instances_) 
{
    ++num_instances_; 

    this_discretization_state_ = rhs.this_discretization_state_; 
    nX_ = rhs.nX_; 
    nY_ = rhs.nY_; 
    num_vertices_ = rhs.num_vertices_; 
    tolerance = rhs.tolerance; 

    f = std::make_shared<InterpolatedGraph>(*rhs.f); 

    origin = rhs.origin; 
    lenght = rhs.lenght; 

    heights = rhs.heights; 
    interpolant_is_set_ = rhs.interpolant_is_set_; 
    mesh_is_set_ = rhs.mesh_is_set_; 

    upper_bound_ = rhs.upper_bound_; 
    lower_bound_ = rhs.lower_bound_; 

    dependency_list_ = rhs.dependency_list_; 
}

PlanarSurface::PlanarSurface( PlanarSurface &&rhs ) : id_( rhs.id_ ) 
{
    f = std::make_shared<InterpolatedGraph>( std::move(*rhs.f) ); 

    this_discretization_state_ = rhs.this_discretization_state_; 
    nX_ = rhs.nX_; 
    nY_ = rhs.nY_; 
    num_vertices_ = rhs.num_vertices_; 
    tolerance = rhs.tolerance; 

    origin = std::move(rhs.origin); 
    lenght = std::move(rhs.lenght); 

    heights = std::move(rhs.heights); 
    interpolant_is_set_ = rhs.interpolant_is_set_; 
    mesh_is_set_ = rhs.mesh_is_set_; 

    upper_bound_ = std::move(rhs.upper_bound_); 
    lower_bound_ = std::move(rhs.lower_bound_); 
    dependency_list_ = std::move(rhs.dependency_list_); 
}

void PlanarSurface::setOrigin( const Point2 &o ) 
{
    origin.x = o.x; 
    origin.y = o.y; 
} 

void PlanarSurface::setOrigin( Point2 &&o ) 
{
    setOrigin(o); 
} 

bool PlanarSurface::setLenght( const Point2 &l ) 
{ 
    if ( l.x <= 0 ) { 
        return false; 
    }

    if ( l.y <= 0 ) { 
        return false; 
    }

    lenght.x = l.x; 
    lenght.y = l.y; 

    return true; 
}

bool PlanarSurface::setLenght( Point2 &&l ) 
{
    return setLenght(l); 
}

Point2 PlanarSurface::getOrigin() 
{
    return origin; 
}

Point2 PlanarSurface::getLenght() 
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

    updateDiscretization(); 
    double height; 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for firstprivate(num_vertices_omp) private(height) default(none) reduction(&&: isEntireSurface) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        isEntireSurface = isEntireSurface && getHeight(i, height); 
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
        std::cout << "Lower boundary got here!\n"; 
    }

    if ( ( upper_surface != nullptr ) && upper_surface->surfaceIsSet() ) { 
        has_upper_boundary = true; 
        std::cout << "Upper boundary got here!\n"; 
    }

    bool isEntireSurface = true; 
    bool status, lstatus, ustatus; 

    updateDiscretization(); 
    double height, lheight, uheight; ; 

    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(lower_surface, upper_surface) firstprivate(num_vertices_omp, has_lower_boundary, has_upper_boundary) private(status, lstatus, ustatus, height, lheight, uheight) default(none) reduction(&&: isEntireSurface) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        status = getHeight(i, height); 
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

bool PlanarSurface::getVertex2D( Natural index, Point2 &v ) { 
    if ( rangeCheck(index) == false ) { 
        return false; 
        /* std::cout << "Problem!\n\n"; */ 
    }

    IndicesType indices; 
    getVertexIndices(index, indices); 

    auto cast = []( Natural n ) -> double { return static_cast<double>(n); };

//    v.x = origin[0] + cast(indices[0]) * lenght[0]/cast(nX_);
//    v.y = origin[1] + cast(indices[1]) * lenght[1]/cast(nY_);
    // FIXME changed by Felipe
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
    return f->getHeight(p, height); 
}

bool PlanarSurface::getHeight( Natural vertex_index, double &height ) { 
    if ( interpolant_is_set_ == false ) { 
        return false; 
    }

    updateDiscretization(); 
    height = heights[vertex_index];  
    double ub, lb;  
    /* #pragma omp critical */
    /* std::cout << "Trying: " << vertex_index << "... "; */  

    /* std::cout << "I'm here! \n\n"; */ 

    bool status = true; 
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

bool PlanarSurface::generateSurface() { 
    if ( f->generateSurface() == false ) { 
        interpolant_is_set_ = false; 
        /* std::cout << "No interpolant today...\n\n"; */ 
        return false; 
    }
    interpolant_is_set_ = true; 
    /* std::cout << "Got interpolant!\n\n"; */ 

    updateHeights(); 

    return true; 
}

bool PlanarSurface::updateHeights() 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    heights.resize(num_vertices_); 
    Point2 v;  

    auto &heights_omp = heights; 
    auto &f_omp = f; 
    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(heights_omp, f_omp) firstprivate(num_vertices_omp) private(v) default(none) 
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        getVertex2D(i, v); 
        heights_omp[i] = f_omp->getRawHeight(v);  
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

void PlanarSurface::pruneBoundingLists() { 
    for ( auto it = upper_bound_.begin(); it != upper_bound_.end(); ++it ) 
    {
        if ( it->expired() ) { 
            upper_bound_.erase(it); 
        }
    }

    for ( auto it = lower_bound_.begin(); it != lower_bound_.end(); ++it ) 
    {
        if ( it->expired() ) { 
            lower_bound_.erase(it); 
        }
    }

    f->pruneBoundingLists(); 
} 

/* Methods */ 
bool PlanarSurface::rangeCheck( Natural i, Natural j ) { 
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


