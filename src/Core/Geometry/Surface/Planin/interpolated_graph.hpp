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


#ifndef __INTERPOLATED_GRAPH__
#define __INTERPOLATED_GRAPH__

#ifdef __APPLE__
#include <libiomp/omp.h>
#else
#include <omp.h>
#endif

#include <vector> 
#include <list> 
#include <set>
#include <memory>
#include "interpolant_2d.hpp" 
#include "core.hpp" // Dummy definitions to be replaced by the projects definitions. 
#include "triangle_soup_wrapper.hpp" 


/* Class InterpolatedGraph is not reentrant. */ 

class InterpolatedGraph 
{
    public: 
        using Ptr = std::shared_ptr<InterpolatedGraph>; 
        using WeakPtr = std::weak_ptr<InterpolatedGraph>; 

        InterpolatedGraph(); 

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

        template<typename __Point3__>
        bool addPoint( __Point3__ &&p ); 

        template<typename __Point3__>
        bool addPoints( const std::vector<__Point3__> &points); 

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

        bool removeAbove( InterpolatedGraph::Ptr &s ); 
        bool removeAbove( InterpolatedGraph::WeakPtr s ); 

        bool removeBelow( InterpolatedGraph::Ptr &s ); 
        bool removeBelow( InterpolatedGraph::WeakPtr s ); 

        bool project( Point3 &p ); 

        void pruneBoundingLists(); 

    private: 
        bool surface_is_set_ = false; 
        Interpolant2D f; 
        std::list< std::weak_ptr<InterpolatedGraph> > upper_bound_; 
        std::list< std::weak_ptr<InterpolatedGraph> > lower_bound_; 
        std::set<unsigned long int> dependency_list_; 

        static unsigned long int num_instances_; 
        const unsigned long int id_; 
};

template<typename __Point3__>
bool InterpolatedGraph::addPoint( __Point3__ &&p ) 
{
    Point2 p2; 
    assign(p2, {p[0], p[1]}); 

    return f.addPointEvaluation(p2, p[2]); 
} 

template<typename __Point3__>
bool InterpolatedGraph::addPoints( const std::vector<__Point3__> &points) 
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
    DxF = f.Dx(p.x, p.y); 
    DyF = f.Dy(p.x, p.y); 
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
    return f(vertex[0], vertex[1]); 
}


template<typename CoordinatesList>
bool InterpolatedGraph::getRawHeightMap( CoordinatesList &vertices ) 
{
    if ( surfaceIsSet() == false ) { 
        return false; 
    }

    using FaceList = std::vector<size_t>; 
    TriangleSoupWrapper<CoordinatesList, FaceList> mesh(vertices); 

    size_t size = mesh.nvertices(); 
    Point3 p; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(mesh) firstprivate(size) private(p) default(none)
    for ( long int i = 0; i < static_cast<long int>(size); ++i )
    {
        mesh.getVertex(i, p); 
        p.z = f(p.x, p.y); 
        /* std::cout << "p.x = " << p.x << ", p.y = " << p.y << ", p.z = " << p.z << std::endl; */ //debug
        mesh.setVertex(i, p); 
    }

    return true; 
}

template<typename T>
void InterpolatedGraph::getDependencyList( T &list ) const 
{
    list.insert(dependency_list_.begin(), dependency_list_.end()); 
}

#endif

