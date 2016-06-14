#include "VTKData.h"

VTKData::VTKData()
{    
    is_empty = true;
}


bool VTKData::isEmpty()
{
    return is_empty;
}


bool VTKData::getHeader( QRegExp RegExp, QString line, vector< std::string >& data )
{
    int pos = RegExp.indexIn( line );
    if( pos == - 1 )
        return false;

    QStringList list = RegExp.capturedTexts();

    QStringList::iterator it = list.begin();
    while (it != list.end()) {
        std::string text = (*it).toLocal8Bit().constData();
        data.push_back( text );
        ++it;
    }

    return true;
}


bool VTKData::getValues( QRegExp RegExp, QString line, vector< std::string > &data )
{

    QStringList list = line.split( RegExp, QString::SkipEmptyParts );
    if( list.empty() == true ) return false;

    QStringList::iterator it = list.begin();
    while (it != list.end()) {
        std::string text = (*it).toLocal8Bit().constData();
        data.push_back( text );
        ++it;
    }

    return true;

}


bool VTKData::readUnstructuredGridFile( std::string& filename )
{

    std::ifstream file;
    file.open( filename.c_str() );
    if( !file.is_open() ) return false;


    std::cout << "-- Begin: parsing file." << std::endl;


    int error = 0;
    FILE_STATES state =  FILE_STATES::HEADER0;


    std::string line;
    std::getline( file, line );

    while ( !file.eof() ) {

        int local_error = parseVTKData_new( file, state, /*qline*/line );


        if( local_error != 2 ){
            std::getline( file, line );
            error = local_error;
        }

    }


    is_empty = false;

    if( error == 0 )
    {
        std::cout << "-- Error: parsing fail." << std::endl << std::endl;
        return false;
    }

    std::cout << "-- End: parsing ok." << std::endl << std::endl;


    return true;

}


int VTKData::parseVTKData_new( std::ifstream& file, FILE_STATES &state, std::string&/*QString*/ line )
{
    const QRegExp EXPHEADER0( "(?:\\s*)(?:vtk)(?:\\s*)(?:DataFile)(?:\\s*)(?:Version)(?:\\s*)(\\d*\\.\\d*)" );
    const QRegExp EXPFORMATFILE( "\\b(ASCII|BINARY)\\b" );
    const QRegExp EXPDATASET( "(?:\\s*)(?:DATASET)(?:\\s*)\\b(STRUCTURED_POINTS|STRUCTURED_GRID|UNSTRUCTURED_GRID|POLYDATA|RECTILINEAR_GRID|FIELD)\\b" );

    const QRegExp EXPDATATYPE( "\\b(POINTS|VERTICES|LINES|POLYGONS|TRIANGLE_STRIPS)\\b(?:\\s*)(\\d+)" );
    const QRegExp EXPCELLINF( "(?:\\s*)(?:CELLS)(?:\\s*)(\\d+)(?:\\s*)(\\d+)" );
    const QRegExp EXPCELLTYPES( "(?:\\s*)(?:CELL_TYPES)(?:\\s*)(\\d+)(?:\\s*)" );

    const QRegExp EXPATTRIBUTEDATA( "(?:\\s*)(?:POINT_DATA)(?:\\s*)(\\d+)(?:\\s*)" );
    const QRegExp EXPLOOKUPTABLE( "(?:\\s*)(?:LOOKUP_TABLE)(?:\\s*)(\.+)(?:\\s*)" );
    const QRegExp EXPCELLDATAINF( "(?:\\s*)(?:CELL_DATA)(?:\\s*)(\\d+)(?:\\s*)" );

    const QRegExp EXPKVALUES( "\\s+" );

    QString qline( line.c_str() );
    vector< std::string > data;

    switch( state )
    {
        case FILE_STATES::HEADER0:
        {
            bool catched_data = getHeader( EXPHEADER0, qline, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != FILE_NARGUMENTS::N_HEADER0 ) )
                return 0;

            file_version = data[ 1 ].c_str();
            state = FILE_STATES::HEADER1;

        }
        break;

        case FILE_STATES::HEADER1:
        {
            file_comments = qline.toStdString();
            state = FILE_STATES::HEADER2;

        }
        break;

        case FILE_STATES::HEADER2:
        {

            bool catched_data = getHeader( EXPFORMATFILE, qline, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != FILE_NARGUMENTS::N_HEADER2 ) )
                return 0;

            file_format = data[ 1 ].c_str();
            state = FILE_STATES::DATASET;

        }
        break;

        case FILE_STATES::DATASET:
        {
            bool catched_data = getHeader( EXPDATASET, qline, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != FILE_NARGUMENTS::N_DATASET ) )
                return 0;

            dataset_type = data[ 1 ].c_str();
            state = readDataSetValues();


        }
        break;

        case FILE_STATES::POINTSINF:
        {
            bool catched_data = getHeader( EXPDATATYPE, qline, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != FILE_NARGUMENTS::N_POINTSINF ) )
                return 0;

            format_points = data[ 1 ].c_str();
            number_of_points = std::atoi( data[ 2 ].c_str() );

            state = FILE_STATES::POINTS_VALUES;


        }
        break;

        case FILE_STATES::POINTS_VALUES:
        {


            int id = 0;
            while(  id < number_of_points && !file.eof() )
            {


                data.clear();
                getValues( EXPKVALUES, qline, data );
                int ndata = (int) data.size();

                if( ndata != FILE_NARGUMENTS::N_POINTSVALUES )
                    return 0;

                float x = std::atof( data[ 0 ].c_str() );
                vector_points.push_back( x );

                float y = std::atof( data[ 1 ].c_str() );
                vector_points.push_back( y );

                float z = std::atof( data[ 2 ].c_str() );
                vector_points.push_back( z );

                std::string stdline = qline.toStdString();
                std::getline( file, stdline );
                qline = QString( stdline.c_str() );
                id++;
            }



            if( id == number_of_points )
                state = FILE_STATES::CELLSINF;



        }
        break;

        case FILE_STATES::CELLSINF:
        {
            bool catched_data = getHeader( EXPCELLINF, qline, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != FILE_NARGUMENTS::N_CELLINF ) )
                return 0;

            number_of_cells = std::atoi( data[ 1 ].c_str() );
            list_cells_size = std::atoi( data[ 2 ].c_str() );

            state = FILE_STATES::CELLS_VALUES;
             count_cells = 0;

        }
        break;

        case FILE_STATES::CELLS_VALUES:
        {



            int id = 0;
            while(  id < number_of_cells && !file.eof() )
            {

                data.clear();
                getValues( EXPKVALUES, qline, data );
                int ndata = (int) data.size();

                if( ndata == 0 )
                    return 0;

                int nvertices = std::atoi( data[ 0 ].c_str() );
                if( nvertices != ( ndata - 1 ) )
                    return 0;

                int idc = (int)vector_cells.size();

                Cell cell;
                cell.id = idc;

                for( int i = 1; i < ndata; ++i  )
                    cell.vertices.push_back( std::atoi( data[ i ].c_str() ) );

                vector_cells.push_back( cell );

                std::string stdline = qline.toStdString();
                std::getline( file, stdline );
                qline = QString( stdline.c_str() );
                id++;
            }


            if( id == number_of_cells )
                state = FILE_STATES::CELLSTYPES;


        }
        break;

        case FILE_STATES::CELLSTYPES:
        {
            bool catched_data = getHeader( EXPCELLTYPES, qline, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != FILE_NARGUMENTS::N_CELLTYPES ) )
                return 0;

            if( number_of_cells != std::atoi( data[ 1 ].c_str() ) )
                return 0;

            state = FILE_STATES::CELLSTYPES_VALUES;
            count_cells = 0;

        }
        break;

        case FILE_STATES::CELLSTYPES_VALUES:
        {




        int id = 0;
        while(  id < number_of_cells && !file.eof() )
        {

            bool catched_data = getValues( EXPKVALUES, qline, data );

            if ( catched_data == false )
                return 0;

            vector_cells[ id ].type = std::atoi( data[ 0 ].c_str() );


            std::getline( file, line );
            qline = QString( line.c_str() );
            id++;
        }


             if( id == number_of_cells ){
                state = FILE_STATES::POINTS_PROPERTIES;
                return 2;
            }
            else
                return 0;

        }
            break;

        case FILE_STATES::POINTS_PROPERTIES:
        {
            bool catched_data = getHeader( EXPATTRIBUTEDATA, qline, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != FILE_NARGUMENTS::N_POINTS_PROPERTIES ) )
                return 0;

            if( number_of_points != std::atoi( data[ 1 ].c_str() ) )
                return 0;

            state = FILE_STATES::POINTS_PROPERTIES_TYPE;
            count_properties = 0;

        }
        break;

        case FILE_STATES::POINTS_PROPERTIES_TYPE:
        {
            if( visitPoints == 2 && visitCells == 2 )
                return 1;

            bool catched_data = getValues( EXPKVALUES, qline, data );
            int ndata = (int) data.size();

            if( ( catched_data == false ) || ( ndata <= 0 ||
                                               ndata > FILE_NARGUMENTS::N_POINTS_PROPERTIES_TYPE ) )
                return 0;

            if( visitCells == 1 )
                visitCells = 2;

             if( data[ 0 ].compare( "SCALARS" ) == 0 ){
                readScalars( data, "POINTS" );
                state = FILE_STATES::LOOKUPTABLE;
                count_properties++;
            }
            else if( data[ 0 ].compare( "VECTORS" ) == 0 ){
                readVectors( data, "POINTS" );
                state = FILE_STATES::POINTS_PROPERTIES_VALUES;                
                count_properties++;
            }
            else{
                state = FILE_STATES::CELLS_PROPERTIES;                
                return 2;
            }


        }
        break;

        case FILE_STATES::LOOKUPTABLE:
        {
            bool catched_data = getHeader( EXPLOOKUPTABLE, qline, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != FILE_NARGUMENTS::N_LOOKUPTABLE ) )
                return 0;

            lookuptable.push_back( data[ 1 ].c_str() );

            state = FILE_STATES::POINTS_PROPERTIES_VALUES;


        }
            break;

        case FILE_STATES::POINTS_PROPERTIES_VALUES:
        {

            int id = 0;
            while(  id < number_of_points && !file.eof() )
            {

                data.clear();
                bool catched_data = getValues( EXPKVALUES, qline, data );
                int ndata = (int) data.size();

                if ( catched_data == false )
                    return 0;

                for( int j = 0; j < ndata; ++j )
                    vector_point_properties[ count_properties - 1 ].addValue( std::atof( data[ j ].c_str() ) );

                std::getline( file, line );
                qline = QString( line.c_str() );
                id++;
            }


            if( id == number_of_points )
            {
                state = FILE_STATES::POINTS_PROPERTIES_TYPE;
                visitPoints = 1;
                return 2;
            }




        }
        break;

        case FILE_STATES::CELLS_PROPERTIES:
        {
            if( visitPoints == 2 && visitCells == 2 )
                return 1;

            bool catched_data = getHeader( EXPCELLDATAINF, qline, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != FILE_NARGUMENTS::N_CELLS_PROPERTIES ) )
            {
                return 0;

            }
            if( number_of_cells != std::atoi( data[ 1 ].c_str() ) )
                return 0;

            if( visitPoints == 1 )
                visitPoints = 2;

            state = FILE_STATES::CELLS_PROPERTIES_TYPE;
            count_properties = 0;

        }
        break;

        case FILE_STATES::CELLS_PROPERTIES_TYPE:
        {

            if( visitPoints == 2 && visitCells == 2 )
                return 1;


            bool catched_data = getValues( EXPKVALUES, qline, data );
            int ndata = (int) data.size();

            if( ( catched_data == false ) || ( ndata <= 0 || ndata > FILE_NARGUMENTS::N_CELLS_PROPERTIES_TYPE ) )
                return 0;

            if( visitPoints == 1 )
                visitPoints = 2;

            if( data[ 0 ].compare( "SCALARS" ) == 0 ){
                readScalars( data, "CELLS" );
                state = FILE_STATES::LOOKUPTABLECELL;
                count_properties++;
            }
            else if( data[ 0 ].compare( "VECTORS" ) == 0 ){
                readVectors( data, "CELLS" );
                state = FILE_STATES::CELLS_PROPERTIES_VALUES;
                count_properties++;

            }
            else{
                state = FILE_STATES::POINTS_PROPERTIES;
                return 2;
            }

        }
        break;

        case FILE_STATES::LOOKUPTABLECELL:
        {
            bool catched_data = getHeader( EXPLOOKUPTABLE, qline, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != FILE_NARGUMENTS::N_LOOKUPTABLE ) )
                return false;

            lookuptablecell.push_back( data[ 1 ].c_str() );

            state = FILE_STATES::CELLS_PROPERTIES_VALUES;

        }
            break;

        case FILE_STATES::CELLS_PROPERTIES_VALUES:
        {


            int id = 0;
            while(  id < number_of_cells && !file.eof() )
            {

                data.clear();
                bool catched_data = getValues( EXPKVALUES, qline, data );
                int ndata = (int) data.size();

                if ( catched_data == false )
                    return 0;

                for( int j = 0; j < ndata; ++j )
                    vector_cell_properties[ count_properties - 1 ].addValue( std::atof( data[ j ].c_str() ) );

                std::getline( file, line );
                qline = QString( line.c_str() );
                id++;

            }


            if( id == number_of_cells ){
                state = FILE_STATES::CELLS_PROPERTIES_TYPE;
                visitCells = 1;
                return 2;
            }


        }

        break;

        default:
            break;

    };

    return 1;
}


void VTKData::readScalars( vector< std::string > data, std::string type )
{

    FlowProperty property;

    property.setFormat( data[ 0 ].c_str() );
    property.setName( data[ 1 ].c_str() );
    property.setType( type );



    if( data.size() > 3 )
        property.setNumberofComponents( std::atoi( data[ 3 ].c_str() ) );
    else
        property.setNumberofComponents( 1 );

    if( type.compare( "POINTS" ) == 0 )
    {
        int id = (int) vector_point_properties.size();
        property.setId( id );

        ppoint_name_id[ data[ 1 ] ] = id;
        vector_point_properties.push_back( property );
    }
    else if( type.compare( "CELLS" ) == 0 )
    {
        int id = (int) vector_cell_properties.size();
        property.setId( id );

        pcell_name_id[ data[ 1 ] ] = id;
        vector_cell_properties.push_back( property );
    }

}


void VTKData::readVectors( vector< std::string > data, std::string type )
{

    FlowProperty property;

    property.setFormat( data[ 0 ].c_str() );
    property.setName( data[ 1 ].c_str() );
    property.setType( type );
    property.setNumberofComponents( 3 );

    if( type.compare( "POINTS" ) == 0 )
    {
        int id = (int) vector_point_properties.size();
        property.setId( id );

        ppoint_name_id[ data[ 1 ] ] = id;
        vector_point_properties.push_back( property );
    }
    else if( type.compare( "CELLS" ) == 0 )
    {
        int id = (int) vector_cell_properties.size();
        property.setId( id );

        pcell_name_id[ data[ 1 ] ] = id;
        vector_cell_properties.push_back( property );
    }

}


VTKData::FILE_STATES VTKData::readDataSetValues()
{

    if ( dataset_type.compare( "UNSTRUCTURED_GRID" ) != 0 )
        cout << "It still will be implemented!" << endl;
    else
        return FILE_STATES::POINTSINF;

    return FILE_STATES::POINTS_PROPERTIES;

}


void VTKData::addCell( int id, int type, vector< int >& vertices )
{
    Cell cell;
    cell.id = id;
    cell.type = type;
    cell.vertices = vertices;

    vector_cells.push_back( cell );

}


void VTKData::addPointFlowProperty( FlowProperty& property )
{
    int id = vector_point_properties.size();
    vector_point_properties.push_back( property );
    std::string name;
    property.getName( name );
    ppoint_name_id[ name ] = id;

}

void VTKData::addCellFlowProperty( FlowProperty& property )
{
    int id = vector_cell_properties.size();
    vector_cell_properties.push_back( property );
    std::string name;
    property.getName( name );
    pcell_name_id[ name ] = id;

}

void VTKData::getFlowProperty( std::string name, std::string type, FlowProperty& property )
{

    if( type.compare( "POINTS" ) == 0 )
    {
        int id = ppoint_name_id[ name ];
        property = vector_point_properties[ id ];
    }
    else if( type.compare( "CELLS" )  == 0 )
    {
        int id = pcell_name_id[ name ];
        property = vector_cell_properties[ id ];
    }


}


void VTKData::computeMaxMinProperties()
{
    int npointproperties = (int) vector_point_properties.size();
    for( int i = 0; i < npointproperties; ++i )
        computeMaxMinPointProperty( i );

    int ncellproperties = (int)  vector_cell_properties.size();
    for( int i = 0; i < ncellproperties; ++i )
        computeMaxMinCellProperty( i );
}


void VTKData::computeMaxMinPointProperty( int id )
{
    int nproperties = (int) vector_point_properties.size();
    if( id < 0 || id >= nproperties )
        return;


    FlowProperty& p = vector_point_properties[ id ];
    int ncoords = p.getNumberofComponents();

    vector< float > values;
    p.getValues( values );


    if( ncoords == 1 )
    {
        auto itmin = std::min_element( values.begin(), values.end() );
        auto itmax = std::max_element( values.begin(), values.end() );

        int idmin  = std::distance( values.begin(), itmin );
        int idmax = std::distance( values.begin(), itmax );

        p.setMinimum( values[ idmin ] );
        p.setMaximum( values[ idmax ] );

    }

    if( ncoords == 3 )
    {
        int nvalues = (int) values.size()/3;

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

        p.setMinimum( min );
        p.setMaximum( max );

        p.addMinimumCoordinate( minx );
        p.addMaximumCoordinate( maxx );

        p.addMinimumCoordinate( miny );
        p.addMaximumCoordinate( maxy );

        p.addMinimumCoordinate( minz );
        p.addMaximumCoordinate( maxz );

    }


}


void VTKData::computeMaxMinCellProperty( int id )
{
    int nproperties = (int) vector_cell_properties.size();
    if( id < 0 || id >= nproperties )
        return;


    FlowProperty& p = vector_cell_properties[ id ];
    int ncoords = p.getNumberofComponents();

    vector< float > values;
    p.getValues( values );


    if( ncoords == 1 )
    {


        std::vector<float>::iterator itmin = std::min_element( values.begin(), values.end() );
        std::vector<float>::iterator itmax = std::max_element( values.begin(), values.end() );

        int idmin  = std::distance( values.begin(), itmin );
        int idmax = std::distance( values.begin(), itmax );

        p.setMinimum( values[ idmin ] );
        p.setMaximum( values[ idmax ] );

    }

    if( ncoords == 3 )
    {
        int nvalues = (int) values.size()/3;

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

        p.setMinimum( min );
        p.setMaximum( max );

        p.addMinimumCoordinate( minx );
        p.addMaximumCoordinate( maxx );

        p.addMinimumCoordinate( miny );
        p.addMaximumCoordinate( maxy );

        p.addMinimumCoordinate( minz );
        p.addMaximumCoordinate( maxz );

    }


}


void VTKData::getMaxMinMagnitudePointProperty( int id, vector< float >& maxmin )
{
    int nproperties = (int) vector_point_properties.size();
    if( id < 0 || id >= nproperties )
        return;

    FlowProperty& p = vector_point_properties[ id ];
    int ncomponents = p.getNumberofComponents();

    if( ncomponents == 1 )
    {
        float min = p.getMinimum();
        float max = p.getMaximum();

        maxmin.push_back( min );
        maxmin.push_back( max );
    }
    else if( ncomponents == 3 )
    {
        float min = p.getMinimum();
        float max = p.getMaximum();

        maxmin.push_back( min );
        maxmin.push_back( max );
    }

}


void VTKData::getMaxMinCoordinatePointProperty( int id, vector< float >& maxmin )
{
    int nproperties = (int) vector_point_properties.size();
    if( id < 0 || id >= nproperties )
        return;

    FlowProperty& p = vector_point_properties[ id ];
    int ncomponents = p.getNumberofComponents();

    if( ncomponents != 3 )
        return;

    float xmin, xmax;
    float ymin, ymax;
    float zmin, zmax;

    p.getMaxMinCoordinateProperty( xmin, xmax, ymin, ymax, zmin, zmax );

    maxmin.push_back( xmin );
    maxmin.push_back( xmax );

    maxmin.push_back( ymin );
    maxmin.push_back( ymax );

    maxmin.push_back( zmin );
    maxmin.push_back( zmax );

}


void VTKData::getMaxMinMagnitudeCellProperty( int id, vector< float >& maxmin )
{
    int nproperties = (int) vector_cell_properties.size();
    if( id < 0 || id >= nproperties )
        return;

    FlowProperty& p = vector_cell_properties[ id ];
    int ncomponents = p.getNumberofComponents();

    if( ncomponents == 1 )
    {
        float min = p.getMinimum();
        float max = p.getMaximum();

        maxmin.push_back( min );
        maxmin.push_back( max );
    }
    else if( ncomponents == 3 )
    {
        float min = p.getMinimum();
        float max = p.getMaximum();

        maxmin.push_back( min );
        maxmin.push_back( max );
    }

}


void VTKData::getMaxMinCoordinateCellProperty( int id, vector< float >& maxmin )
{
    int nproperties = (int) vector_cell_properties.size();
    if( id < 0 || id >= nproperties )
        return;

    FlowProperty& p = vector_cell_properties[ id ];
    int ncomponents = p.getNumberofComponents();

    if( ncomponents != 3 )
        return;

    float xmin, xmax;
    float ymin, ymax;
    float zmin, zmax;

    p.getMaxMinCoordinateProperty( xmin, xmax, ymin, ymax, zmin, zmax );

    maxmin.push_back( xmin );
    maxmin.push_back( xmax );

    maxmin.push_back( ymin );
    maxmin.push_back( ymax );

    maxmin.push_back( zmin );
    maxmin.push_back( zmax );

}


void VTKData::writeFile( std::string filename ) const
{
    ofstream file;
    file.open( filename.c_str() );
    if( !file.is_open() ) return;

    file <<  "# vtk DataFile Version 2.0" << endl;
    file << "Unstructured Grid" << endl;
	file << "ASCII" << endl;
    file << "DATASET " << "UNSTRUCTURED_GRID" << endl;

	int npoints = vector_points.size() / 3;
	file << "POINTS " << npoints << " double" << endl;

    for( int i = 0; i < npoints; ++i )
        file << vector_points[ 3*i ] << " " << vector_points[ 3*i + 1 ] << " " << vector_points[ 3*i + 2 ] << endl;

    file << endl;

	int size = vector_cells.size() * 5;
	file << "CELLS " << vector_cells.size() << " " << size << endl;

	for (int i = 0; i < vector_cells.size(); ++i)
    {
        Cell cell = vector_cells[ i ];
        int nvertices = (int) cell.vertices.size();

        file << nvertices ;
        for( int j = 0; j < nvertices; j++ )
            file << " " << cell.vertices[ j ];

		file << endl;
    }


    file << endl;
    file << "CELL_TYPES " << vector_cells.size() << endl;

    for( int i = 0; i < vector_cells.size(); ++i )
    {
        Cell cell = vector_cells[ i ];
        file << cell.type << endl;
    }


    if( vector_point_properties.empty() == false )
    {

        int npproperties = (int) vector_point_properties.size();
		int npoints = vector_points.size() / 3;
        file << "POINT_DATA " << npoints << endl;

        for( int i = 0; i < npproperties; ++i )
        {
            FlowProperty p = vector_point_properties[ i ];

            std::string format;
            p.getFormat( format );
            std::string name;
            p.getName( name );

            file << format << " " << name << " double" << endl;

            if( format.compare( "SCALARS" ) == 0 ){
                file << "LOOKUP_TABLE default" << endl;

                vector< float > values;
                p.getValues( values );
				for (int j = 0; j < values.size(); ++j)
                    file << values[ j ] << endl;

            }
            else if( format.compare( "VECTORS" ) == 0 ){

                vector< float > values;
                p.getValues( values ); 
				int npoints = values.size() / 3;
                for( int j = 0; j < npoints; ++j )
                    file << values[ 3*j ] << " " << values[ 3*j + 1 ] << " " << values[ 3*j + 2 ] << endl;

            }
        }

    }

    file << endl;

    if( vector_cell_properties.empty() == false )
    {

        int npproperties = (int) vector_cell_properties.size();
        file << "CELL_DATA " << vector_cells.size() << endl;

        for( int i = 0; i < npproperties; ++i )
        {
            FlowProperty p = vector_cell_properties[ i ];

            std::string format;
            p.getFormat( format );
            std::string name;
            p.getName( name );

            file << format << " " << name << " double" << endl;

            if( format.compare( "SCALARS" ) == 0 ){
                file << "LOOKUP_TABLE default" << endl;

                vector< float > values;
                p.getValues( values );
                for( int j = 0; j < vector_cells.size(); ++j )
                    file << values[ j ] << endl;

            }
            else if( format.compare( "VECTORS" ) == 0 ){

                vector< float > values;
                p.getValues( values );
				int npoints = values.size() / 3;
				for (int j = 0; j < npoints; ++j)
                    file << values[ 3*j ] << " " << values[ 3*j + 1 ] << " " << values[ 3*j + 2 ] << endl;

            }
        }

    }

}

void VTKData::clear()
{
    file_version.clear();
    file_comments.clear();
    file_format.clear();
    dataset_type.clear();

    format_points.clear();
    number_of_points = 0;
    vector_points.clear();

    number_of_cells = 0;
    list_cells_size = 0;
    vector_cells.clear();
    count_cells = 0;

    vector_point_properties.clear();
    vector_cell_properties.clear();
    ppoint_name_id.clear();
    pcell_name_id.clear();
    count_properties;

    lookuptable.clear();
    lookuptablecell.clear();

    visitCells = 0;
    visitPoints = 0;
    is_empty = true;
}
