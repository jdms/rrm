#ifndef STRATIGRAPHY_HPP
#define STRATIGRAPHY_HPP

#include <vector>
#include <map>

#include <QtGlobal>


//#include "Core/Geometry/PolygonalCurve/CurveN.hpp"
#include "Core/Geometry/PolygonalCurve2D.hpp"


class Stratigraphy
{

    public:



        Stratigraphy(){}
		

        inline void addCurve( const float& d, Curve2D const& c ){ curves_list[ d ] = c; crosssections_list.push_back( d ); }
        inline Curve2D* getCurve( const float& d ){ return &curves_list[ d ]; }
        inline void updateCurve( const float& d, Curve2D const& c ){ curves_list[ d ] = c; crosssections_list.push_back( d ); }


        inline std::vector< float > getCrossections(){ return crosssections_list; }

		inline bool isEmpty(){ return curves_list.empty(); }
	

        inline void setId( const unsigned int& id ){ index = id; }
        inline unsigned int getId(){ return index; }


        inline void updateSurface( const std::vector< float >& vertices,  const std::vector< unsigned int >& faces )
        {

            surface_vertices.clear();
            surface_faces.clear();

            surface_vertices.assign( vertices.begin(), vertices.end() );
            surface_faces.assign( faces.begin(), faces.end() );

        }
        inline void updateCurveWireframe( const std::vector< unsigned int >& edges )
        {
            curve_edges.clear();
            curve_edges.assign( edges.begin(), edges.end() );
        }


        inline std::vector< unsigned int >& getCurveEdges(){ return curve_edges; }

        inline std::vector< float >& getSurfaceVertices(){ return surface_vertices; }
        inline std::vector< unsigned int >& getSurfaceFaces(){ return surface_faces; }



	protected:
	

        unsigned int index;

        std::vector< float > crosssections_list;

        std::map< float, Curve2D > curves_list;

        std::vector< unsigned int > curve_edges;

        std::vector< float > surface_vertices;
        std::vector< unsigned int > surface_faces;



};


#endif
