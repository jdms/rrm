#ifndef FLOWVISUALIZATIONCONTROLLER_H
#define FLOWVISUALIZATIONCONTROLLER_H

#include <vector>

#include <QString>

#include "VTKData.h"
#include "ColorMap.h"

using namespace std;

class FlowVisualizationController
{

    public:

        FlowVisualizationController();

        bool readData();

        void setVertices( vector< float > vertices );
        void getVertices(vector< float >& vertices );

        void setTriangles( vector< int > triangles );
        void getTriangles( vector< unsigned int >& triangles );

        void getWireframe( vector< unsigned int >& lines );
        void transformInLines( std::vector< int > cell, vector< unsigned int >& triangles );


        void setBoundingBox( float xmin, float xmax, float ymin, float ymax, float zmin, float zmax );
        void getBoundingBox( float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax ) ;

        void setColors( vector< float > colors );
        void getColors(vector< float >& colors , int option = 0 );

        void setColorMap( std::string colormap );
        std::string getColorMap() ;

        void getVerticesCell( int idcell, std::vector< int >& cell_vertices );

        void setPointProperties( vector< std::string > property , vector< int > dim, vector< float > values );
        void getPointProperties( vector< std::string >& property, vector< int >& dim, vector< float >& values );

        void setCellsProperties( vector< std::string > property, vector< int > dim, vector< float > values );
        void getCellsProperties( vector< std::string > & property, vector< int >& dim, vector< float >& values ) ;

        void setCurrentProperty( std::string property , string type );
        void getCurrentProperty( std::string& property, std::string& type ) ;

        void setCurrentProperty( int property );

        void transformInTriangles( vector< int > cell , vector< unsigned int> &triangles );

        void clear();


    protected:

        enum VERTICESSHAPE { TETRAHEDRON = 4 };
        enum VERTICESPROPERTY { SCALARS = 1, VECTORS = 3 };

        std::map< std::string, int > property_map;
        std::string current_property;
        std::string current_property_type;
        std::string current_colormap;

        VTKData data;
        ColorMap colormap;


};

#endif // FLOWVISUALIZATIONCONTROLLER_H
