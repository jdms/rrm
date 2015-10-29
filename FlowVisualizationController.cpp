#include <algorithm>

#include "FlowVisualizationController.h"


FlowVisualizationController::FlowVisualizationController()
{
    current_property = "Velocity";
    current_property_type = "POINTS";

}


bool FlowVisualizationController::readData()
{
    std::string filename = std::string( "/Users/Clarissa/Downloads/results_example9coarse.vtk" );

    if( data.isEmpty() == false )
        cout << "You should clean before!" << endl;

    bool read_ok = data.readUnstructuredGridFile( filename );
    data.setupData();


    return read_ok;
}




void FlowVisualizationController::setVertices( vector< float > vertices )
{

}


void FlowVisualizationController::getVertices(vector< float >& vertices )
{
    if( data.isEmpty() == true ) return;

    data.getPoints( vertices );
}


void FlowVisualizationController::setTriangles( vector< int > triangles )
{

}


void FlowVisualizationController::getVerticesCell( int idcell, std::vector< int >& cell_vertices )
{
    if( data.isEmpty() == true ) return;

//    vector< int > shape;
//    vector< int > index;

//    data.getCells( shape, index );
//    int ncells = shape.size();

//    int id = 0;
//    int nvertices = 0;
//    for( int i = 0; i < idcell; ++i  )
//    {
//        int cell_shape = shape[ i ];

//        if ( cell_shape == VTKData::CODESHAPE::TETRAHEDRON )
//            nvertices = FlowVisualizationController::VERTICESSHAPE::TETRAHEDRON;

//        id += nvertices;

//    }

//    if( idcell == 0 )
//    {
//        int cell_shape = shape[ 0 ];
//        if ( cell_shape == VTKData::CODESHAPE::TETRAHEDRON )
//            nvertices = FlowVisualizationController::VERTICESSHAPE::TETRAHEDRON;
//    }


//    for( int i = id; i < nvertices + id; ++i )
//        cell_vertices.push_back( index[ i ] );


    vector< int > shape;
    vector< int > index;

    data.getCells( shape, index );
    cell_vertices.push_back( index[ 4*idcell ] );
    cell_vertices.push_back( index[ 4*idcell + 1 ] );
    cell_vertices.push_back( index[ 4*idcell + 2 ] );
    cell_vertices.push_back( index[ 4*idcell + 3 ] );



}

void FlowVisualizationController::getTriangles( vector< unsigned int >& triangles )
{

    if( data.isEmpty() == true ) return;

    vector< int > shape;
    vector< int > index;

    data.getCells( shape, index );
    int ncells = shape.size();


    for( int i = 0; i < ncells; ++i  )
    {
//        int cell_shape = shape[ i ];

//        int nvertices = 0;
//        if ( cell_shape == VTKData::CODESHAPE::TETRAHEDRON ) nvertices = FlowVisualizationController::VERTICESSHAPE::TETRAHEDRON;

//        int id = nvertices*i;

////        std::cout << "number of vertices for whole model = " << ncells << ", start vector = " << i << ", end vector = " << end << std::endl;

//        std::vector< int > cell_vertices ( index.begin() + id,  index.begin() + id + nvertices  );
//        transformInTriangles( cell_vertices, triangles );

        int id = 4*i;
        int nvertices = 4;
        std::vector< int > cell_vertices ( index.begin() + id,  index.begin() + id + nvertices  );
        transformInTriangles( cell_vertices, triangles );


    }

}



void FlowVisualizationController::getWireframe( vector< unsigned int >& lines )
{
    if( data.isEmpty() == true ) return;

    vector< int > shape;
    vector< int > index;

    data.getCells( shape, index );
    int ncells = shape.size();


    for( int i = 0; i < ncells; ++i  )
    {

        std::vector< int > cell_vertices;
        getVerticesCell( i, cell_vertices );
        transformInLines( cell_vertices, lines );


    }
}

void FlowVisualizationController::transformInLines( std::vector< int > cell, vector< unsigned int >& triangles )
{

    int nvertices = cell.size();

    if( nvertices == FlowVisualizationController::VERTICESSHAPE::TETRAHEDRON )
    {
        triangles.push_back( ( unsigned int ) cell[ 0 ] );
        triangles.push_back( ( unsigned int ) cell[ 1 ] );

        triangles.push_back( ( unsigned int ) cell[ 1 ] );
        triangles.push_back( ( unsigned int ) cell[ 3 ] );

        triangles.push_back( ( unsigned int ) cell[ 3 ] );
        triangles.push_back( ( unsigned int ) cell[ 0 ] );

        triangles.push_back( ( unsigned int ) cell[ 1 ] );
        triangles.push_back( ( unsigned int ) cell[ 2 ] );

        triangles.push_back( ( unsigned int ) cell[ 2 ] );
        triangles.push_back( ( unsigned int ) cell[ 3 ] );

        triangles.push_back( ( unsigned int ) cell[ 2 ] );
        triangles.push_back( ( unsigned int ) cell[ 0 ] );

        triangles.push_back( ( unsigned int ) cell[ 3 ] );
        triangles.push_back( ( unsigned int ) cell[ 2 ] );

    }
}






void FlowVisualizationController::transformInTriangles( vector< int > cell, vector< unsigned int >& triangles )
{

    int nvertices = cell.size();

    if( nvertices == FlowVisualizationController::VERTICESSHAPE::TETRAHEDRON )
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


void FlowVisualizationController::getBoundingBox( float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax )
{
    if( data.isEmpty() == true ) return;

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

void FlowVisualizationController::getColors( vector< float >& colors, int option  )
{
    if( data.isEmpty() == true ) return;

    COLORMAP map;

    if( current_colormap == "JET" )
        map = COLORMAP::JET;
    else if( current_colormap == "CONSTANT" )
        map = COLORMAP::CONSTANT;
    else
        map = COLORMAP::JET;

    int ncomp = 0;
    vector< float > values;
    vector< float > maxmin;

    data.getAttribute( current_property, current_property_type, ncomp, values );


    if( current_property_type == "POINTS" )
    {


        if( ncomp == 1 )
        {
            data.getMaxMinAttribute( current_property, current_property_type, ncomp, maxmin );

            int nvalues = values.size();
            for( int i = 0; i < nvalues; ++i )
            {
                QVector4D color = colormap.getColor( map, values[ i ], maxmin[ 0 ], maxmin[ 1 ] );
                colors.push_back( color.x() );
                colors.push_back( color.y() );
                colors.push_back( color.z() );

            }

        }

        else if( ncomp == 3 )
        {
            int nvalues = values.size()/3;

            if( option == 0 )
            {

                data.getMaxMinAttribute( current_property, current_property_type, ncomp, maxmin );

                for( int i = 0; i < nvalues; ++i )
                {

                    float value = values[ 3*i ]*values[ 3*i ] + values[ 3*i + 1 ]*values[ 3*i + 1 ] + values[ 3*i + 2 ]*values[ 3*i + 2 ];
                    QVector4D color = colormap.getColor( map, value, maxmin[ 0 ], maxmin[ 1 ] );
                    colors.push_back( color.x() );
                    colors.push_back( color.y() );
                    colors.push_back( color.z() );

                }


            }

            else if( option == 1 )
            {
                data.getMaxMinCoordinates( current_property, current_property_type, maxmin );

                for( int i = 0; i < nvalues; ++i )
                {

                    QVector4D color = colormap.getColor( map, values[ 3*i ], maxmin[ 0 ], maxmin[ 1 ] );
                    colors.push_back( color.x() );
                    colors.push_back( color.y() );
                    colors.push_back( color.z() );

                }


            }

            else if( option == 2 )
            {
                data.getMaxMinCoordinates( current_property, current_property_type, maxmin );

                for( int i = 0; i < nvalues; ++i )
                {

                    QVector4D color = colormap.getColor( map, values[ 3*i + 1 ], maxmin[ 2 ], maxmin[ 3 ] );
                    colors.push_back( color.x() );
                    colors.push_back( color.y() );
                    colors.push_back( color.z() );

                }


            }

            else if( option == 3 )
            {
                data.getMaxMinCoordinates( current_property, current_property_type, maxmin );

                for( int i = 0; i < nvalues; ++i )
                {

                    QVector4D color = colormap.getColor( map, values[ 3*i + 2 ], maxmin[ 4 ], maxmin[ 5 ] );
                    colors.push_back( color.x() );
                    colors.push_back( color.y() );
                    colors.push_back( color.z() );

                }


            }


        }

    }

    else if( current_property_type == "CELLS" )
    {

        int npoints = data.getNumberofPoints();
        colors.resize( 3*npoints );

        if( ncomp == 1 )
        {
            data.getMaxMinAttribute( current_property, current_property_type, ncomp, maxmin );

            int nvalues = values.size();
            for( int i = 0; i < nvalues; ++i )
            {
                QVector4D color = colormap.getColor( map, values[ i ], maxmin[ 0 ], maxmin[ 1 ] );
                vector< int > vertices;
                getVerticesCell( i, vertices );

                int id0 = vertices[ 0 ];
                color[ 3*id0 ] = color.x();
                color[ 3*id0 + 1 ] = color.y();
                color[ 3*id0 + 2 ] = color.z();

                int id1 = vertices[ 1 ];
                color[ 3*id1 ] = color.x();
                color[ 3*id1 + 1 ] = color.y();
                color[ 3*id1 + 2 ] = color.z();

                int id2 = vertices[ 2 ];
                color[ 3*id2 ] = color.x();
                color[ 3*id2 + 1 ] = color.y();
                color[ 3*id2 + 2 ] = color.z();

                int id3 = vertices[ 3 ];
                color[ 3*id3 ] = color.x();
                color[ 3*id3 + 1 ] = color.y();
                color[ 3*id3 + 2 ] = color.z();

//                for( int j = 0; j < vertices.size(); ++j )
//                {
//                    int id = vertices[ j ];
//                    colors[ 3*id ] = color.x();
//                    colors[ 3*id + 1 ] = color.y();
//                    colors[ 3*id + 2 ] = color.z();

//                }

            }

        }

        else if( ncomp == 3 )
        {
            int nvalues = values.size()/3;

            if( option == 0 )
            {

                data.getMaxMinAttribute( current_property, current_property_type, ncomp, maxmin );

                for( int k = 0; k < nvalues; ++k )
                {
                    float value = values[ 3*k ]*values[ 3*k ] + values[ 3*k + 1 ]*values[ 3*k + 1 ] + values[ 3*k + 2 ]*values[ 3*k + 2 ];
                    QVector4D color = colormap.getColor( map, value, maxmin[ 0 ], maxmin[ 1 ] );
                    vector< int > vertices;
                    getVerticesCell( k, vertices );

                    int id0 = vertices[ 0 ];
                    color[ 3*id0 ] = color.x();
                    color[ 3*id0 + 1 ] = color.y();
                    color[ 3*id0 + 2 ] = color.z();

                    int id1 = vertices[ 1 ];
                    color[ 3*id1 ] = color.x();
                    color[ 3*id1 + 1 ] = color.y();
                    color[ 3*id1 + 2 ] = color.z();

                    int id2 = vertices[ 2 ];
                    color[ 3*id2 ] = color.x();
                    color[ 3*id2 + 1 ] = color.y();
                    color[ 3*id2 + 2 ] = color.z();

                    int id3 = vertices[ 3 ];
                    color[ 3*id3 ] = color.x();
                    color[ 3*id3 + 1 ] = color.y();
                    color[ 3*id3 + 2 ] = color.z();

//                    for( int j = 0; j < vertices.size(); ++j )
//                    {
//                        int id = vertices[ j ];
//                        colors[ 3*id ] = color.x();
//                        colors[ 3*id + 1 ] = color.y();
//                        colors[ 3*id + 2 ] = color.z();

//                    }

                }


            }

            else if( option == 1 )
            {
                data.getMaxMinCoordinates( current_property, current_property_type, maxmin );

                for( int i = 0; i < nvalues; ++i )
                {

                    QVector4D color = colormap.getColor( map, values[ 3*i ], maxmin[ 0 ], maxmin[ 1 ] );
                    vector< int > vertices;
                    getVerticesCell( i, vertices );

                    int id0 = vertices[ 0 ];
                    color[ 3*id0 ] = color.x();
                    color[ 3*id0 + 1 ] = color.y();
                    color[ 3*id0 + 2 ] = color.z();

                    int id1 = vertices[ 1 ];
                    color[ 3*id1 ] = color.x();
                    color[ 3*id1 + 1 ] = color.y();
                    color[ 3*id1 + 2 ] = color.z();

                    int id2 = vertices[ 2 ];
                    color[ 3*id2 ] = color.x();
                    color[ 3*id2 + 1 ] = color.y();
                    color[ 3*id2 + 2 ] = color.z();

                    int id3 = vertices[ 3 ];
                    color[ 3*id3 ] = color.x();
                    color[ 3*id3 + 1 ] = color.y();
                    color[ 3*id3 + 2 ] = color.z();


//                    for( int j = 0; j < vertices.size(); ++j )
//                    {
//                        int id = vertices[ j ];
//                        colors[ 3*id ] = color.x();
//                        colors[ 3*id + 1 ] = color.y();
//                        colors[ 3*id + 2 ] = color.z();

//                    }
                }


            }


            else if( option == 2 )
            {
                data.getMaxMinCoordinates( current_property, current_property_type, maxmin );

                for( int i = 0; i < nvalues; ++i )
                {

                    QVector4D color = colormap.getColor( map, values[ 3*i + 1 ], maxmin[ 2 ], maxmin[ 3 ] );
                    vector< int > vertices;
                    getVerticesCell( i, vertices );

                    int id0 = vertices[ 0 ];
                    color[ 3*id0 ] = color.x();
                    color[ 3*id0 + 1 ] = color.y();
                    color[ 3*id0 + 2 ] = color.z();

                    int id1 = vertices[ 1 ];
                    color[ 3*id1 ] = color.x();
                    color[ 3*id1 + 1 ] = color.y();
                    color[ 3*id1 + 2 ] = color.z();

                    int id2 = vertices[ 2 ];
                    color[ 3*id2 ] = color.x();
                    color[ 3*id2 + 1 ] = color.y();
                    color[ 3*id2 + 2 ] = color.z();

                    int id3 = vertices[ 3 ];
                    color[ 3*id3 ] = color.x();
                    color[ 3*id3 + 1 ] = color.y();
                    color[ 3*id3 + 2 ] = color.z();
//                    for( int j = 0; j < vertices.size(); ++j )
//                    {
//                        int id = vertices[ j ];
//                        colors[ 3*id ] = color.x();
//                        colors[ 3*id + 1 ] = color.y();
//                        colors[ 3*id + 2 ] = color.z();

//                    }

                }


            }

            else if( option == 3 )
            {
                data.getMaxMinCoordinates( current_property, current_property_type, maxmin );

                for( int i = 0; i < nvalues; ++i )
                {

                    QVector4D color = colormap.getColor( map, values[ 3*i + 2 ], maxmin[ 4 ], maxmin[ 5 ] );
                    vector< int > vertices;
                    getVerticesCell( i, vertices );

                    int id0 = vertices[ 0 ];
                    color[ 3*id0 ] = color.x();
                    color[ 3*id0 + 1 ] = color.y();
                    color[ 3*id0 + 2 ] = color.z();

                    int id1 = vertices[ 1 ];
                    color[ 3*id1 ] = color.x();
                    color[ 3*id1 + 1 ] = color.y();
                    color[ 3*id1 + 2 ] = color.z();

                    int id2 = vertices[ 2 ];
                    color[ 3*id2 ] = color.x();
                    color[ 3*id2 + 1 ] = color.y();
                    color[ 3*id2 + 2 ] = color.z();

                    int id3 = vertices[ 3 ];
                    color[ 3*id3 ] = color.x();
                    color[ 3*id3 + 1 ] = color.y();
                    color[ 3*id3 + 2 ] = color.z();
//                    for( int j = 0; j < vertices.size(); ++j )
//                    {
//                        int id = vertices[ j ];
//                        colors[ 3*id ] = color.x();
//                        colors[ 3*id + 1 ] = color.y();
//                        colors[ 3*id + 2 ] = color.z();

//                    }

                }


            }

        }

    }

}


void FlowVisualizationController::setColorMap( std::string colormap )
{
    current_colormap = colormap;
}


std::string FlowVisualizationController::getColorMap()
{
    return current_colormap;
}


void FlowVisualizationController::setPointProperties( vector< std::string > property, vector< int > dim, vector< float > values )
{
    data = VTKData();
}


void FlowVisualizationController::getPointProperties( vector< std::string >& property, vector< int >& dim, vector< float >& values )
{

    if( data.isEmpty() == true ) return;


    vector< std::string > format;
    data.getAttributesPoints( format, property, values );

    int npoints = property.size();

    for( int i = 0; i < npoints; ++i ){

        if( format[ i ].compare( "SCALARS" ) == 0 )
            dim.push_back ( FlowVisualizationController::VERTICESPROPERTY::SCALARS );

        else if( format[ i ].compare( "VECTORS" ) == 0 )
            dim.push_back ( FlowVisualizationController::VERTICESPROPERTY::VECTORS );

    }

}


void FlowVisualizationController::setCellsProperties( vector< std::string > property, vector < int > dim, vector< float > values )
{

}


void FlowVisualizationController::getCellsProperties( vector< std::string >& property, vector< int >& dim, vector< float >& values )
{

    if( data.isEmpty() == true ) return;

    vector < std::string > format;
    data.getAttributesCells( format, property, values );

    int npoints = property.size();

    for( int i = 0; i < npoints; ++i ){

        if( format[ i ].compare( "SCALARS" ) == 0 )
            dim.push_back ( VERTICESPROPERTY::SCALARS );

        else if( format[ i ].compare( "VECTORS" ) == 0 )
            dim.push_back ( VERTICESPROPERTY::VECTORS );

    }
}


void FlowVisualizationController::setCurrentProperty( std::string property, std::string type )
{
    current_property = property;
    current_property_type = type;
}


 void FlowVisualizationController::getCurrentProperty( std::string& property, std::string& type )
{
    property = current_property;
    type = current_property_type;
}

 void FlowVisualizationController::setCurrentProperty( int property )
 {
     vector< std::string > format;
     vector< std::string > name;
     vector< float > values;

     data.getAttributesPoints( format, name, values );
     if( name.empty() == true ) return;
     current_property = name[ property ];
     current_property_type = "POINTS";

 }


void FlowVisualizationController::clear()
{
    /*
     * property_map.clear();
     * data.clear();
     */
}
