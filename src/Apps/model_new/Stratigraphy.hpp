#ifndef STRATIGRAPHY_HPP
#define STRATIGRAPHY_HPP

#include <vector>
#include <map>

#include <QtGlobal>


#include "Core/Geometry/PolygonalCurve/CurveN.hpp"


class Stratigraphy
{

    public:


        typedef qreal			Real;
        typedef PolygonalCurve<Real, 2, PointN<Real, 2>, VectorN<Real, 2> > Curve2D;
        typedef PointN<Real, 2> 					    Point2D;

		
        Stratigraphy(){}
		

        inline void addCurve( const float& d, Stratigraphy::Curve2D const& c ){ curves_list[ d ] = c; crosssections_list.push_back( d ); }
        inline Stratigraphy::Curve2D getCurve( const float& d ){ return curves_list[ d ]; }
        inline void updateCurve( const float& d, Stratigraphy::Curve2D const& c ){ curves_list[ d ] = c; crosssections_list.push_back( d ); }


        inline std::vector< float > getCrossections(){ return crosssections_list; }

		inline bool isEmpty(){ return curves_list.empty(); }
	

        inline void setId( unsigned int id ){ index = id; }
        inline unsigned int getId(){ return index; }


        inline void updateSurface( const std::vector< Stratigraphy::Point2D >& vertices,  const std::vector< unsigned int >& faces )
        {
            surface_vertices = vertices;
            surface_faces = faces;
        }
        inline void updateSurfaceWireframe( const std::vector< unsigned int >& edges ){ surface_edges = edges; }


	protected:
	

        unsigned int index;

        std::vector< float > crosssections_list;

        std::map< float, Stratigraphy::Curve2D > curves_list;

        std::vector< Stratigraphy::Point2D > surface_vertices;
        std::vector< unsigned int > surface_faces;
        std::vector< unsigned int > surface_edges;


};


#endif
