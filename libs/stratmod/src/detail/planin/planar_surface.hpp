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


#ifndef __PLANAR_SURFACE__
#define __PLANAR_SURFACE__

#include <array>
#include <vector> 
#include <list> 
#include <memory> 
#include <type_traits>
#include <cstdint>

#include "use_openmp.hpp"
#include "serialization_primitives.hpp"

#include "interpolated_graph.hpp"
#include "triangle_soup_wrapper.hpp" 

#include "mesh/polyhedra.hpp"

/* Class PlanarSurface is not reentrant. */ 

/* template<typename CoordinatesListType = std::vector<float>, typename Natural = unsigned long int> */ 
class PlanarSurface { 
    public: 
        /* using CoordinatesList = CoordinatesListType; */ 
        using Ptr = std::shared_ptr<PlanarSurface>; 
        using WeakPtr = std::weak_ptr<PlanarSurface>; 
        /* using NaturalType = Natural; */ 
        using Natural = unsigned long int; 
        using SurfaceId = unsigned long int; 

        enum class Coordinate : Natural { WIDTH = 0, HEIGHT = 1, DEPTH = 2 };  

        using IndicesType = std::array<Natural, 2>; 

        PlanarSurface( bool extruded_surface = false, bool orthogonally_oriented = false ); 

        PlanarSurface( const PlanarSurface & ); 
        PlanarSurface& operator=( const PlanarSurface & ) = delete; 

        PlanarSurface( PlanarSurface && ); 
        PlanarSurface& operator=( PlanarSurface && ) = delete; 


        /* Methods */ 

        /* Setters. */ 
        bool addExtrusionPathPoint( double abscissa, double ordinate );
        void setPathOrigin( double abscissa, double ordinate = 0. );

        bool addPoint( const Point3 &p ); 
        bool addPoint( Point3 &&p ); 
        bool addPoints( const std::vector<Point3> &points ); 

        template<typename Point3Type>
        bool addPoint( Point3Type &&p ); 

        template<typename Point3Type>
        bool addPoints( const std::vector<Point3Type> &points); 

        static void setOrigin( const Point3 &o ); 
        static void setOrigin( Point3 &&o ); 

        static bool setLenght( const Point3 &l ); 
        static bool setLenght( Point3 &&l ); 

        /* Create surface. */ 
        bool generateSurface(); 
        bool updateRawCache(); 
        bool updateCache();

        void updateDiscretization(); 
        static bool requestChangeDiscretization( Natural numX, Natural numY ); 
        static bool setOutputCoordinatesOrdering( Coordinate first, Coordinate second, Coordinate third ); 

        static Natural getDiscretizationX();
        static Natural getDiscretizationY();

        /* Geta mesh, a vertex list or just the surface's connectivity. */ 
        template<typename VList>
        bool getVertexList( VList &vlist );  

        template<typename FList>
        unsigned int getFaceList( FList &flist ); 

        template<typename VList, typename FList> 
        unsigned int getMesh( VList &vertex_list, FList &face_list ); 

        template<typename NList>
        bool getNormalList( NList &normal_list ); 

        template<typename VList>
        bool getRawPathVertexList( VList &path_vertex_list );

        template<typename VList>
        bool getPathVertexList( VList &path_vertex_list );

        /* Basic methods. */ 
        bool surfaceIsSet();
        unsigned long int getID() const; 
        bool checkIfDependsOn( unsigned long int surface_id ); 

        bool rangeCheck( Natural vertex );

        bool weakEntireSurfaceCheck(); 
        bool weakBoundedEntireSurfaceCheck( PlanarSurface::Ptr &lower_surface, PlanarSurface::Ptr &upper_surface ); 
        bool weakBoundedEntireSurfaceCheck( PlanarSurface::WeakPtr lower_surface, PlanarSurface::WeakPtr upper_surface ); 

        bool weakBoundedEntireSurfaceCheck( std::vector<PlanarSurface::Ptr> &lower_surfaces, std::vector<PlanarSurface::Ptr> &upper_surfaces ); 
        bool weakBoundedEntireSurfaceCheck( std::vector<PlanarSurface::WeakPtr> &lower_surfaces, std::vector<PlanarSurface::WeakPtr> &upper_surfaces ); 
        bool weakBoundedEntireSurfaceCheck( std::vector<PlanarSurface::WeakPtr> &&lower_surfaces, std::vector<PlanarSurface::WeakPtr> &&upper_surfaces ); 

        bool weakIntersectionCheck( PlanarSurface::Ptr &s );
        bool weakIntersectionCheck( PlanarSurface::WeakPtr s ); 

        bool weakCompleteIntersectionCheck( PlanarSurface::Ptr &s );
        bool weakCompleteIntersectionCheck( PlanarSurface::WeakPtr s ); 

        bool weakLiesAboveCheck( PlanarSurface::Ptr &s ); 
        bool weakLiesAboveCheck( PlanarSurface::WeakPtr s ); 

        bool weakLiesBelowCheck( PlanarSurface::Ptr &s ); 
        bool weakLiesBelowCheck( PlanarSurface::WeakPtr s ); 

        bool weakLiesAboveOrEqualsCheck( PlanarSurface::Ptr &s ); 
        bool weakLiesAboveOrEqualsCheck( PlanarSurface::WeakPtr s ); 

        bool weakLiesAboveOrEqualsCheck( std::vector<PlanarSurface::Ptr> &surfaces ); 
        bool weakLiesAboveOrEqualsCheck( std::vector<PlanarSurface::WeakPtr> &surfaces ); 

        bool weakLiesBelowOrEqualsCheck( PlanarSurface::Ptr &s ); 
        bool weakLiesBelowOrEqualsCheck( PlanarSurface::WeakPtr s ); 

        bool weakLiesBelowOrEqualsCheck( std::vector<PlanarSurface::Ptr> &surfaces ); 
        bool weakLiesBelowOrEqualsCheck( std::vector<PlanarSurface::WeakPtr> &surfaces ); 

        bool getVertex2D( Natural index, Point2 &v );
        bool getVertex3D( Natural index, Point3 &v );
        bool getMappedVertex3D( Natural index, Point3 &v );

        bool getHeight( Natural vertex_index, double &height );
        bool getHeight( Natural i, Natural j, double &height ); 

        bool getHeight( Natural vertex_index, double &height, SurfaceId &bounding_surface_id );

        bool getHeight( const Point2 &p, double &height );
        bool getHeight( Point2 &&p, double &height );

        bool getCachedHeight( Natural vertex_index, double &height );
        bool getCachedHeight( Natural i, Natural j, double &height );

        std::size_t getNumX(); 
        std::size_t getNumY(); 
        std::size_t getNumVertices(); 
        Natural getVertexIndex( Natural i, Natural j ) const;

        bool getCachedBoundingSurfacesIDs( std::vector<SurfaceId> &bounding_surface_ids );

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

        Point3 getOrigin(); 
        Point3 getLenght(); 

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

        template<typename T>
        void getUpperBoundList( T &list ) const; 

        template<typename T>
        void getLowerBoundList( T &list ) const; 

        bool removeAbove( PlanarSurface::Ptr &s ); 
        bool removeAbove( PlanarSurface::WeakPtr s ); 

        bool removeBelow( PlanarSurface::Ptr &s ); 
        bool removeBelow( PlanarSurface::WeakPtr s ); 

        /* Maintenance */ 
        void pruneBoundingLists(); 
        void clearBoundingLists(); 

        /* Misc */ 
        bool isExtrudedSurface(); 
        bool isPathExtrudedSurface();
        bool isOrthogonallyOrientedSurface();

        bool getRawData( std::vector<Point2> &points, std::vector<double> &fevals );

        template<typename FList = std::vector<size_t>>
        size_t mergeFaceLists( const std::vector<FList> &flists, FList &merged_flist );

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
        unsigned long int id_; 

        InterpolatedGraph::Ptr f;  

        static std::array<Natural, 3> coordinates_map_; 

        static Point3 origin; 
        static Point3 lenght; 

        std::vector<double> heights; 
        std::vector<double> normals;
        /* std::vector<bool> valid_heights; */ 

        bool interpolant_is_set_ = false; 

        /* Right now, 'mesh_is_set_' is useless. */ 
        bool mesh_is_set_ = false; 

        std::list< std::weak_ptr<PlanarSurface> > upper_bound_; 
        std::list< std::weak_ptr<PlanarSurface> > lower_bound_; 
        /* std::list< std::weak_ptr<PlanarSurface> > unprocessed_upper_bound_; */ 
        /* std::list< std::weak_ptr<PlanarSurface> > unprocessed_lower_bound_; */ 
        std::set< unsigned long int> dependency_list_; 

        bool extruded_surface_ = false; 

        /* Cache the results of applying rules as well as original surface heights */ 
        /* bool raw_cache_is_fresh_ = false; */
        bool cache_is_fresh_ = false;
        std::vector<double> cached_heights_;
        std::vector<bool> cached_valid_heights_;
        std::vector<SurfaceId> cached_bounding_surface_ids_;
        bool surface_is_empty_ = false;

        /* Methods */ 
        bool getHeight( Natural vertex_index, double &height, 
                std::vector<double> &base_heights,
                std::list<std::weak_ptr<PlanarSurface>> &ub_list,
                std::list<std::weak_ptr<PlanarSurface>> &lb_list,
                SurfaceId &bounding_surface_id
                );

        bool rangeCheck( Natural i, Natural j );
        bool getVertexIndices( Natural v, IndicesType &indices ); 

        size_t getBlockIndex( size_t i, size_t j ) const;
        IndicesType getBlockIndices( size_t b ) const;

        size_t getVertexIndexFromPositionInBlock( size_t vpos,  size_t bindex ) const;

        TriangleHeights getTriangleHeightsFromPositionInBlock( size_t tpos, size_t bindex );

        size_t getTriangleIndexFromPositionInBlock( size_t tpos, size_t bindex ) const;

        std::vector<size_t> getTriangleConnectivityFromIndex(size_t tindex ) const;

        size_t getTriangleIndexFromConnectivity( std::vector<size_t> t_connectivity );

        bool compareSurfaceWptr( const PlanarSurface::WeakPtr &left, const PlanarSurface::WeakPtr &right ) const;

        friend class SRules;

        // Cereal provides an easy way to serialize objects
        friend class cereal::access;

        template<typename Archive>
        void save( Archive &ar, const std::uint32_t version ) const;

        template<typename Archive>
        void load( Archive &ar, const std::uint32_t version );
};

template<typename Point3Type>
bool PlanarSurface::addPoint( Point3Type &&p ) 
{ 
    return f->addPoint(p);
}

template<typename Point3Type>
bool PlanarSurface::addPoints( const std::vector<Point3Type> &points)
{
    return f->addPoints(points);
}

template<typename VList>
bool PlanarSurface::getRawPathVertexList( VList &pvlist )
{
    if ( surfaceIsSet() == false ) {
        return false; 
    }

    if ( f->isPathExtrudedSurface() == false )
    {
        return false;
    }

    std::vector<double> abscissas;
    std::vector<double> ordinates;

    if ( f->getRawPathData(abscissas, ordinates) == false )
    {
        return false;
    }

    pvlist.resize( 2*abscissas.size() );

    std::cout << "\nPath list: ";
    for ( size_t i = 0; i < abscissas.size(); ++i )
    {
        if ( isOrthogonallyOrientedSurface() )
        {
            pvlist[ 2*i + 0 ] = abscissas[i];
            pvlist[ 2*i + 1 ] = ordinates[i];
        }
        else
        {
            pvlist[ 2*i + 1 ] = abscissas[i];
            pvlist[ 2*i + 0 ] = ordinates[i];
        }
        std::cout << "(" << pvlist[2*i+0] << ", " << pvlist[2*i+1] << "), ";
    }
    std::cout << std::endl << std::flush;

    return true;
}

template<typename VList>
bool PlanarSurface::getPathVertexList( VList &pvlist )
{
    if ( surfaceIsSet() == false ) {
        return false; 
    }

    if ( f->isPathExtrudedSurface() == false )
    {
        return false;
    }

    pvlist.resize(2*nY_);
    Point2 v {};

    for ( long int j = 0; j < static_cast<long int>( nY_ ); ++j )
    {
        getVertex2D( getVertexIndex(0, j), v );
        if ( isOrthogonallyOrientedsurface() )
        {
            pvlist[ 2*j + 0 ] = v.x; ;
            pvlist[ 2*j + 1 ] = f->getPathOrdinate(v.x);
        }
        else
        {
            pvlist[ 2*j + 0 ] = f->getPathOrdinate(v.y);
            pvlist[ 2*j + 1 ] = v.y;
        }
        
    }

    return true;
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
    auto &cmap_omp = coordinates_map_; 

    if ( num_vertices_ > 0 )
    {
        using OutRealType = typename std::remove_reference< decltype( vlist[0] ) >::type;

        /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
        #pragma omp parallel for shared(vlist_omp, cmap_omp) firstprivate(num_vertices_omp) private(v) default(none)
        for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
        {
            getVertex3D(i, v);
            vlist_omp[3*i + 0] = static_cast<OutRealType>( v[cmap_omp[0]] ); 
            vlist_omp[3*i + 1] = static_cast<OutRealType>( v[cmap_omp[1]] ); 
            vlist_omp[3*i + 2] = static_cast<OutRealType>( v[cmap_omp[2]] ); 
        }

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
        return 0; 
    }

    if ( cache_is_fresh_ && surface_is_empty_ )
    {
        return 0;
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
    if ( surfaceIsSet() == false ) {
        return false; 
    }

    if ( f->isSmooth() < 1 )
    {
        return false;
    }

    updateDiscretization(); 

    /* std::cout << "Num vertices: " << num_vertices_ << "\n\n"; */ 
    nlist.resize(3*num_vertices_); 
    auto &nlist_omp = nlist; 
    auto num_vertices_omp = num_vertices_; 
    auto &cmap_omp = coordinates_map_; 

    if ( num_vertices_ > 0 )
    {
        using OutRealType = typename std::remove_reference< decltype( nlist[0] ) >::type;

        /* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type*/ 
        /* #pragma omp parallel for shared(nlist_omp, cmap_omp) firstprivate(num_vertices_omp) private(v) default(none) */
        for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) 
        {
            nlist_omp[3*i + 0] = static_cast<OutRealType>( normals[ 3*i + cmap_omp[0] ] ); 
            nlist_omp[3*i + 1] = static_cast<OutRealType>( normals[ 3*i + cmap_omp[1] ] ); 
            nlist_omp[3*i + 2] = static_cast<OutRealType>( normals[ 3*i + cmap_omp[2] ] ); 
        }

    }

    return true; 
}
/* { */
/*     if ( f->isSmooth() < 1 ) { */ 
/*         return false; */ 
/*     } */

/*     /1* std::cout << "Got here? \n\n"; *1/ */ 
/*     if ( interpolant_is_set_ == false ) { */
/*         /1* std::cout << "No normals for you today...\n\n"; *1/ */ 
/*         return false; */ 
/*     } */

/*     updateDiscretization(); */ 

/*     /1* std::cout << "Num vertices: " << num_vertices_ << "\n\n"; *1/ */ 
/*     nlist.resize(3*num_vertices_); */ 
/*     Point2 v; */ 
/*     Point3 n; */ 
/*     auto &nlist_omp = nlist; */ 
/*     auto &cmap_omp = coordinates_map_; */ 

/*     auto num_vertices_omp = num_vertices_; */ 
/*     /1* VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type *1/ */ 
/*     #pragma omp parallel for shared(nlist_omp, cmap_omp) firstprivate(num_vertices_omp) private(v, n) default(none) */
/*     for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) */ 
/*     { */
/*         getNormal(v, n); */ 
/*         nlist_omp[3*i + 0] = n[cmap_omp[0]]; */ 
/*         nlist_omp[3*i + 1] = n[cmap_omp[1]]; */ 
/*         nlist_omp[3*i + 2] = n[cmap_omp[2]]; */ 
/*     } */

/*     return true; */ 
/* } */ 



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

    /* T n; */
    /* bool success = f->getNormal(p, n); */ 

    /* normal[0] = n[ coordinates_map_[0] ]; */
    /* normal[1] = n[ coordinates_map_[1] ]; */
    /* normal[2] = n[ coordinates_map_[2] ]; */

    /* return success; */
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

template<typename T>
void PlanarSurface::getUpperBoundList( T &list ) const 
{
    list.insert( list.end(), upper_bound_.begin(), upper_bound_.end() ); 
    std::unique( list.begin(), list.end(), 
            [this] ( PlanarSurface::WeakPtr &l, PlanarSurface::WeakPtr &r)  -> bool 
            { 
                return this->compareSurfaceWptr(l, r); 
            } 
        ); 
}

template<typename T>
void PlanarSurface::getLowerBoundList( T &list ) const 
{
    list.insert( list.end(), lower_bound_.begin(), lower_bound_.end() ); 
    std::unique( list.begin(), list.end(), 
            [this] ( PlanarSurface::WeakPtr &l, PlanarSurface::WeakPtr &r)  -> bool 
            { 
                return this->compareSurfaceWptr(l, r); 
            } 
        ); 
}

template<typename FList>
size_t PlanarSurface::mergeFaceLists( const std::vector<FList> &flists, FList &merged_flist )
{
    size_t num_triangles = 0;

    if ( flists.empty() )
    {
        return 0;
    }

    size_t numBlocks = discretization_X * discretization_Y;
    size_t numTrianglesPerBlock = 8;
    size_t numTriangles = numTrianglesPerBlock * numBlocks;

    std::vector<bool> triangle_is_present(numTriangles, false);
    size_t v0, v1, v2, tindex;
    std::vector<size_t> triangle;

    for ( size_t i = 0; i < flists.size(); ++i )
    {
        auto &triangle_list = flists[i];

        num_triangles = triangle_list.size()/3;

        if ( num_triangles > numTriangles )
        {
            return 0;
        }

        for ( size_t j = 0; j < num_triangles; ++j )
        {
            v0 = triangle_list[ 3*j + 0 ];
            v1 = triangle_list[ 3*j + 1 ];
            v2 = triangle_list[ 3*j + 2 ];

            tindex = getTriangleIndexFromConnectivity( {v0, v1, v2} );
            triangle_is_present[tindex] = true;
        }
    }

    num_triangles = 0;
    merged_flist.clear();

    for ( size_t t = 0; t < numTriangles; ++t )
    {
        if ( triangle_is_present[t] )
        {
            triangle = getTriangleConnectivityFromIndex(t);
            std::copy( triangle.begin(), triangle.end(), std::back_inserter(merged_flist) );
            ++num_triangles;
        }
    }

    return num_triangles;
}


#if defined( BUILD_WITH_SERIALIZATION )
    #include "cereal/types/array.hpp"
    #include "cereal/types/vector.hpp"
    #include "cereal/types/list.hpp"
    #include "cereal/types/memory.hpp"

    template<typename Archive>
    void PlanarSurface::save( Archive &ar, const std::uint32_t version ) const
    {
        (void)(version);
        ar(
           discretization_X, 
           discretization_Y, 
           tolerance, 
           global_discretization_state_, 
           this_discretization_state_, 
           nX_, 
           nY_, 
           num_vertices_, 
           num_instances_, 
           id_, 
           f, 
           coordinates_map_, 
           origin, 
           lenght, 
           /* heights, */ 
           interpolant_is_set_, 
           mesh_is_set_, 
           upper_bound_, 
           lower_bound_, 
           dependency_list_, 
           extruded_surface_
          );
    }

    template<typename Archive>
    void PlanarSurface::load( Archive &ar, const std::uint32_t version )
    {
        (void)(version);
        ar(
           discretization_X, 
           discretization_Y, 
           tolerance, 
           global_discretization_state_, 
           this_discretization_state_, 
           nX_, 
           nY_, 
           num_vertices_, 
           num_instances_, 
           id_, 
           f, 
           coordinates_map_, 
           origin, 
           lenght, 
           /* heights, */ 
           interpolant_is_set_, 
           mesh_is_set_, 
           upper_bound_, 
           lower_bound_, 
           dependency_list_, 
           extruded_surface_
          );

        updateRawCache();
    }

    CEREAL_CLASS_VERSION(PlanarSurface, 1);

#else
    template<typename Archive>
    void PlanarSurface::save( Archive &, const std::uint32_t ) const {}

    template<typename Archive>
    void PlanarSurface::load( Archive &, const std::uint32_t ) {}

#endif /* BUILD_WITH_SERIALIZATION */

#endif 

