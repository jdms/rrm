#include <algorithm>

#include "FlowVisualizationController.h"


FlowVisualizationController::FlowVisualizationController()
{


}


bool FlowVisualizationController::readData()
{
    std::string filename = std::string( "/Users/Clarissa/Downloads/results_example9coarse.vtk" );

    if( data.isEmpty() == false )
        cout << "You should clean before!" << endl;

    bool read_ok = data.readUnstructuredGridFile( filename );
    data.computeMaxMinProperties();
    setCurrentProperty( 0 );

    return read_ok;
}


void FlowVisualizationController::getVerticesCell( int idcell, std::vector< int >& cell_vertices )
{
    if( data.isEmpty() == true ) return;

    VTKData::Cell c;
    data.getCell( idcell, c );
    cell_vertices = c.vertices;

}


void FlowVisualizationController::getTriangles( vector< unsigned int >& triangles )
{

    if( data.isEmpty() == true ) return;


    vector< VTKData::Cell > cells;
    data.getVectorCells( cells );

    int ncells = (int) cells.size();
    for( int i = 0; i < ncells; ++i )
    {

        vector< int > cell_vertices;
        getVerticesCell( i, cell_vertices );
        transformInTriangles( cell_vertices, triangles );

    }


}


void FlowVisualizationController::getWireframe( vector< unsigned int >& lines )
{
    if( data.isEmpty() == true ) return;

    vector< VTKData::Cell > cells;
    data.getVectorCells( cells );

    int ncells = (int) cells.size();
    for( int i = 0; i < ncells; ++i  )
    {

        std::vector< int > cell_vertices;
        getVerticesCell( i, cell_vertices );
        transformInLines( cell_vertices, lines );


    }
}


void FlowVisualizationController::transformInLines( std::vector< int > cell, vector< unsigned int >& triangles )
{

    int nvertices = (int) cell.size();

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

    int nvertices = (int) cell.size();

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


void FlowVisualizationController::addPointProperty( std::string name, std::string format, std::string type, int ncoords, vector< float > values )
{
    FlowProperty p;
    p.setName( name );
    p.setFormat( format );
    p.setType( type );
    p.setNumberofComponents( ncoords );
    p.setValues( values );

    data.addPointFlowProperty( p );

}

void FlowVisualizationController::addCellProperty( std::string name, std::string format, std::string type, int ncoords, vector< float > values )
{
    FlowProperty p;
    p.setName( name );
    p.setFormat( format );
    p.setType( type );
    p.setNumberofComponents( ncoords );
    p.setValues( values );

    data.addCellFlowProperty( p );

}


void FlowVisualizationController::getPointProperty( int id, std::string& name, std::string& format, std::string& type, int& ncoords )
{
    FlowProperty p;

    data.getPointFlowProperty( id, p );

    p.getName( name );
    p.getFormat( format );
    p.getType( type );
    ncoords = p.getNumberofComponents();

}


void FlowVisualizationController::getCellProperty( int id, std::string& name, std::string& format, std::string& type, int& ncoords )
{
    FlowProperty p;

    data.getCellFlowProperty( id, p );

    p.getName( name );
    p.getFormat( format );
    p.getType( type );
    ncoords = p.getNumberofComponents();
}



void FlowVisualizationController::getBoundingBox( float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax )
{
    if( data.isEmpty() == true ) return;

    vector< float > vertices;
    data.getVectorPoints( vertices );

    int nvertices = (int) vertices.size()/3;
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



void FlowVisualizationController::getColors( vector< float >& colors, int option  )
{


    if( current_property_type == "POINTS" )
    {
        if( option != 0 ){
            getCoordinateColors( colors, option );
            return;
        }

        getMagnitudeColors( colors );
        return;
    }
    else if( current_property_type == "CELLS" )
    {
        if( option != 0 ){
            getCoordinateColorsCells( colors, option );
            return;
        }

        getMagnitudeColorsCells( colors );
        return;
    }



}


void FlowVisualizationController::getMagnitudeColors( vector< float >& colors )
{
    if( data.isEmpty() == true ) return;

    COLORMAP map;

    if( current_colormap == "JET" )
        map = COLORMAP::JET;
    else if( current_colormap == "CONSTANT" )
        map = COLORMAP::CONSTANT;
    else
        map = COLORMAP::JET;

    FlowProperty p;
    data.getFlowProperty( current_property, current_property_type, p );

    vector< float > values;
    p.getValues( values );


    float max = p.getMaximum();
    float min = p.getMinimum();

    int ncoords = p.getNumberofComponents();
    if( ncoords == 1 )
    {
        int nvalues = (int) values.size();
        for( int i = 0; i < nvalues; ++i )
        {
            QVector4D color = colormap.getColor( map, values[ i ], min, max );
            colors.push_back( color.x() );
            colors.push_back( color.y() );
            colors.push_back( color.z() );

        }
    }
    else if( ncoords == 3 )
    {

        int nvalues = (int) values.size()/3;
        for( int i = 0; i < nvalues; ++i )
        {

            float value = values[ 3*i ]*values[ 3*i ] + values[ 3*i + 1 ]*values[ 3*i + 1 ] + values[ 3*i + 2 ]*values[ 3*i + 2 ];
            QVector4D color = colormap.getColor( map, value, min, max );
            colors.push_back( color.x() );
            colors.push_back( color.y() );
            colors.push_back( color.z() );

        }
    }

}


void FlowVisualizationController::getCoordinateColors( vector< float >& colors, int option )
{
    if( data.isEmpty() == true ) return;

    COLORMAP map;

    if( current_colormap == "JET" )
        map = COLORMAP::JET;
    else if( current_colormap == "CONSTANT" )
        map = COLORMAP::CONSTANT;
    else
        map = COLORMAP::JET;

    FlowProperty p;
    data.getFlowProperty( current_property, current_property_type, p );

    vector< float > values;
    p.getValues( values );
    int nvalues = (int) values.size()/3;

    float xmin, xmax, ymin, ymax, zmin, zmax;
    p.getMaxMinCoordinateProperty( xmin, xmax, ymin, ymax, zmin, zmax );


    switch ( option )
    {
        case 1:
        {
            for( int i = 0; i < nvalues; ++i )
            {
                QVector4D color = colormap.getColor( map, values[ 3*i ], xmin, xmax );
                colors.push_back( color.x() );
                colors.push_back( color.y() );
                colors.push_back( color.z() );
            }
        }
        break;
        case 2:
        {
            for( int i = 0; i < nvalues; ++i )
            {

                QVector4D color = colormap.getColor( map, values[ 3*i + 1 ], ymin, ymax );
                colors.push_back( color.x() );
                colors.push_back( color.y() );
                colors.push_back( color.z() );
            }
        }

            break;
        case 3:
        {
            for( int i = 0; i < nvalues; ++i )
            {

                QVector4D color = colormap.getColor( map, values[ 3*i + 2 ], zmin, zmax );
                colors.push_back( color.x() );
                colors.push_back( color.y() );
                colors.push_back( color.z() );

            }

        }
        break;


        default:
        break;

    };


}


void FlowVisualizationController::getMagnitudeColorsCells( vector< float >& colors )
{
    if( data.isEmpty() == true ) return;

    COLORMAP map;

    if( current_colormap == "JET" )
        map = COLORMAP::JET;
    else if( current_colormap == "CONSTANT" )
        map = COLORMAP::CONSTANT;
    else
        map = COLORMAP::JET;

    FlowProperty p;
    data.getFlowProperty( current_property, current_property_type, p );

    vector< float > values;
    p.getValues( values );


    int npoints = data.getNumberofPoints();
    colors.resize( 3*npoints );

    float max = p.getMaximum();
    float min = p.getMinimum();

    int ncoords = p.getNumberofComponents();
    if( ncoords == 1 )
    {

        int nvalues = (int) values.size();
        for( int i = 0; i < nvalues; ++i )
        {
            QVector4D color = colormap.getColor( map, values[ i ], min, max );
            vector< int > vertices;
            getVerticesCell( i, vertices );

            int id0 = vertices[ 0 ];
            colors[ 3*id0 ] = color.x();
            colors[ 3*id0 + 1 ] = color.y();
            colors[ 3*id0 + 2 ] = color.z();

            int id1 = vertices[ 1 ];
            colors[ 3*id1 ] = color.x();
            colors[ 3*id1 + 1 ] = color.y();
            colors[ 3*id1 + 2 ] = color.z();

            int id2 = vertices[ 2 ];
            colors[ 3*id2 ] = color.x();
            colors[ 3*id2 + 1 ] = color.y();
            colors[ 3*id2 + 2 ] = color.z();

            int id3 = vertices[ 3 ];
            colors[ 3*id3 ] = color.x();
            colors[ 3*id3 + 1 ] = color.y();
            colors[ 3*id3 + 2 ] = color.z();

        }

    }
    else if( ncoords == 3 )
    {
        int nvalues = (int) values.size()/3;

        for( int k = 0; k < nvalues; ++k )
        {
            float value = values[ 3*k ]*values[ 3*k ] + values[ 3*k + 1 ]*values[ 3*k + 1 ] + values[ 3*k + 2 ]*values[ 3*k + 2 ];
            QVector4D color = colormap.getColor( map, value, min, max );
            vector< int > vertices;
            getVerticesCell( k, vertices );

            int id0 = vertices[ 0 ];
            colors[ 3*id0 ] = color.x();
            colors[ 3*id0 + 1 ] = color.y();
            colors[ 3*id0 + 2 ] = color.z();

            int id1 = vertices[ 1 ];
            colors[ 3*id1 ] = color.x();
            colors[ 3*id1 + 1 ] = color.y();
            colors[ 3*id1 + 2 ] = color.z();

            int id2 = vertices[ 2 ];
            colors[ 3*id2 ] = color.x();
            colors[ 3*id2 + 1 ] = color.y();
            colors[ 3*id2 + 2 ] = color.z();

            int id3 = vertices[ 3 ];
            colors[ 3*id3 ] = color.x();
            colors[ 3*id3 + 1 ] = color.y();
            colors[ 3*id3 + 2 ] = color.z();


        }


    }


}


void FlowVisualizationController::getCoordinateColorsCells( vector< float >& colors, int option )
{

    if( data.isEmpty() == true ) return;

    COLORMAP map;

    if( current_colormap == "JET" )
        map = COLORMAP::JET;
    else if( current_colormap == "CONSTANT" )
        map = COLORMAP::CONSTANT;
    else
        map = COLORMAP::JET;

    FlowProperty p;
    data.getFlowProperty( current_property, current_property_type, p );

    vector< float > values;
    p.getValues( values );
    int nvalues = (int) values.size()/3;

    int npoints = data.getNumberofPoints();
    colors.resize( 3*npoints );

    vector< float > maxmin;
    float xmin, xmax, ymin, ymax, zmin, zmax;
    p.getMaxMinCoordinateProperty( xmin, xmax, ymin, ymax, zmin, zmax );

    switch( option )
    {
        case 1:
        {
            for( int i = 0; i < nvalues; ++i )
            {

                QVector4D color = colormap.getColor( map, values[ 3*i ], xmin, xmax );
                vector< int > vertices;
                getVerticesCell( i, vertices );

                int id0 = vertices[ 0 ];
                colors[ 3*id0 ] = color.x();
                colors[ 3*id0 + 1 ] = color.y();
                colors[ 3*id0 + 2 ] = color.z();

                int id1 = vertices[ 1 ];
                colors[ 3*id1 ] = color.x();
                colors[ 3*id1 + 1 ] = color.y();
                colors[ 3*id1 + 2 ] = color.z();

                int id2 = vertices[ 2 ];
                colors[ 3*id2 ] = color.x();
                colors[ 3*id2 + 1 ] = color.y();
                colors[ 3*id2 + 2 ] = color.z();

                int id3 = vertices[ 3 ];
                colors[ 3*id3 ] = color.x();
                colors[ 3*id3 + 1 ] = color.y();
                colors[ 3*id3 + 2 ] = color.z();

            }

        }
        break;

        case 2:
        {
            for( int i = 0; i < nvalues; ++i )
            {

                QVector4D color = colormap.getColor( map, values[ 3*i + 1 ], ymin, ymax );
                vector< int > vertices;
                getVerticesCell( i, vertices );

                int id0 = vertices[ 0 ];
                colors[ 3*id0 ] = color.x();
                colors[ 3*id0 + 1 ] = color.y();
                colors[ 3*id0 + 2 ] = color.z();

                int id1 = vertices[ 1 ];
                colors[ 3*id1 ] = color.x();
                colors[ 3*id1 + 1 ] = color.y();
                colors[ 3*id1 + 2 ] = color.z();

                int id2 = vertices[ 2 ];
                colors[ 3*id2 ] = color.x();
                colors[ 3*id2 + 1 ] = color.y();
                colors[ 3*id2 + 2 ] = color.z();

                int id3 = vertices[ 3 ];
                colors[ 3*id3 ] = color.x();
                colors[ 3*id3 + 1 ] = color.y();
                colors[ 3*id3 + 2 ] = color.z();

            }

        }
        break;

        case 3:
        {
            for( int i = 0; i < nvalues; ++i )
            {

                QVector4D color = colormap.getColor( map, values[ 3*i + 2 ], zmin, zmax );
                vector< int > vertices;
                getVerticesCell( i, vertices );

                int id0 = vertices[ 0 ];
                colors[ 3*id0 ] = color.x();
                colors[ 3*id0 + 1 ] = color.y();
                colors[ 3*id0 + 2 ] = color.z();

                int id1 = vertices[ 1 ];
                colors[ 3*id1 ] = color.x();
                colors[ 3*id1 + 1 ] = color.y();
                colors[ 3*id1 + 2 ] = color.z();

                int id2 = vertices[ 2 ];
                colors[ 3*id2 ] = color.x();
                colors[ 3*id2 + 1 ] = color.y();
                colors[ 3*id2 + 2 ] = color.z();

                int id3 = vertices[ 3 ];
                colors[ 3*id3 ] = color.x();
                colors[ 3*id3 + 1 ] = color.y();
                colors[ 3*id3 + 2 ] = color.z();

            }
        }
        break;

    };

}


void FlowVisualizationController::getPointMaxMin(  FlowProperty p, vector< float >& maxmin )
{


    vector< float > values;
    p.getValues( values );

    int ncoords = p.getNumberofComponents();
    if( ncoords == 1 )
    {
        maxmin.push_back( p.getMinimum() );
        maxmin.push_back( p.getMaximum() );
    }

    else if( ncoords == 3 )
    {

        float xmin, xmax, ymin, ymax, zmin, zmax;
        p.getMaxMinCoordinateProperty( xmin, xmax, ymin, ymax, zmin, zmax );

        maxmin.push_back( p.getMinimum() );
        maxmin.push_back( p.getMaximum() );

        maxmin.push_back( xmin );
        maxmin.push_back( xmax );

        maxmin.push_back( ymin );
        maxmin.push_back( ymax );

        maxmin.push_back( zmin );
        maxmin.push_back( zmax );

    }

}


void FlowVisualizationController::getCellMaxMin(  vector< float >& maxmin )
{
    FlowProperty p;
    data.getFlowProperty( current_property, "CELL", p );

    vector< float > values;
    p.getValues( values );

    int ncoords = p.getNumberofComponents();
    if( ncoords == 1 )
    {
        maxmin.push_back( p.getMinimum() );
        maxmin.push_back( p.getMaximum() );
    }

    else if( ncoords == 3 )
    {

        float xmin, xmax, ymin, ymax, zmin, zmax;
        p.getMaxMinCoordinateProperty( xmin, xmax, ymin, ymax, zmin, zmax );

        maxmin.push_back( p.getMinimum() );
        maxmin.push_back( p.getMaximum() );

        maxmin.push_back( xmin );
        maxmin.push_back( xmax );

        maxmin.push_back( ymin );
        maxmin.push_back( ymax );

        maxmin.push_back( zmin );
        maxmin.push_back( zmax );

    }

}


 void FlowVisualizationController::setCurrentProperty( int id )
 {

     int npproperties = data.getNumberofPointsProperties();
     if( id >= 0 && id < npproperties )
     {
         FlowProperty p;
         data.getPointFlowProperty( id, p );
         p.getName( current_property );
         current_property_type = "POINTS";
         return;
     }

     int ncproperties = data.getNumberofCellsProperties();
     if( id >= 0 && id < ncproperties )
     {
         FlowProperty p;
         data.getCellFlowProperty( id, p );
         p.getName( current_property );
         current_property_type = "CELLS";
         return;
     }

 }


void FlowVisualizationController::clear()
{
    /*
     * property_map.clear();
     * data.clear();
     */
}


void FlowVisualizationController::openSurfaceFile( std::string filename )
{

    region.readsurfacemeshPOLY( (char *)filename.c_str() );
}


void FlowVisualizationController::computeVolumetricMesh()
{
    region.buildtetrahedralmesh();
    region.flowpreparation();
}


void FlowVisualizationController::computeFlowProperties()
{
//    region.flowpreparation();
    region.computepressure();
    region.computevelocity();
    region.flowdiagnostics();
}


void FlowVisualizationController::getSurface( vector< float > points, vector< unsigned int > edges )
{
    double *point_list;
    int *edge_list;
    int npoints = 0;
    int nedges = 0;
    region.getTrianglesList( npoints, &point_list, nedges, &edge_list );

    for( int i = 0; i < npoints; ++i )
        points.push_back( point_list[ i ] );

    for( int i = 0; i < nedges; ++i )
        edges.push_back( ( unsigned int ) edge_list[ i ] );
}


void FlowVisualizationController::getSurfaceBoundingBox( float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax )
{
    if( data.isEmpty() == true ) return;


    vector< float > vertices;
    vector< unsigned int > edges;

    getSurface( vertices, edges );

    int npoints = (int) vertices.size()/3;
    if( npoints == 0 ) return;

    xmin = vertices[ 0 ]; xmax = vertices[ 0 ];
    ymin = vertices[ 1 ]; ymax = vertices[ 1 ];
    zmin = vertices[ 2 ]; zmax = vertices[ 2 ];

    for( int i = 0; i < npoints; i++ )
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


void FlowVisualizationController::updatePropertiesNamesVector()
{
    vector< FlowProperty > vector_props;
    vector< std::string > names_ppoints;
    vector< std::string > names_pcells;


    data.getPointsFlowProperties( vector_props );

    int count = 0;
    int nprops = vector_props.size();
    for( int i = 0; i < nprops; ++i )
    {
        FlowProperty& p = vector_props[ i ];
        std::string name;
        p.getName( name );
        names_ppoints.push_back( name );

        property_map[ count ] = p;
        count++;

    }

    data.getCellsFlowProperties( vector_props );

    nprops = vector_props.size();
    for( int i = 0; i < nprops; ++i )
    {
        FlowProperty& p = vector_props[ i ];
        std::string name;
        p.getName( name );
        names_pcells.push_back( name );

        property_map[ count ] = p;
        count++;
    }

    emit updateComboBox( names_ppoints, names_pcells );
}


void FlowVisualizationController::computePressure()
{
    region.computepressure();
    updatePropertiesNamesVector();


}


void FlowVisualizationController::computeVelocity()
{
    region.computevelocity();
    updatePropertiesNamesVector();
}


void FlowVisualizationController::computeTOF()
{
    region.flowdiagnostics();
    updatePropertiesNamesVector();
}


void FlowVisualizationController::selectFlowProperty(int id, bool& option  )
{
    FlowProperty &p = property_map[ id ];
    option = false;

    std::string type;
    p.getType( type );

    if( type.compare( "VECTORS" ) == 0 )
        option = true;

}


FlowProperty &FlowVisualizationController::getPropertyfromMap( int id )
{
    return property_map[ id ];
}


/*int n = in.numberoffacets;
    tetgenio::facet* list = in.facetlist;
    for( int i = 0; i < n; ++i )
    {
        tetgenio::facet f = list[ i ];
        int np = f.numberofpolygons;
        tetgenio::polygon *pl = f.polygonlist;
        for( int j = 0; j < np; ++j )
        {
            tetgenio::polygon p = pl[ j ];
            int nv = p.numberofvertices;
            int *verticeslist = p.vertexlist;

            for( int k = 0; k < nv; ++k )
            {
                int id = verticeslist[ k ];
                int idj = 0;
            }
        }
    }*/
