#include "FlowVisualizationController.h"

FlowVisualizationController::FlowVisualizationController()
{

}


bool FlowVisualizationController::readData()
{
    std::string filename = std::string( "/Users/Clarissa/Downloads/results_example9coarse.vtk" );
    bool read_ok = data.readUnstructuredGridFile( filename );

    return read_ok;
}


void FlowVisualizationController::setVertices( vector< float > vertices )
{

}


void FlowVisualizationController::getVertices(vector< float >& vertices ) const
{
    data.getPoints( vertices );
}


void FlowVisualizationController::setTriangles( vector< int > triangles )
{

}


void FlowVisualizationController::getTriangles( vector< unsigned int >& triangles )
{
    vector< int > shape;
    vector< int > index;

    data.getCells( shape, index );
    int ncells = shape.size();


    for( int i = 0; i < ncells; ++i  )
    {
        int cell_shape = shape[ i ];

        int nvertices = 0;
        if ( cell_shape == VTKData::CODESHAPE::TETRAHEDRON ) nvertices = VERTICESSHAPE::TETRAHEDRON;

        int id = nvertices*i;

//        std::cout << "number of vertices for whole model = " << ncells << ", start vector = " << i << ", end vector = " << end << std::endl;

        std::vector< int > cell_vertices ( index.begin() + id,  index.begin() + id + nvertices  );
        transformInTriangles( cell_vertices, triangles );


    }

}


void FlowVisualizationController::transformInTriangles( vector< int > cell, vector< unsigned int >& triangles )
{

    int nvertices = cell.size();

    if( nvertices == VERTICESSHAPE::TETRAHEDRON )
    {
        triangles.push_back( ( unsigned int ) cell[ 0 ] );
        triangles.push_back( ( unsigned int ) cell[ 1 ] );
        triangles.push_back( ( unsigned int ) cell[ 3 ] );

        triangles.push_back( ( unsigned int ) cell[ 1 ] );
        triangles.push_back( ( unsigned int ) cell[ 2 ] );
        triangles.push_back( ( unsigned int ) cell[ 3 ] );

        triangles.push_back( ( unsigned int ) cell[ 2 ] );
        triangles.push_back( ( unsigned int ) cell[ 0 ] );
        triangles.push_back( ( unsigned int ) cell[ 3 ] );

        triangles.push_back( ( unsigned int ) cell[ 0 ] );
        triangles.push_back( ( unsigned int ) cell[ 1 ] );
        triangles.push_back( ( unsigned int ) cell[ 2 ] );


    }


}


void FlowVisualizationController::setBoundingBox( float xmin, float xmax, float ymin, float ymax, float zmin, float zmax )
{
}


void FlowVisualizationController::getBoundingBox( float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax ) const
{
    vector< float > vertices;
    data.getPoints( vertices );

    int nvertices = vertices.size()/3;
    if( nvertices == 0 ) return;

    xmin = vertices[ 0 ]; xmax = vertices[ 0 ];
    ymin = vertices[ 1 ]; ymax = vertices[ 1 ];
    zmin = vertices[ 2 ]; zmax = vertices[ 2 ];

    for( int i = 0; i < nvertices; i++ )
    {

        float x = vertices[ 3*i ];
        float y = vertices[ 3*i + 1 ];
        float z = vertices[ 3*i + 2 ];

        if( xmin >= x ) xmin = x;
        if( xmax <= x ) xmax = x;


        if( ymin >= y ) ymin = y;
        if( ymax <= y ) ymax = y;

        if( zmin >= z ) zmin = z;
        if( zmax <= z ) zmax = z;

    }

}


void FlowVisualizationController::setColors( vector< float > colors )
{

}


void FlowVisualizationController::getColors( vector< float >& colors ) const
{

}


void FlowVisualizationController::setColorMap()
{

}


void FlowVisualizationController::getColorMap() const
{

}


void FlowVisualizationController::setPointProperties( vector< std::string > property, vector< int > dim, vector< float > values )
{

}


void FlowVisualizationController::getPointProperties( vector< std::string >& property, vector< int >& dim, vector< float >& values ) const
{
    vector< std::string > format;
    data.getAttributesPoints( format, property, values );

    int npoints = property.size();

    for( int i = 0; i < npoints; ++i ){

        if( format[ i ].compare( "SCALARS" ) == 0 )
            dim.push_back ( VERTICESPROPERTY::SCALARS );

        else if( format[ i ].compare( "VECTORS" ) == 0 )
            dim.push_back ( VERTICESPROPERTY::VECTORS );

    }

}


void FlowVisualizationController::setCellsProperties( vector< std::string > property, vector < int > dim, vector< float > values )
{

}


void FlowVisualizationController::getCellsProperties( vector< std::string >& property, vector< int >& dim, vector< float >& values ) const
{
    vector < std::string > format;
    data.getAttributesPoints( format, property, values );

    int npoints = property.size();

    for( int i = 0; i < npoints; ++i ){

        if( format[ i ].compare( "SCALARS" ) == 0 )
            dim.push_back ( VERTICESPROPERTY::SCALARS );

        else if( format[ i ].compare( "VECTORS" ) == 0 )
            dim.push_back ( VERTICESPROPERTY::VECTORS );

    }
}


void FlowVisualizationController::setCurrentProperty( std::string property )
{
    current_property = property;
}


 std::string FlowVisualizationController::getCurrentProperty() const
{
    return current_property;
}


void FlowVisualizationController::clear(){}
