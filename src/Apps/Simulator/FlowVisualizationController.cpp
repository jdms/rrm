#include <algorithm>

#include "FlowVisualizationController.h"


FlowVisualizationController::FlowVisualizationController()
{
    clear();
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

    if( nvertices == FlowVisualizationController::VERTICESSHAPE::NTETRAHEDRON )
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

    if( nvertices == FlowVisualizationController::VERTICESSHAPE::NTETRAHEDRON )
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
    FlowProperty p = data.getPointFlowProperty( id );

    //data.getPointFlowProperty( id, p );

    p.getName( name );
    p.getFormat( format );
    p.getType( type );
    ncoords = p.getNumberofComponents();

}


void FlowVisualizationController::getCellProperty( int id, std::string& name, std::string& format, std::string& type, int& ncoords )
{
    FlowProperty p = data.getCellFlowProperty( id );

//    data.getCellFlowProperty( id, p );

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

    COLORMAP map;

    if( current_colormap == "CONSTANT" )
    {        
            map = COLORMAP::CONSTANT;
            getColorConstant( map, colors );
            return;
    }
    else if( current_colormap == "JET" )
        map = COLORMAP::JET;
    else
        map = COLORMAP::JET;


    if( current_property_type == "POINTS" )
    {
        if( option != 0 ){
            getCoordinateColors( map, colors, option );
            return;
        }

        getMagnitudeColors( map, colors );
        return;
    }
    else if( current_property_type == "CELLS" )
    {
        if( option != 0 ){
            getCoordinateColorsCells( map, colors, option );
            return;
        }

        getMagnitudeColorsCells( map, colors );
        return;
    }


}

void FlowVisualizationController::getSurfaceColors( vector< float >& colors  )
{

    COLORMAP map;

    if( current_colormap == "CONSTANT" )
       map = COLORMAP::CONSTANT;

    else if( current_colormap == "JET" )
        map = COLORMAP::JET;
    else
        map = COLORMAP::JET;

    int nvalues = (int) surface_file.numberofpoints;
    for( int i = 0; i < nvalues; ++i )
    {
        QVector4D color = colormap.getColor( map, 0, 0, 0 );
        colors.push_back( color.x() );
        colors.push_back( color.y() );
        colors.push_back( color.z() );

    }

}


void FlowVisualizationController::getColorConstant( COLORMAP map, vector< float >& colors )
{

    int nvalues = (int) data.getNumberofPoints();    
    for( int i = 0; i < nvalues; ++i )
    {
        QVector4D color = colormap.getColor( map, 0, 0, 0 );
        colors.push_back( color.x() );
        colors.push_back( color.y() );
        colors.push_back( color.z() );

    }

}


void FlowVisualizationController::getMagnitudeColors( COLORMAP map, vector< float >& colors )
{
    if( data.isEmpty() == true ) return;

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


    /*
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
    */
}


void FlowVisualizationController::getCoordinateColors( COLORMAP map, vector< float >& colors, int option )
{
    if( data.isEmpty() == true ) return;

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


void FlowVisualizationController::getMagnitudeColorsCells( COLORMAP map, vector< float >& colors )
{
    if( data.isEmpty() == true ) return;

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


void FlowVisualizationController::getCoordinateColorsCells( COLORMAP map, vector< float >& colors, int option )
{

    if( data.isEmpty() == true ) return;

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
         FlowProperty p = data.getPointFlowProperty( id );
//         data.getPointFlowProperty( id, p );
         p.getName( current_property );
         current_property_type = "POINTS";
         return;
     }

     int ncproperties = data.getNumberofCellsProperties();
     if( id >= 0 && id < ncproperties )
     {
         FlowProperty p = data.getCellFlowProperty( id );
//         data.getCellFlowProperty( id, p );
         p.getName( current_property );
         current_property_type = "CELLS";
         return;
     }

 }


void FlowVisualizationController::clear()
{
    current_colormap = "CONSTANT";
    property_map.clear();
    data.clear();
}


void FlowVisualizationController::openSurfaceFile( std::string filename )
{

    region.readsurfacemeshPOLY( (char *)filename.c_str() );
    getTetgenioObject();

}

void FlowVisualizationController::openUserInputFile( std::string filename )
{

    region.tolerance(0.00001, 0.00001);
    region.userinput( (char *) filename.c_str() );

}

void FlowVisualizationController::computeVolumetricMesh()
{
    region.buildtetrahedralmesh();

    std::vector< NODE > nodes = region.getnodelist();
    std::vector< TETRAHEDRON > elements = region.getelementlist();

    loadSurfaceData( nodes, elements );

    region.flowpreparation();
}


void FlowVisualizationController::computeFlowProperties()
{
//    region.flowpreparation();
    region.computepressure();
    region.computevelocity();
    region.flowdiagnostics();
}



void FlowVisualizationController::getSurfaceBoundingBox( float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax )
{


    vector< float > vertices;
    getPointsSurface( vertices );

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
    property_map.clear();

    int count = 0;
    int nprops = (int)vector_props.size();
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

    nprops = (int)vector_props.size();
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
    loadPressureData();
    updatePropertiesNamesVector();


}


void FlowVisualizationController::computeVelocity()
{
    region.computevelocity();
    loadVelocityData();
    updatePropertiesNamesVector();
}


void FlowVisualizationController::computeTOF()
{
    region.flowdiagnostics();
    loadTOFData();
    updatePropertiesNamesVector();
}


void FlowVisualizationController::selectFlowProperty(int id, bool& option  )
{
    FlowProperty &p = property_map[ id ];
    option = false;

    std::string format;
    p.getFormat( format );

    if( format.compare( "VECTORS" ) == 0 )
        option = true;

}


FlowProperty &FlowVisualizationController::getPropertyfromMap( int id )
{
    return property_map[ id ];
}


void FlowVisualizationController::getTetgenioObject()
{
    region.getTetgenioObject( surface_file );
}



void FlowVisualizationController::getTrianglesSurface( vector< unsigned int >& triangles )
{
    int nfaces = surface_file.numberoffacets;
    tetgenio::facet* faces = surface_file.facetlist;
    for( int i = 0; i < nfaces; ++i )
    {
        tetgenio::facet f = faces[ i ];

        int npolygons = f.numberofpolygons;
        tetgenio::polygon *polygons = f.polygonlist;

        for( int j = 0; j < npolygons; ++j )
        {
            tetgenio::polygon p = polygons[ j ];
            int nvertices = p.numberofvertices;
            int *vertices = p.vertexlist;

            getTrianglesfromRectangles( nvertices, vertices, triangles );
        }

    }

}


void FlowVisualizationController::getTrianglesfromRectangles( int nvertices, int* verticeslist, vector< unsigned int >& triangles  )
{
    if( nvertices != 4 ) return;

    triangles.push_back( ( unsigned int )verticeslist[ 0 ] );
    triangles.push_back( ( unsigned int )verticeslist[ 1 ] );
    triangles.push_back( ( unsigned int )verticeslist[ 3 ] );

    triangles.push_back( ( unsigned int )verticeslist[ 1 ] );
    triangles.push_back( ( unsigned int )verticeslist[ 2 ] );
    triangles.push_back( ( unsigned int )verticeslist[ 3 ] );

}


void FlowVisualizationController::getWireframeSurface( vector< unsigned int >& lines )
{

   int nfaces = surface_file.numberoffacets;
   tetgenio::facet* faces = surface_file.facetlist;
   for( int i = 0; i < nfaces; ++i )
   {
       tetgenio::facet f = faces[ i ];

       int npolygons = f.numberofpolygons;
       tetgenio::polygon *polygons = f.polygonlist;

       for( int j = 0; j < npolygons; ++j )
       {
           tetgenio::polygon p = polygons[ j ];
           int nvertices = p.numberofvertices;
           int *vertices = p.vertexlist;

           if( nvertices != 4 ) continue;

           lines.push_back( vertices[ 0 ] );
           lines.push_back( vertices[ 1 ] );

           lines.push_back( vertices[ 1 ] );
           lines.push_back( vertices[ 2 ] );

           lines.push_back( vertices[ 2 ] );
           lines.push_back( vertices[ 3 ] );

           lines.push_back( vertices[ 3 ] );
           lines.push_back( vertices[ 0 ] );

       }

   }

}


void FlowVisualizationController::getPointsSurface( vector< float >& vertices )
{
   int npoints = surface_file.numberofpoints;
   double *points = surface_file.pointlist;

   for( int i = 0; i < npoints; ++i )
   {
       vertices.push_back( (float)points[ 3*i ] );
       vertices.push_back( (float)points[ 3*i + 1 ] );
       vertices.push_back( (float)points[ 3*i + 2 ] );
   }

}


void FlowVisualizationController::loadPressureData()
{
    std::vector< NODE > nodes = region.getnodelist();

    vector< float > values;
    int npoints = (int)nodes.size();
    for( int i = 0; i < npoints; ++i )
        values.push_back( (float) nodes[ i ].P() );

    auto itmin = std::min_element( values.begin(), values.end() );
    auto itmax = std::max_element( values.begin(), values.end() );

    int idmin  = std::distance( values.begin(), itmin );
    int idmax = std::distance( values.begin(), itmax );

    int id = data.getNumberofPointsProperties();

    FlowProperty p;

    p.setName( "Pressure" );
    p.setFormat( "SCALARS" );
    p.setType( "POINTS" );
    p.setNumberofComponents( 1 );
    p.setValues( values );
    p.setId( id );
    p.setMinimum( values[ idmin ] );
    p.setMaximum( values[ idmax ] );

    data.addPointFlowProperty( p );

}


void FlowVisualizationController::loadTOFData()
{
    std::vector< NODE > nodes = region.getnodelist();

    vector< float > values;
    int npoints = (int)nodes.size();
    for( int i = 0; i < npoints; ++i )
        values.push_back( (float) nodes[ i ].tof() );

    auto itmin = std::min_element( values.begin(), values.end() );
    auto itmax = std::max_element( values.begin(), values.end() );

    int idmin  = std::distance( values.begin(), itmin );
    int idmax = std::distance( values.begin(), itmax );

    int id = data.getNumberofPointsProperties();

    FlowProperty p;

    p.setName( "TOF -- Points" );
    p.setFormat( "SCALARS" );
    p.setType( "POINTS" );
    p.setNumberofComponents( 1 );
    p.setValues( values );
    p.setId( id );
    p.setMinimum( values[ idmin ] );
    p.setMaximum( values[ idmax ] );

    data.addPointFlowProperty( p );



    std::vector< TETRAHEDRON > elements = region.getelementlist();

    vector< float > values1;
    int ncells = (int)elements.size();
    for( int i = 0; i < ncells; ++i )
        values1.push_back( (float) elements[ i ].tof() );


    itmin = std::min_element( values1.begin(), values1.end() );
    itmax = std::max_element( values1.begin(), values1.end() );

    idmin  = std::distance( values1.begin(), itmin );
    idmax = std::distance( values1.begin(), itmax );

    id = data.getNumberofCellsProperties();

    FlowProperty p1;

    p1.setName( "TOF -- Cells" );
    p1.setFormat( "SCALARS" );
    p1.setType( "CELLS" );
    p1.setNumberofComponents( 1 );
    p1.setValues( values1 );
    p1.setId( id );
    p1.setMinimum( values1[ idmin ] );
    p1.setMaximum( values1[ idmax ] );

    data.addCellFlowProperty( p1 );

}


void FlowVisualizationController::loadVelocityData()
{

    std::vector< NODE > nodes = region.getnodelist();

    vector< float > values;
    int npoints = (int)nodes.size();
    for( int i = 0; i < npoints; ++i )
    {
        values.push_back( (float) nodes[ i ].Ux() );
        values.push_back( (float) nodes[ i ].Uy() );
        values.push_back( (float) nodes[ i ].Uz() );
    }

    int nvalues = (int) npoints/3;

    float min = values[ 0 ]*values[ 0 ] + values[ 1 ]*values[ 1 ] + values[ 2 ]*values[ 2 ];
    float max = min;

    float minx = values[ 0 ], miny = values[ 1 ], minz = values[ 2 ];
    float maxx = minx, maxy = miny, maxz = minz;


    for( int j = 3; j < nvalues; ++j )
    {
        float norm =  values[ 3*j ]*values[ 3*j ] + values[ 3*j + 1 ]*values[ 3*j + 1 ] + values[ 3*j + 2 ]*values[ 3*j + 2 ];

        if( norm <= min ) min = norm;
        if( norm >= max ) max = norm;

        if( values[ 3*j ] <= minx ) minx = values[ 3*j ];
        if( values[ 3*j ] >= maxx ) maxx = values[ 3*j ];

        if( values[ 3*j + 1 ] <= miny ) miny = values[ 3*j + 1];
        if( values[ 3*j + 1 ] >= maxy ) maxy = values[ 3*j + 1 ];

        if( values[ 3*j + 2 ] <= minz ) minz = values[ 3*j + 2 ];
        if( values[ 3*j + 2 ] >= maxz ) maxz = values[ 3*j + 2 ];
    }


    int id = data.getNumberofPointsProperties();


    FlowProperty p1;

    p1.setName( "Velocity -- Points" );
    p1.setFormat( "VECTORS" );
    p1.setType( "POINTS" );
    p1.setNumberofComponents( 3 );
    p1.setValues( values );
    p1.setId( id );
    p1.setMinimum( min );
    p1.setMaximum( max );

    p1.addMinimumCoordinate( minx );
    p1.addMaximumCoordinate( maxx );

    p1.addMinimumCoordinate( miny );
    p1.addMaximumCoordinate( maxy );

    p1.addMinimumCoordinate( minz );
    p1.addMaximumCoordinate( maxz );

    data.addPointFlowProperty( p1 );



    std::vector< TETRAHEDRON > elements = region.getelementlist();

    vector< float > values1;
    int ncells = (int)elements.size();
    for( int i = 0; i < ncells; ++i ){
        values1.push_back( (float) elements[ i ].Ux() );
        values1.push_back( (float) elements[ i ].Uy() );
        values1.push_back( (float) elements[ i ].Uz() );
    }

    nvalues = (int) ncells/3;

    min = values1[ 0 ]*values1[ 0 ] + values1[ 1 ]*values1[ 1 ] + values1[ 2 ]*values1[ 2 ];
    max = min;

    minx = values1[ 0 ], miny = values1[ 1 ], minz = values1[ 2 ];
    maxx = minx, maxy = miny, maxz = minz;


    for( int j = 3; j < nvalues; ++j )
    {
        float norm =  values1[ 3*j ]*values1[ 3*j ] + values1[ 3*j + 1 ]*values1[ 3*j + 1 ] + values1[ 3*j + 2 ]*values1[ 3*j + 2 ];

        if( norm <= min ) min = norm;
        if( norm >= max ) max = norm;

        if( values1[ 3*j ] <= minx ) minx = values1[ 3*j ];
        if( values1[ 3*j ] >= maxx ) maxx = values1[ 3*j ];

        if( values1[ 3*j + 1 ] <= miny ) miny = values1[ 3*j + 1];
        if( values1[ 3*j + 1 ] >= maxy ) maxy = values1[ 3*j + 1 ];

        if( values1[ 3*j + 2 ] <= minz ) minz = values1[ 3*j + 2 ];
        if( values1[ 3*j + 2 ] >= maxz ) maxz = values1[ 3*j + 2 ];
    }


    id = data.getNumberofCellsProperties();

    FlowProperty p;

    p.setName( "Velocity -- Cells" );
    p.setFormat( "VECTORS" );
    p.setType( "CELLS" );
    p.setNumberofComponents( 3 );
    p.setValues( values1 );
    p.setMinimum( min );
    p.setMaximum( max );

    p.addMinimumCoordinate( minx );
    p.addMaximumCoordinate( maxx );

    p.addMinimumCoordinate( miny );
    p.addMaximumCoordinate( maxy );

    p.addMinimumCoordinate( minz );
    p.addMaximumCoordinate( maxz );

    data.addCellFlowProperty( p );


}

void FlowVisualizationController::loadSurfaceData( std::vector< NODE > nodes, std::vector< TETRAHEDRON > elements )
{

    int npoints = (int)nodes.size();
    int ncells = (int)elements.size();

    for( int i = 0; i < npoints; ++i )
        data.addVectorPoint( nodes[ i ].x(), nodes[ i ].y(), nodes[ i ].z() );

    for( int i = 0; i < ncells; ++i )
    {
        TETRAHEDRON tet = elements[ i ];
        vector< int > vertices;
        vertices.push_back( tet.node( 0 ) );
        vertices.push_back( tet.node( 1 ) );
        vertices.push_back( tet.node( 2 ) );
        vertices.push_back( tet.node( 3 ) );

        data.addCell( i, 10, vertices );
    }

    data.setEmpty( false );

}

void FlowVisualizationController::getUserInput( std::string file_user, std::string surface_file, float tol1, float tol2 )
{

    region.tolerance( tol1, tol2 );
    region.userinput( (char *) file_user.c_str() );
    region.readsurfacemeshPOLY( (char *)surface_file.c_str() );
    getTetgenioObject();


}

void FlowVisualizationController::exportFile( std::string filename )
{
    data.writeFile( filename );
}

