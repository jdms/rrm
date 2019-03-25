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


#include "interpolated_graph.hpp" 
#include "triangle_soup_wrapper.hpp" 

unsigned long int InterpolatedGraph::num_instances_ = 0; 

InterpolatedGraph::InterpolatedGraph( bool extruded_surface, bool orthogonally_oriented ) : 
    id_(num_instances_), 
    f( Kernel(), (extruded_surface == true ? 2 : 3) ),
    path( Kernel(), (extruded_surface == true ? 2 : 3) )
{ 
    ++num_instances_;
    dependency_list_.insert(id_); 
    extruded_surface_ = extruded_surface; 
    orthogonally_oriented_ = orthogonally_oriented;

    path_origin = {{{ 0., 0. }}};
}

InterpolatedGraph::InterpolatedGraph( const InterpolatedGraph &rhs ) : id_(num_instances_) 
{
    ++num_instances_; 

    surface_is_set_ = rhs.surface_is_set_; 

    f = rhs.f; 
    path = rhs.path;

    upper_bound_ = rhs.upper_bound_; 
    lower_bound_ = rhs.lower_bound_; 

    dependency_list_ = rhs.dependency_list_; 
    auto iter = dependency_list_.find(rhs.id_); 
    dependency_list_.erase(iter); 
    dependency_list_.insert(id_); 

    extruded_surface_ = rhs.extruded_surface_; 
    orthogonally_oriented_ = rhs.orthogonally_oriented_; 

    path_is_set_ = rhs.path_is_set_;
    path_origin = rhs.path_origin;
}

InterpolatedGraph::InterpolatedGraph( InterpolatedGraph &&rhs ) : id_(rhs.id_) 
{ 
    surface_is_set_ = rhs.surface_is_set_; 

    f = std::move(rhs.f); 
    path = std::move(rhs.path);

    upper_bound_ = std::move(rhs.upper_bound_); 
    lower_bound_ = std::move(rhs.lower_bound_); 

    dependency_list_ = std::move(rhs.dependency_list_); 

    extruded_surface_ = rhs.extruded_surface_; 
    orthogonally_oriented_ = rhs.orthogonally_oriented_; 

    path_is_set_ = rhs.path_is_set_;
    path_origin = rhs.path_origin;
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

bool InterpolatedGraph::getRawHeight( const Point2 &p, double &height )
{
    if ( surfaceIsSet() == false ) 
    { 
        return false; 
    }

    if ( isExtrudedSurface() == true )  
    { 
        if ( !orthogonally_oriented_ ) // i.e. surfaces are extruded along the y direction
        {
            if ( path_is_set_ )
            {
                double origin = path(path_origin.x, 0) - path_origin.y;
                height = f(p.x - (path(p.y, 0) - origin), 0);
            }

            else
            {
                height = f(p.x, 0.0); 
            }
        }
        else // if ( orthogonally_oriented_ ) // i.e. surfaces are extruded along the x direction
        {
            if ( path_is_set_ )
            {
                double origin = path(path_origin.x, 0) - path_origin.y;
                height = f(p.y - (path(p.x, 0) - origin), 0);
            }

            else
            {
                height = f(p.y, 0.0); 
            }
        }
    }
    else 
    { 
        height = f(p.x, p.y);  
    }

    return true;
}

double InterpolatedGraph::getPathOrdinate( double abscissa )
{
    return path(abscissa, 0);
}

bool InterpolatedGraph::getBoundedHeight( const Point2 &p, double &height, double ubound, double lbound )
{
    if ( getRawHeight(p, height) == false )
    {
        return false;
    }
    
    if ( height > ubound ) { 
        height = ubound; 
    }
    else if ( height < lbound ) { 
        height = lbound; 
    }

    double upper_bound, lower_bound;  

    bool status = true; 
    for ( auto &p_upper_bound_ : upper_bound_ )
        if ( auto upper_surface = p_upper_bound_.lock() ) { 
            if ( upper_surface->surfaceIsSet() ) {
                 upper_surface->getBoundedHeight(p, upper_bound, ubound, lbound); 
                if ( height >= upper_bound ) { 
                    height = upper_bound; 
                    status &= false; 
                }
            }
        }

    for ( auto &p_lower_bound_ : lower_bound_ )
        if ( auto lower_surface = p_lower_bound_.lock() ) { 
            if ( lower_surface->surfaceIsSet() ) { 
                lower_surface->getBoundedHeight(p, lower_bound, ubound, lbound); 
                if ( height <= lower_bound ) { 
                    height = lower_bound; 
                    status &= false; 
                }
            }
        }

    return status; 
}

bool InterpolatedGraph::getBoundedHeight( Point2 &&p, double &height, double ubound, double lbound ) 
{
    return getBoundedHeight(p, height, ubound, lbound); 
}

bool InterpolatedGraph::getBoundedHeight( Point3 &p, double ubound, double lbound ) 
{
    Point2 p2;
    p2.x = p.x; 
    p2.y = p.y;

    return getBoundedHeight(p2, p.z, ubound, lbound); 
}

bool InterpolatedGraph::getHeight( const Point2 &p, double &height ) 
{
    /* if ( surfaceIsSet() == false ) { */ 
    /*     return false; */ 
    /* } */

    /* if ( isExtrudedSurface() == true ) { */ 
    /*     height = f(p.x, 0.0); */ 
    /* } */
    /* else { */ 
    /*     height = f(p.x, p.y); */  
    /* } */

    if ( getRawHeight(p, height) == false )
    {
        return false;
    }

    double upper_bound, lower_bound;  

    bool status = true; 
    for ( auto &p_upper_bound_ : upper_bound_ )
        if ( auto upper_surface = p_upper_bound_.lock() ) { 
            if ( upper_surface->surfaceIsSet() ) {
                 upper_surface->getHeight(p, upper_bound); 
                if ( height >= upper_bound ) { 
                    height = upper_bound; 
                    status &= false; 
                }
            }
        }

    for ( auto &p_lower_bound_ : lower_bound_ )
        if ( auto lower_surface = p_lower_bound_.lock() ) { 
            if ( lower_surface->surfaceIsSet() ) { 
                lower_surface->getHeight(p, lower_bound); 
                if ( height <= lower_bound ) { 
                    height = lower_bound; 
                    status &= false; 
                }
            }
        }

    return status; 
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

void InterpolatedGraph::setPathOrigin( double abscissa, double ordinate )
{
    path_origin.x = abscissa;
    path_origin.y = ordinate;
}

bool InterpolatedGraph::addExtrusionPathPoint( double abscissa, double ordinate )
{
    if ( surfaceIsSet() == true )
    {
        return false;
    }

    if ( isExtrudedSurface() == false )
    {
        return false;
    }


    path.addPointEvaluation( {{{ abscissa, 0 }}}, ordinate );

    return true;
}

bool InterpolatedGraph::addPoint( const Point3 &p ) 
{
    if ( surfaceIsSet() == true ) { 
        return false; 
    }

    Point2 p2; 

    if ( isExtrudedSurface() == true ) { 
        p2.x = p.x; p2.y = 0.0; 
    }
    else { 
        p2.x = p.x; p2.y = p.y; 
    }

    return f.addPointEvaluation(p2, p.z); 
}

bool InterpolatedGraph::addPoint( Point3 &&p ) 
{
    return addPoint(p); 
} 

bool InterpolatedGraph::addPoints( const std::vector<Point3> &points ) 
{
    if ( surfaceIsSet() == true ) { 
        return false; 
    }

    bool status = true; 

    for ( auto &p : points ) { 
        status &= addPoint(p); 
    }

    return status; 
} 

bool InterpolatedGraph::setFillDistance( double value )
{
    return f.setFillDistance(value);
}

double InterpolatedGraph::getFillDistance()
{
    return f.getFillDistance();
}

bool InterpolatedGraph::generateSurface() 
{
    if ( surfaceIsSet() == true ) { 
        return false; 
    }

    surface_is_set_ = f.interpolate(); 

    if ( surface_is_set_ && isExtrudedSurface() )
    {
        path_is_set_ = path.interpolate();
    }

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

    getHeight(p2, height );

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

    getHeight(p2, height );

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

bool InterpolatedGraph::liesAboveRawSurface( const Point3 &p ) 
{ 

/*     if ( surfaceIsSet() == false ) { */ 
/*         return false; */ 
/*     } */

/*     double height = 0.0; */ 
/*     if ( isExtrudedSurface() == true ) { */ 
/*         height = f(p.x, 0.0); */ 
/*     } */
/*     else { */ 
/*         height = f(p.x, p.y); */  
/*     } */

/*     if ( p.z < height ) { */ 
/*         return false; */ 
/*     } */

/*     return true; */ 
 
    Point2 p2; 
    p2.x = p.x; 
    p2.y = p.y; 
    double height {}; 

    getRawHeight(p2, height);

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
/*     if ( surfaceIsSet() == false ) { */ 
/*         return false; */ 
/*     } */

/*     double height = 0.0; */ 
/*     if ( isExtrudedSurface() == true ) { */ 
/*         height = f(p.x, 0.0); */ 
/*     } */
/*     else { */ 
/*         height = f(p.x, p.y); */  
/*     } */

/*     if ( p.z < height ) { */ 
/*         return false; */ 
/*     } */

/*     if ( p.z > height ) { */ 
/*         return false; */ 
/*     } */

/*     return true; */ 
 
    Point2 p2; 
    p2.x = p.x; 
    p2.y = p.y; 
    double height {}; 

    getRawHeight(p2, height );

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

    // upper_bound_.push_back( std::weak_ptr<InterpolatedGraph>(s) );  
    // s->getLowerBoundList(lower_bound_); 
    
    upper_bound_.push_back(std::weak_ptr<InterpolatedGraph>(s));

    pruneBoundingLists();

    std::list< std::weak_ptr<InterpolatedGraph> > lbound{};
    s->getLowerBoundList(lbound);

    bool new_bound = true;

    for ( auto &lb : lbound )
    {
        new_bound = true;
        for ( auto &ub : upper_bound_ )
        {
            new_bound &= (compareSurfaceWptr(lb, ub) == false);
        }

        if ( new_bound )
        {
            lower_bound_.push_back(lb);
        }

    }

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

    // lower_bound_.push_back( std::weak_ptr<InterpolatedGraph>(s) );  
    // s->getUpperBoundList(upper_bound_); 

    lower_bound_.push_back(std::weak_ptr<InterpolatedGraph>(s));

    pruneBoundingLists();

    std::list< std::weak_ptr<InterpolatedGraph> > ubound{};
    s->getUpperBoundList(ubound);

    bool new_bound = true;

    for (auto &ub : ubound)
    {
        new_bound = true;
        for (auto &lb : lower_bound_)
        {
            new_bound &= (compareSurfaceWptr(ub, lb) == false);
        }

        if (new_bound)
        {
            upper_bound_.push_back(ub);
        }

    }

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
}

void InterpolatedGraph::clearBoundingLists()
{
    upper_bound_.clear(); 
    lower_bound_.clear(); 
}

bool InterpolatedGraph::isExtrudedSurface() 
{ 
    return extruded_surface_;
} 

bool InterpolatedGraph::isPathExtrudedSurface()
{
    return path_is_set_;
}

bool InterpolatedGraph::isOrthogonallyOrientedSurface()
{
    return orthogonally_oriented_;
}

bool InterpolatedGraph::compareSurfaceWptr( const InterpolatedGraph::WeakPtr &left, const InterpolatedGraph::WeakPtr &right ) const
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

bool InterpolatedGraph::getRawData( std::vector<Point2> &points, std::vector<double> &fevals )
{
    return f.getRawData(points, fevals);
}

bool InterpolatedGraph::getRawPathData( std::vector<double> &abscissas, std::vector<double> &ordinates )
{
    if ( !path_is_set_ )
    {
        return false;
    }

    std::vector<Point2> points;
    std::vector<double> fevals;

    if ( !path.getRawData(points, fevals) )
    {
        return false;
    }

    size_t num_points = points.size();

    abscissas.resize(num_points);
    ordinates.resize(num_points);

    for ( size_t i = 0; i < num_points; ++i )
    {
        abscissas[i] = points[i].x;
        ordinates[i] = fevals[i];
    }

    return true;
}
