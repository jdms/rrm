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


#ifndef __INTERPOLATED_GRAPH__
#define __INTERPOLATED_GRAPH__

#include <algorithm>
#include <vector> 
#include <list> 
#include <set>
#include <memory>

#include "use_openmp.hpp"

#include "interpolant_2d.hpp" 
#include "core.hpp" 
#include "triangle_soup_wrapper.hpp" 

/* Class InterpolatedGraph is not reentrant. */ 

class InterpolatedGraph 
{
    public: 
        using Ptr = std::shared_ptr<InterpolatedGraph>; 
        using WeakPtr = std::weak_ptr<InterpolatedGraph>; 

        InterpolatedGraph( bool extruded_surface = false ); 

        InterpolatedGraph( const InterpolatedGraph& ); 
        InterpolatedGraph& operator=( const InterpolatedGraph& ) = delete; 

        InterpolatedGraph( InterpolatedGraph&& ); 
        InterpolatedGraph& operator=( InterpolatedGraph&& ) = delete; 

        bool surfaceIsSet(); 
        unsigned long int getID() const; 
        bool checkIfDependsOn( unsigned long int surface_id ); 

        bool getHeight( const Point2 &p, double &height ); 
        bool getHeight( Point2 &&p, double &height ); 
        bool getHeight( Point3 &p ); 

        bool getRawHeight( const Point2 &p, double &height );
        double getPathOrdinate( double abscissa );

        template<typename CoordinatesList2D>
        double getRawHeight( CoordinatesList2D &&vertex ); 

        template<typename CoordinatesList>
        bool getRawHeightMap( CoordinatesList &vertices ); 

        int isSmooth(); 

        template<typename T>
        bool getNormal( const Point2 &p, T& normal ); 
        template<typename T>
        bool getNormal( Point2 &&p, T& normal ); 

        bool addPoint( const Point3 &p ); 
        bool addPoint( Point3 &&p ); 
        bool addPoints( const std::vector<Point3> &points ); 

        template<typename Point3Type>
        bool addPoint( Point3Type &&p ); 

        template<typename Point3Type>
        bool addPoints( const std::vector<Point3Type> &points); 

        bool addExtrusionPathPoint( double abscissa, double ordinate );
        void setPathOrigin( double abscissa, double ordinate = 0. );

        bool generateSurface(); 

        /* bool getMesh( BoundBox bbox, MeshType *mesh ); */ 
        /* bool getMesh( std::vector<BoundBox> &bboxes, MeshType *mesh ); */ 

        bool liesAbove( const Point3 &p ); 
        bool liesAbove( Point3 &&p ); 
        bool liesBelow( const Point3 &p ); 
        bool liesBelow( Point3 &&p ); 

        bool liesAboveRawSurface( const Point3 &p ); 
        bool liesAboveRawSurface( Point3 &&p ); 
        bool liesBelowRawSurface( const Point3 &p ); 
        bool liesBelowRawSurface( Point3 &&p ); 

        template<typename T>
        void getDependencyList( T &list ) const; 

        template<typename T>
        void getUpperBoundList( T &list ) const;

        template<typename T>
        void getLowerBoundList( T &list ) const; 

        bool removeAbove( InterpolatedGraph::Ptr &s ); 
        bool removeAbove( InterpolatedGraph::WeakPtr s ); 

        bool removeBelow( InterpolatedGraph::Ptr &s ); 
        bool removeBelow( InterpolatedGraph::WeakPtr s ); 

        bool project( Point3 &p ); 

        void pruneBoundingLists(); 
        void clearBoundingLists(); 

        bool isExtrudedSurface(); 
        bool isPathExtrudedSurface();

    private: 
        static unsigned long int num_instances_; 
        const unsigned long int id_; 

        bool surface_is_set_ = false; 
        bool path_is_set_ = false;

        Interpolant2D f; 

        Interpolant2D path;
        Point2 path_origin;

        std::list< std::weak_ptr<InterpolatedGraph> > upper_bound_; 
        std::list< std::weak_ptr<InterpolatedGraph> > lower_bound_; 
        std::set<unsigned long int> dependency_list_; 

        bool extruded_surface_ = false; 

        bool compareSurfaceWptr( const InterpolatedGraph::WeakPtr &left, const InterpolatedGraph::WeakPtr &right ) const;
};

template<typename Point3Type>
bool InterpolatedGraph::addPoint( Point3Type &&p ) 
{
    Point2 p2; 
    if ( isExtrudedSurface() == true ) { 
        assign( p2, {p[0], 0.0} ); 
    }
    else {
        assign( p2, {p[0], p[1]} ); 
    }

    return f.addPointEvaluation(p2, p[2]); 
} 

template<typename Point3Type>
bool InterpolatedGraph::addPoints( const std::vector<Point3Type> &points) 
{
    bool status = true; 
    for ( auto &p : points ) 
    {
        status &= addPoint(p); 
    }

    return status;
}

template<typename T>
bool InterpolatedGraph::getNormal( const Point2 &p, T& normal ) 
{
    if ( isSmooth() < 1 ) { 
        return false; 
    }

    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    double height, DxF, DyF, normDF;  
    bool status; 

    status = getHeight(p, height); 

    if ( isExtrudedSurface() )
    {
        if ( path_is_set_ )
        {
            double origin = path(path_origin.x, 0) - path_origin.y;
            /* height = f(p.x - (path(p.y, 0) - origin), 0); */

            DxF = f.Dx(p.x - (path(p.y,0) - origin), 0);
            DyF = f.Dx(p.x - (path(p.y,0) - origin), 0) * path.Dx(p.y,0);
        }

        else
        {
            DxF = f.Dx(p.x, 0); 
            DyF = 0; 
        }
    }

    else
    {
        DxF = f.Dx(p.x, p.y); 
        DyF = f.Dy(p.x, p.y); 
    }
    normDF = DxF*DxF + DyF*DyF + 1; 

    normal[0] = -DxF/normDF; 
    normal[1] = -DyF/normDF; 
    normal[2] = 1/normDF; 

    return status;
}

template<typename T>
bool InterpolatedGraph::getNormal( Point2 &&p, T& normal ) 
{
    return getNormal(p, normal);
}

template<typename CoordinatesList2D>
double InterpolatedGraph::getRawHeight( CoordinatesList2D &&vertex ) 
{
    /* double height = 0.0; */ 
    /* if ( isExtrudedSurface() == true ) { */ 
    /*     height = f(vertex[0], 0.0); */ 
    /* } */
    /* else { */ 
    /*     height = f(vertex[0], vertex[1]); */  
    /* } */

    /* return height; */  

    Point2 p2; 
    p2.x = vertex[0]; 
    p2.y = vertex[1]; 
    double height; 

    if ( getRawHeight(p2, height ) == false ) { 
        return false; 
    }

    return height; 
}


template<typename CoordinatesList>
bool InterpolatedGraph::getRawHeightMap( CoordinatesList &vertices ) 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    // TODO: substitute the calls to TriangleSoupWrapper "mesh.[get,set]Vertex" by simple lambdas 
    using FaceList = std::vector<size_t>; 

    // The use of the TriangleSoupWrapper here is deprecated and considered a bug. 
    FaceList unused_flist; 
    TriangleSoupWrapper<CoordinatesList, FaceList> mesh(vertices, unused_flist); 

    size_t size = mesh.nvertices(); 
    Point3 p; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(mesh) firstprivate(size) private(p) default(none)
    for ( long int i = 0; i < static_cast<long int>(size); ++i )
    {
        if ( mesh.getVertex(i, p) == true ) { 

            /* if ( isExtrudedSurface() == true ) { */ 
            /*     p.z = f(p.x, 0.0); */ 
            /* } */
            /* else { */ 
            /*     p.z = f(p.x, p.y); */  
            /* } */

            /* p.z = f(p.x, p.y); */ 
            /* std::cout << "p.x = " << p.x << ", p.y = " << p.y << ", p.z = " << p.z << std::endl; */ //debug
            getRawHeight( {{{ p.x, p.y }}}, p.z );
            mesh.setVertex(i, p); 
        }
    }

    return true; 
}

template<typename T>
void InterpolatedGraph::getDependencyList( T &list ) const 
{
    list.insert(dependency_list_.begin(), dependency_list_.end()); 
}

template<typename T>
void InterpolatedGraph::getUpperBoundList( T &list ) const
{
    list.insert( list.end(), upper_bound_.begin(), upper_bound_.end() ); 
    std::unique( list.begin(), list.end(), 
            [this] ( InterpolatedGraph::WeakPtr &l, InterpolatedGraph::WeakPtr &r)  -> bool 
            { 
                return this->compareSurfaceWptr(l, r); 
            } 
        ); 
}

template<typename T>
void InterpolatedGraph::getLowerBoundList( T &list ) const
{
    list.insert( list.end(), lower_bound_.begin(), lower_bound_.end() ); 
    std::unique( list.begin(), list.end(), 
            [this] ( InterpolatedGraph::WeakPtr &l, InterpolatedGraph::WeakPtr &r)  -> bool 
            { 
                return this->compareSurfaceWptr(l, r); 
            } 
        ); 
}

#endif

