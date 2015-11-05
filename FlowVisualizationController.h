#ifndef FLOWVISUALIZATIONCONTROLLER_H
#define FLOWVISUALIZATIONCONTROLLER_H

#include <vector>

#include <QObject>
#include <QString>

#include "VTKData.h"
#include "ColorMap.h"
#include "FlowComputation/region.h"

using namespace std;

class FlowVisualizationController: public QObject
{
    Q_OBJECT

    public:

        FlowVisualizationController();

        bool readData();

        inline void setVertices( vector< float > vertices ){ if( vertices.empty() == true ) return; data.setVectorPoints( vertices ); }
        inline void getVertices(vector< float >& vertices ){ if( data.isEmpty() == true ) return; data.getVectorPoints( vertices ); }


        inline void setColorMap( std::string colormap ){ current_colormap = colormap; }
        inline const std::string getColorMap() { return current_colormap; }


        void addPointProperty( std::string name, std::string format, std::string type, int ncoords, vector< float > values );
        inline void setPointProperties( vector< FlowProperty > properties ){ data.setPointsFlowProperties( properties ); }
        inline void getPointProperties( vector< FlowProperty >& properties ){  if( data.isEmpty() == true ) return;  data.getPointsFlowProperties( properties ); }


        void addCellProperty( std::string name, std::string format, std::string type, int ncoords, vector< float > values );
        inline void setCellsProperties( vector< FlowProperty > properties ){ if( properties.empty() == true ) return; data.setCellsFlowProperties( properties ); }
        inline void getCellsProperties( vector< FlowProperty > & properties ) {if( data.isEmpty() == true ) return; data.getCellsFlowProperties( properties ); }


        void setCurrentProperty( int id );
        inline void setCurrentProperty( std::string property , string type ){ current_property = property; current_property_type = type; }
        inline void getCurrentProperty( std::string& property, std::string& type ) { property = current_property; type = current_property_type; }


        void getPointProperty( int id, std::string& name, std::string& format, std::string& type, int &ncoords );
        void getCellProperty( int id, std::string& name, std::string& format, std::string& type, int &ncoords );



        void getBoundingBox( float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax ) ;


        void setColors( vector< float > colors );
        void getColors(vector< float >& colors , int option = 0 );


        void getTriangles( vector< unsigned int >& triangles );
        void getWireframe( vector< unsigned int >& lines );


        void transformInTriangles( vector< int > cell , vector< unsigned int> &triangles );
        void transformInLines( std::vector< int > cell, vector< unsigned int >& triangles );

\
        void getColorConstant(COLORMAP map, vector< float >& colors );
        void getMagnitudeColors( COLORMAP map, vector< float >& colors );
        void getCoordinateColors( COLORMAP map, vector< float >& colors, int option );
        void getMagnitudeColorsCells( COLORMAP map, vector< float >& colors );
        void getCoordinateColorsCells( COLORMAP map, vector< float >& colors, int option );




        void getPointMaxMin(  FlowProperty p, vector< float >& maxmin );
        void getCellMaxMin(  vector< float >& maxmin );

        void getVerticesCell( int idcell, std::vector< int >& cell_vertices );


        inline const int getNumberofPointsProperties() { return data.getNumberofPointsProperties(); }
        inline const int getNumberofCellsProperties() { return data.getNumberofCellsProperties(); }


        void getSurface( vector< float > points, vector< unsigned int > edges );
        void getSurfaceBoundingBox( float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax );


        void updatePropertiesNamesVector();
        FlowProperty &getPropertyfromMap( int id );

        void getTetgenioObject();

        void clear();

        void getTrianglesSurface( vector< unsigned int >& triangles );
        void getTrianglesfromRectangles( int nvertices, int* verticeslist, vector< unsigned int >& triangles  );
        void getWireframeSurface( vector< unsigned int >& lines );
        void getPointsSurface( vector< float >& vertices );


    protected:

        void loadSurfaceData( std::vector< NODE > nodes, std::vector< TETRAHEDRON > elements );


    public slots:

        void openSurfaceFile( std::string filename );
        void openUserInputFile( std::string filename );
        void computeVolumetricMesh();
        void computeFlowProperties();

        void computePressure();
        void computeVelocity();
        void computeTOF();

        void selectFlowProperty( int id, bool& option );
        void getUserInput( std::string file_user, std::string surface_file, float tol1, float tol2 );

signals:
        void updateComboBox( vector< std::string> , vector< std::string > );


    protected:

        enum VERTICESSHAPE { NTETRAHEDRON = 4 };
        enum VERTICESPROPERTY { SCALARS = 1, VECTORS = 3 };

        std::map< int, FlowProperty > property_map;
        std::string current_property;
        std::string current_property_type;
        std::string current_colormap;

        VTKData data;
        ColorMap colormap;

        REGION region;
        tetgenio surface_file;


};

#endif // FLOWVISUALIZATIONCONTROLLER_H
