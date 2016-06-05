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


#include "interpolated_graph.hpp" 
#include "triangle_soup_wrapper.hpp" 

#ifdef __APPLE__
#include <libiomp/omp.h>
#endif

unsigned long int InterpolatedGraph::num_instances_ = 0; 

InterpolatedGraph::InterpolatedGraph() : id_(num_instances_) 
{ 
    ++num_instances_;
    dependency_list_.insert(id_); 
}

InterpolatedGraph::InterpolatedGraph( const InterpolatedGraph &rhs ) : id_(num_instances_) 
{
    ++num_instances_; 

    surface_is_set_ = rhs.surface_is_set_; 
    f = rhs.f; 
    upper_bound_ = rhs.upper_bound_; 
    lower_bound_ = rhs.lower_bound_; 
    dependency_list_ = rhs.dependency_list_; 
}

InterpolatedGraph::InterpolatedGraph( InterpolatedGraph &&rhs ) : id_(rhs.id_) 
{ 
    surface_is_set_ = rhs.surface_is_set_; 
    f = std::move(rhs.f); 
    upper_bound_ = std::move(rhs.upper_bound_); 
    lower_bound_ = std::move(rhs.lower_bound_); 

    dependency_list_ = std::move(rhs.dependency_list_); 
}

bool InterpolatedGraph::surfaceIsSet() 
{
    return surface_is_set_; 
}

unsigned long int InterpolatedGraph::getID() const 
{
    return id_; 
}

bool InterpolatedGraph::checkIfDependsOn( unsigned long int surface_id ) { 

    if ( dependency_list_.find(surface_id) == dependency_list_.end() ) { 
        return false; 
    }
    
    return true;
}
 
int InterpolatedGraph::isSmooth() 
{
    return f.isSmooth(); 
}

bool InterpolatedGraph::getHeight( const Point2 &p, double &height ) 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    height = f(p.x, p.y);  
    double upper_bound, lower_bound;  

    for ( auto &p_upper_bound_ : upper_bound_ )
    if ( auto upper_surface = p_upper_bound_.lock() ) { 
        if ( upper_surface->getHeight(p, upper_bound) )
        if ( height > upper_bound ) { 
            return false; 
        }
    }

    for ( auto &p_lower_bound_ : lower_bound_ )
    if ( auto lower_surface = p_lower_bound_.lock() ) { 
        if ( lower_surface->getHeight(p, lower_bound) )  
        if ( height < lower_bound ) { 
            return false; 
        }
    }

    return true; 
}

bool InterpolatedGraph::getHeight( Point2 &&p, double &height ) 
{
    return getHeight(p, height); 
}

bool InterpolatedGraph::getHeight( Point3 &p ) 
{
    Point2 p2;
    p2.x = p.x; 
    p2.y = p.y; 

    return getHeight(p2, p.z); 
}

bool InterpolatedGraph::addPoint( const Point3 &p ) 
{
    if ( surfaceIsSet() == true ) { 
        return false; 
    }

    Point2 p2; 
    p2.x = p.x; p2.y = p.y; 

    return f.addPointEvaluation(p2, p.z); 
}
 
bool InterpolatedGraph::addPoint( Point3 &&p ) 
{
    return addPoint(p); 
} 

bool InterpolatedGraph::addPoints( const std::vector<Point3> &points ) 
{
    bool status = true; 

    for ( auto &p : points ) { 
        status &= addPoint(p); 
    }

    return status; 
} 

bool InterpolatedGraph::generateSurface() 
{
    if ( surfaceIsSet() == true ) { 
        return false; 
    }

    surface_is_set_ = f.interpolate(); 
    return surface_is_set_; 
} 

/* bool InterpolatedGraph::getMesh( BoundBox bbox, MeshType *mesh ) */ 
/* { */
/*     UNUSED(bbox); */ 
/*     UNUSED(mesh); */ 

/*     return false; */ 
/* } */ 
/* bool InterpolatedGraph::getMesh( std::vector<BoundBox> &bboxes, MeshType *mesh ) */ 
/* { */
/*     UNUSED(bboxes); */ 
/*     UNUSED(mesh); */ 

/*     return false; */ 
/* } */

bool InterpolatedGraph::liesAbove( const Point3 &p ) { 
    Point2 p2; 
    p2.x = p.x; 
    p2.y = p.y; 
    double height; 

    // Check whether p lies in a region for which the surface is well defined 
    if ( getHeight(p2, height ) == false ) { 
        return false; 
    }

    if ( p.z < height ) { 
        return false; 
    }

    return true; 

}

bool InterpolatedGraph::liesAbove( Point3 &&p ) 
{ 
    return liesAbove(p); 
} 

bool InterpolatedGraph::liesBelow( const Point3 &p ) 
{ 
    Point2 p2; 
    p2.x = p.x; 
    p2.y = p.y; 
    double height; 

    // Check whether p lies in a region for which the surface is well defined 
    if ( getHeight(p2, height ) == false ) { 
        return false; 
    }

    if ( p.z > height ) { 
        return false; 
    }

    return true; 
} 

bool InterpolatedGraph::liesBelow( Point3 &&p ) 
{ 
    return liesBelow(p); 
} 

/* 1 */ 

bool InterpolatedGraph::liesAboveRawSurface( const Point3 &p ) { 

    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    double height = f(p.x, p.y); 
    if ( p.z < height ) { 
        return false; 
    }

    return true; 

}

bool InterpolatedGraph::liesAboveRawSurface( Point3 &&p ) 
{ 
    return liesAboveRawSurface(p); 
} 

bool InterpolatedGraph::liesBelowRawSurface( const Point3 &p ) 
{ 
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    double height = f(p.x, p.y); 
    if ( p.z > height ) { 
        return false; 
    }

    return true; 
} 

bool InterpolatedGraph::liesBelowRawSurface( Point3 &&p ) 
{ 
    return liesBelowRawSurface(p); 
} 

/* 2 */

bool InterpolatedGraph::removeAbove( InterpolatedGraph::Ptr &s ) 
{ 
    unsigned long int my_id = getID(); 
    if ( s->checkIfDependsOn( my_id ) == true ) { 
        return false; 
    }

    s->getDependencyList( dependency_list_ ); 

    upper_bound_.push_back( std::weak_ptr<InterpolatedGraph>(s) );  
    return true; 
} 

bool InterpolatedGraph::removeAbove( InterpolatedGraph::WeakPtr s ) 
{ 
    auto sptr = s.lock(); 
    return removeAbove(sptr); 
} 

bool InterpolatedGraph::removeBelow( InterpolatedGraph::Ptr &s ) 
{ 
    unsigned long int my_id = getID(); 
    if ( s->checkIfDependsOn( my_id ) == true ) { 
        return false; 
    }

    s->getDependencyList( dependency_list_ ); 

    lower_bound_.push_back( std::weak_ptr<InterpolatedGraph>(s) );  
    return true; 
} 

bool InterpolatedGraph::removeBelow( InterpolatedGraph::WeakPtr s ) 
{ 
    auto sptr = s.lock(); 
    return removeBelow(sptr); 
} 

bool InterpolatedGraph::project( Point3 &p ) 
{
    Point2 p2; 
    p2.x = p.x; 
    p2.y = p.y; 

    if ( getHeight(p2, p.z) == false ) { 
        return false; 
    }

    return true; 
}

void InterpolatedGraph::pruneBoundingLists() 
{
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
}


