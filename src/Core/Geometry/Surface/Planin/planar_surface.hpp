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


#ifndef __PLANAR_SURFACE__
#define __PLANAR_SURFACE__

#ifdef __APPLE__
#include <libiomp/omp.h>
#else 
#include <omp.h>
#endif

#include <array>
#include <vector> 
#include <list> 
#include <memory> 

#include "interpolated_graph.hpp"
#include "triangle_soup_wrapper.hpp" 

/* Class PlanarSurface is not reentrant. */ 

/* template<typename CoordinatesListType = std::vector<float>, typename Natural = unsigned long int> */ 
class PlanarSurface { 
    public: 
        /* using CoordinatesList = CoordinatesListType; */ 
        using Ptr = std::shared_ptr<PlanarSurface>; 
        using WeakPtr = std::weak_ptr<PlanarSurface>; 
        /* using NaturalType = Natural; */ 
        using Natural = unsigned long int; 

        using IndicesType = std::array<Natural, 2>; 

        PlanarSurface(); 

        PlanarSurface( const PlanarSurface & ); 
        PlanarSurface& operator=( const PlanarSurface & ) = delete; 

        PlanarSurface( PlanarSurface && ); 
        PlanarSurface& operator=( PlanarSurface && ) = delete; 


        /* Methods */ 

        /* Setters. */ 
        bool addPoint( const Point3 &p ); 
        bool addPoint( Point3 &&p ); 
        bool addPoints( const std::vector<Point3> &points ); 

        template<typename __Point3__>
        bool addPoint( __Point3__ &&p ); 

        template<typename __Point3__>
        bool addPoints( const std::vector<__Point3__> &points); 

        void setOrigin( const Point2 &o ); 
        void setOrigin( Point2 &&o ); 

        bool setLenght( const Point2 &l ); 
        bool setLenght( Point2 &&l ); 

        /* Create surface. */ 
        bool generateSurface(); 
        bool updateHeights(); 

        void updateDiscretization(); 
        void requestChangeDiscretization( Natural numX, Natural numY ); 

        /* Geta mesh, a vertex list or just the surface's connectivity. */ 
        template<typename VList>
        bool getVertexList( VList &vlist );  

        template<typename FList>
        unsigned int getFaceList( FList &flist ); 

        template<typename VList, typename FList> 
        unsigned int getMesh( VList &vertex_list, FList &face_list ); 

        template<typename NList>
        bool getNormalList( NList &normal_list ); 

        /* Basic methods. */ 
        bool surfaceIsSet();
        unsigned long int getID() const; 
        bool checkIfDependsOn( unsigned long int surface_id ); 

        bool rangeCheck( Natural vertex );

        bool weakEntireSurfaceCheck(); 
        bool weakBoundedEntireSurfaceCheck( PlanarSurface::Ptr &lower_surface, PlanarSurface::Ptr &upper_surface ); 
        bool weakBoundedEntireSurfaceCheck( PlanarSurface::WeakPtr lower_surface, PlanarSurface::WeakPtr upper_surface ); 

        bool weakIntersectionCheck( PlanarSurface::Ptr &s );
        bool weakIntersectionCheck( PlanarSurface::WeakPtr s ); 

        bool weakLiesAboveCheck( PlanarSurface::Ptr &s ); 
        bool weakLiesAboveCheck( PlanarSurface::WeakPtr s ); 

        bool weakLiesBelowCheck( PlanarSurface::Ptr &s ); 
        bool weakLiesBelowCheck( PlanarSurface::WeakPtr s ); 

        bool getVertex2D( Natural index, Point2 &v );
        bool getVertex3D( Natural index, Point3 &v );

        bool getHeight( Natural vertex_index, double &height );

        bool getHeight( const Point2 &p, double &height );
        bool getHeight( Point2 &&p, double &height );

        template<typename CoordinatesList>
        bool getRawHeightMap( CoordinatesList &vertices );

        template<typename T>
        bool getNormal( const Point2 &p, T& normal ); 
        template<typename T>
        bool getNormal( Point2 &&p, T& normal ); 

        InterpolatedGraph::WeakPtr get_interpolant();

        double getTolerance(); 

        /* bool getMesh( BoundBox bbox, MeshType *mesh ); */ 
        /* bool getMesh( std::vector<BoundBox> &bboxes, MeshType *mesh ); */ 

        Point2 getOrigin(); 
        Point2 getLenght(); 

        bool liesAbove( const Point3 &p ); 
        bool liesAbove( Point3 &&p ); 
        bool liesAboveRawSurface( const Point3 &p ); 
        bool liesAboveRawSurface( Point3 &&p ); 

        bool liesBelow( const Point3 &p ); 
        bool liesBelow( Point3 &&p ); 
        bool liesBelowRawSurface( const Point3 &p ); 
        bool liesBelowRawSurface( Point3 &&p ); 

        bool project( Point3 &p ); 

        /* Geologic rules */ 
        template<typename T>
        void getDependencyList( T &list ) const; 

        bool removeAbove( PlanarSurface::Ptr &s ); 
        bool removeAbove( PlanarSurface::WeakPtr s ); 

        bool removeBelow( PlanarSurface::Ptr &s ); 
        bool removeBelow( PlanarSurface::WeakPtr s ); 

        /* Maintenance */ 
        void pruneBoundingLists(); 

    private:
        /* Members 'discretization_X' and 'discretization_Y' are supposed to 
         * be specified. Everything else should be kept as is. 
         */
        static Natural discretization_X;
        static Natural discretization_Y;
        double tolerance; 


        static unsigned long int global_discretization_state_; 
        unsigned long int this_discretization_state_; 

        Natural nX_; // = 2*discretization_X + 1; 
        Natural nY_; // = 2*discretization_Y + 1; 
        Natural num_vertices_; // = nX_ * nY_; 

        static unsigned long int num_instances_; 
        const unsigned long int id_; 

        InterpolatedGraph::Ptr f;  

        Point2 origin; 
        Point2 lenght; 

        std::vector<double> heights; 
        /* std::vector<bool> valid_heights; */ 

        bool interpolant_is_set_ = false; 

        /* Right now, 'mesh_is_set_' is useless. */ 
        bool mesh_is_set_ = false; 

        std::list< std::weak_ptr<PlanarSurface> > upper_bound_; 
        std::list< std::weak_ptr<PlanarSurface> > lower_bound_; 
        std::set< unsigned long int> dependency_list_; 


        /* Methods */ 
        bool rangeCheck( Natural i, Natural j );
        Natural getVertexIndex( Natural i, Natural j );
        bool getVertexIndices( Natural v, IndicesType &indices ); 
};

template<typename __Point3__>
bool PlanarSurface::addPoint( __Point3__ &&p ) 
{ 
    return f->addPoint(p);
}

template<typename __Point3__>
bool PlanarSurface::addPoints( const std::vector<__Point3__> &points)
{
    return f->addPoints(points);
}

template<typename VList>
bool PlanarSurface::getVertexList( VList &vlist ) 
{
    /* std::cout << "Got here? \n\n"; */ 
    if ( surfaceIsSet() == false ) {
        /* std::cout << "No mesh for you today...\n\n"; */ 
        return false; 
    }

    updateDiscretization(); 

    /* std::cout << "Num vertices: " << num_vertices_ << "\n\n"; */ 
    vlist.resize(3*num_vertices_); 
    Point3 v; 
    auto &vlist_omp = vlist; 
    auto num_vertices_omp = num_vertices_; 

    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(vlist_omp) firstprivate(num_vertices_omp) private(v) default(none)
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        getVertex3D(i, v);
        vlist_omp[3*i + 0] = v.x; 
        vlist_omp[3*i + 1] = v.z;
        vlist_omp[3*i + 2] = v.y;
    }

    return true; 
}

/*
 * *-------*-------*
 * |\v6    |v7    /|v8
 * | \  t5 | t7  / |  
 * |  \    |    /  |  
 * |   \   |   /   |  
 * | t4 \  |  / t6 |  
 * |     \ | /     |  
 * |      \|/      |  
 * *-------*-------*  
 * |v3    /|\v4    |v5
 * |     / | \     |  
 * | t1 /  |  \ t3 |  
 * |   /   |   \   |  
 * |  /    |    \  |  
 * | /  t0 | t2  \ |  
 * |/      |      \|  
 * *-------*-------*
 *  v0      v1      v2
 *
 *  t0 = (v0, v1, v4)
 *  t1 = (v0, v4, v3)
 *  t2 = (v1, v2, v4)
 *  t3 = (v2, v5, v4)
 *  t4 = (v3, v4, v6)
 *  t5 = (v4, v7, v6)
 *  t6 = (v4, v5, v8)
 *  t7 = (v4, v8, v7)
 */ 

template<typename FList>
unsigned int PlanarSurface::getFaceList( FList &flist ) 
{
    if ( surfaceIsSet() == false ) {
        return false; 
    }

    updateDiscretization(); 

    Point3 v0, v1, v2, v3, v4, v5, v6, v7, v8; 
    bool b0, b1, b2, b3, b4, b5, b6, b7, b8; 

    flist.clear(); 

    /* std::cout << "Getting face list: \n"; */ 
    unsigned int face_count = 0; 
    for ( Natural i = 0; i < nX_ - 1; i += 2 ) {
        for ( Natural j = 0; j < nY_ - 1; j += 2 ) {
            b0 = getVertex3D( getVertexIndex(i+0, j+0), v0 ); 
            b1 = getVertex3D( getVertexIndex(i+1, j+0), v1 ); 
            b2 = getVertex3D( getVertexIndex(i+2, j+0), v2 ); 
            b3 = getVertex3D( getVertexIndex(i+0, j+1), v3 ); 
            b4 = getVertex3D( getVertexIndex(i+1, j+1), v4 ); 
            b5 = getVertex3D( getVertexIndex(i+2, j+1), v5 ); 
            b6 = getVertex3D( getVertexIndex(i+0, j+2), v6 ); 
            b7 = getVertex3D( getVertexIndex(i+1, j+2), v7 ); 
            b8 = getVertex3D( getVertexIndex(i+2, j+2), v8 ); 

            // Triangle t0: 
            /* if ( b0 && b1 && b4 ) { */ 
            if ( b0 || b1 || b4 ) { 
                /* std::cout << "Got t0! \n"; */ 
                flist.push_back( getVertexIndex(i+0, j+0) ); 
                flist.push_back( getVertexIndex(i+1, j+0) ); 
                flist.push_back( getVertexIndex(i+1, j+1) ); 
                ++face_count; 
            }

            // Triangle t1:
            /* if ( b0 && b4 && b3 ) { */
            if ( b0 || b4 || b3 ) {
                /* std::cout << "Got t1! \n"; */
                flist.push_back( getVertexIndex(i+0, j+0) );
                flist.push_back( getVertexIndex(i+1, j+1) );
                flist.push_back( getVertexIndex(i+0, j+1) );
                ++face_count;
            }

            // Triangle t2:
            /* if ( b1 && b2 && b4 ) { */
            if ( b1 || b2 || b4 ) {
                /* std::cout << "Got t2! \n"; */
                flist.push_back( getVertexIndex(i+1, j+0) );
                flist.push_back( getVertexIndex(i+2, j+0) );
                flist.push_back( getVertexIndex(i+1, j+1) );
                ++face_count;
            }

            // Triangle t3:
            /* if ( b2 && b5 && b4 ) { */
            if ( b2 || b5 || b4 ) {
                /* std::cout << "Got t3! \n"; */
                flist.push_back( getVertexIndex(i+2, j+0) );
                flist.push_back( getVertexIndex(i+2, j+1) );
                flist.push_back( getVertexIndex(i+1, j+1) );
                ++face_count;
            }

            // Triangle t4:
            /* if ( b3 && b4 && b6 ) { */
            if ( b3 || b4 || b6 ) {
                /* std::cout << "Got t4! \n"; */
                flist.push_back( getVertexIndex(i+0, j+1) );
                flist.push_back( getVertexIndex(i+1, j+1) );
                flist.push_back( getVertexIndex(i+0, j+2) );
                ++face_count;
            }

            // Triangle t5:
            /* if ( b4 && b7 && b6 ) { */
            if ( b4 || b7 || b6 ) {
                /* std::cout << "Got t5! \n"; */
                flist.push_back( getVertexIndex(i+1, j+1) );
                flist.push_back( getVertexIndex(i+1, j+2) );
                flist.push_back( getVertexIndex(i+0, j+2) );
                ++face_count;
            }

            // Triangle t6:
            /* if ( b4 && b5 && b8 ) { */
            if ( b4 || b5 || b8 ) {
                /* std::cout << "Got t6! \n"; */
                flist.push_back( getVertexIndex(i+1, j+1) );
                flist.push_back( getVertexIndex(i+2, j+1) );
                flist.push_back( getVertexIndex(i+2, j+2) );
                ++face_count;
            }

            // Triangle t7:
            /* if ( b4 && b8 && b7 ) { */
            if ( b4 || b8 || b7 ) {
                /* std::cout << "Got t7! \n"; */
                flist.push_back( getVertexIndex(i+1, j+1) );
                flist.push_back( getVertexIndex(i+2, j+2) );
                flist.push_back( getVertexIndex(i+1, j+2) );
                ++face_count;
            }
        }
    }
//    for ( Natural i = 0; i < nX_ - 1; i += 2 ) {
//        for ( Natural j = 0; j < nY_ - 1; j += 2 ) {
//    for ( Natural i = 15; i < 16; i += 2 ) {
//        for ( Natural j = 15; j < 16; j += 2 ) {
//            b0 = getVertex3D( getVertexIndex(i+0, j+0), v0 );
//            b1 = getVertex3D( getVertexIndex(i+1, j+0), v1 );
//            b2 = getVertex3D( getVertexIndex(i+2, j+0), v2 );
//            b3 = getVertex3D( getVertexIndex(i+0, j+1), v3 );
//            b4 = getVertex3D( getVertexIndex(i+1, j+1), v4 );
//            b5 = getVertex3D( getVertexIndex(i+2, j+1), v5 );
//            b6 = getVertex3D( getVertexIndex(i+0, j+2), v6 );
//            b7 = getVertex3D( getVertexIndex(i+1, j+2), v7 );
//            b8 = getVertex3D( getVertexIndex(i+2, j+2), v8 );
//
//            // Triangle t0:
//            /* if ( b0 && b1 && b4 ) { */
//            if ( b0 || b1 || b4 ) {
//                /* std::cout << "Got t0! \n"; */
//                flist.push_back( getVertexIndex(i+0, j+0) );
//                flist.push_back( getVertexIndex(i+1, j+0) );
//                flist.push_back( getVertexIndex(i+1, j+1) );
//                ++face_count;
//            }
//
////            // Triangle t1:
////            /* if ( b0 && b4 && b3 ) { */
////            if ( b0 || b4 || b3 ) {
////                /* std::cout << "Got t1! \n"; */
////                flist.push_back( getVertexIndex(i+0, j+0) );
////                flist.push_back( getVertexIndex(i+1, j+1) );
////                flist.push_back( getVertexIndex(i+0, j+1) );
////                ++face_count;
////            }
////
////            // Triangle t2:
////            /* if ( b1 && b2 && b4 ) { */
////            if ( b1 || b2 || b4 ) {
////                /* std::cout << "Got t2! \n"; */
////                flist.push_back( getVertexIndex(i+1, j+0) );
////                flist.push_back( getVertexIndex(i+2, j+0) );
////                flist.push_back( getVertexIndex(i+1, j+1) );
////                ++face_count;
////            }
////
////            // Triangle t3:
////            /* if ( b2 && b5 && b4 ) { */
////            if ( b2 || b5 || b4 ) {
////                /* std::cout << "Got t3! \n"; */
////                flist.push_back( getVertexIndex(i+2, j+0) );
////                flist.push_back( getVertexIndex(i+2, j+1) );
////                flist.push_back( getVertexIndex(i+1, j+1) );
////                ++face_count;
////            }
////
////            // Triangle t4:
////            /* if ( b3 && b4 && b6 ) { */
////            if ( b3 || b4 || b6 ) {
////                /* std::cout << "Got t4! \n"; */
////                flist.push_back( getVertexIndex(i+0, j+1) );
////                flist.push_back( getVertexIndex(i+1, j+1) );
////                flist.push_back( getVertexIndex(i+0, j+2) );
////                ++face_count;
////            }
////
////            // Triangle t5:
////            /* if ( b4 && b7 && b6 ) { */
////            if ( b4 || b7 || b6 ) {
////                /* std::cout << "Got t5! \n"; */
////                flist.push_back( getVertexIndex(i+1, j+1) );
////                flist.push_back( getVertexIndex(i+1, j+2) );
////                flist.push_back( getVertexIndex(i+0, j+2) );
////                ++face_count;
////            }
////
////            // Triangle t6:
////            /* if ( b4 && b5 && b8 ) { */
////            if ( b4 || b5 || b8 ) {
////                /* std::cout << "Got t6! \n"; */
////                flist.push_back( getVertexIndex(i+1, j+1) );
////                flist.push_back( getVertexIndex(i+2, j+1) );
////                flist.push_back( getVertexIndex(i+2, j+2) );
////                ++face_count;
////            }
////
////            // Triangle t7:
////            /* if ( b4 && b8 && b7 ) { */
////            if ( b4 || b8 || b7 ) {
////                /* std::cout << "Got t7! \n"; */
////                flist.push_back( getVertexIndex(i+1, j+1) );
////                flist.push_back( getVertexIndex(i+2, j+2) );
////                flist.push_back( getVertexIndex(i+1, j+2) );
////                ++face_count;
////            }
//        }
//    }

    return face_count; 
}

template<typename VList, typename FList> 
unsigned int PlanarSurface::getMesh( VList &vertex_list, FList &face_list ) {

    unsigned int face_count = 0; 

    if ( getVertexList(vertex_list) == true ) { 
        face_count = getFaceList(face_list); 
    }

    return face_count; 
}


template<typename NList>
bool PlanarSurface::getNormalList( NList &nlist ) 
{
    if ( f->isSmooth() < 1 ) { 
        return false; 
    }

    /* std::cout << "Got here? \n\n"; */ 
    if ( interpolant_is_set_ == false ) {
        /* std::cout << "No normals for you today...\n\n"; */ 
        return false; 
    }

    updateDiscretization(); 

    /* std::cout << "Num vertices: " << num_vertices_ << "\n\n"; */ 
    nlist.resize(3*num_vertices_); 
    Point2 v; 
    Point3 n; 
    auto &nlist_omp = nlist; 

    auto num_vertices_omp = num_vertices_; 
    /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
    #pragma omp parallel for shared(nlist_omp) firstprivate(num_vertices_omp) private(v, n) default(none)
    for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
    {
        getNormal(v, n); 
        nlist_omp[3*i + 0] = n.x; 
        nlist_omp[3*i + 1] = n.z;
        nlist_omp[3*i + 2] = n.y;
    }

    return true; 
} 



template<typename CoordinatesList>
bool PlanarSurface::getRawHeightMap( CoordinatesList &vertices ) { 
    return f->getRawHeightMap(vertices); 
}

template<typename T>
bool PlanarSurface::getNormal( const Point2 &p, T& normal ) 
{
    if ( interpolant_is_set_ == false ) { 
        return false; 
    }

    return f->getNormal(p, normal); 
}

template<typename T>
bool PlanarSurface::getNormal( Point2 &&p, T& normal ) 
{
    return getNormal(p, normal);
}

template<typename T>
void PlanarSurface::getDependencyList( T &list ) const 
{
    list.insert(dependency_list_.begin(), dependency_list_.end()); 
}


#endif 

