#include "VTKData.h"

VTKData::VTKData()
{
    properties_id = 0;
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


    bool error = false;
    UNSTRUCTUREDGRID_STATES state =  UNSTRUCTUREDGRID_STATES::HEADER0;


    std::string line;
    std::getline( file, line );

    while ( !file.eof() ) {

        QString qline( line.c_str() );
        bool local_error = !parseVTKData( state, qline );
        error = ( error || local_error );

        std::getline( file, line );

    }


    is_empty = false;

    if( error == true )
    {
        std::cout << "-- Error: parsing fail." << std::endl << std::endl;
        return false;
    }



    return true;

}


void VTKData::setupData()
{
    int npointsattributes = attribute_name.size();
    for( int i = 0; i < npointsattributes; ++i )
    {
        vector< float > data;
        int ncomp = 0;
        getAttribute( attribute_name[ i ], "POINTS", ncomp, data );


        if( ncomp == 1 )
        {
            auto itmin = std::min_element( data.begin(), data.end() );
            auto itmax = std::max_element( data.begin(), data.end() );

            int idmin  = std::distance( data.begin(), itmin );
            int idmax = std::distance( data.begin(), itmax );

            std::pair<float, float> minmax;
            minmax.first = data[ idmin ];
            minmax.second = data[ idmax ];

            point_properties_maxmin.push_back( minmax );
        }


        else if( ncomp == 3 )
        {

            if( data.empty() == true ) return;

            int nelemens = (int) data.size()/3;

            float min = data[ 0 ]*data[ 0 ] + data[ 1 ]*data[ 1 ] + data[ 2 ]*data[ 2 ];
            float max = min;

            float minx = data[ 0 ], miny = data[ 1 ], minz = data[ 2 ];
            float maxx = minx, maxy = miny, maxz = minz;


            for( int j = 3; j < nelemens; ++j )
            {
                float norm =  data[ 3*j ]*data[ 3*j ] + data[ 3*j + 1 ]*data[ 3*j + 1 ] + data[ 3*j + 2 ]*data[ 3*j + 2 ];

                if( norm <= min ) min = norm;
                if( norm >= max ) max = norm;

                if( data[ 3*j ] <= minx ) minx = data[ 3*j ];
                if( data[ 3*j ] >= maxx ) maxx = data[ 3*j ];

                if( data[ 3*j + 1 ] <= miny ) miny = data[ 3*j + 1];
                if( data[ 3*j + 1 ] >= maxy ) maxy = data[ 3*j + 1 ];

                if( data[ 3*j + 2 ] <= minz ) minz = data[ 3*j + 2 ];
                if( data[ 3*j + 2 ] >= maxz ) maxz = data[ 3*j + 2 ];
            }

            std::pair<float, float> minmax;
            minmax.first = min;
            minmax.second = max;
            point_properties_maxmin.push_back( minmax );

            std::pair<float, float> minmax_X;
            minmax_X.first = minx;
            minmax_X.second = maxx;

            point_properties_maxmin_X.push_back( minmax_X );

            std::pair<float, float> minmax_Y;
            minmax_Y.first = miny;
            minmax_Y.second = maxy;

            point_properties_maxmin_Y.push_back( minmax_Y );


            std::pair<float, float> minmax_Z;
            minmax_Z.first = minz;
            minmax_Z.second = maxz;

            point_properties_maxmin_Z.push_back( minmax_Z );


        }

    }


    int ncellsattributes = attributecell_name.size();
    for( int i = 0; i < ncellsattributes; ++i )
    {
        vector< float > data;
        int ncomp = 0;
        getAttribute( attributecell_name[ i ], "CELLS", ncomp, data );


        if( ncomp == 1 )
        {
            auto itmin = std::min_element( data.begin(), data.end() );
            auto itmax = std::max_element( data.begin(), data.end() );

            int idmin  = std::distance( data.begin(), itmin );
            int idmax = std::distance( data.begin(), itmax );

            std::pair<float, float> minmax;
            minmax.first = data[ idmin ];
            minmax.second = data[ idmax ];

            cells_properties_maxmin.push_back( minmax );
        }


        else if( ncomp == 3 )
        {
            if( data.empty() == true ) return;

            int nelemens = (int) data.size()/3;

            float min = data[ 0 ]*data[ 0 ] + data[ 1 ]*data[ 1 ] + data[ 2 ]*data[ 2 ];
            float max = min;

            float minx = data[ 0 ], miny = data[ 1 ], minz = data[ 2 ];
            float maxx = minx, maxy = miny, maxz = minz;


            for( int j = 3; j < nelemens; ++j )
            {
                float norm =  data[ 3*j ]*data[ 3*j ] + data[ 3*j + 1 ]*data[ 3*j + 1 ] + data[ 3*j + 2 ]*data[ 3*j + 2 ];

                if( norm <= min ) min = norm;
                if( norm >= max ) max = norm;

                if( data[ 3*j ] <= minx ) minx = data[ 3*j ];
                if( data[ 3*j ] >= maxx ) maxx = data[ 3*j ];

                if( data[ 3*j + 1 ] <= miny ) miny = data[ 3*j + 1];
                if( data[ 3*j + 1 ] >= maxy ) maxy = data[ 3*j + 1 ];

                if( data[ 3*j + 2 ] <= minz ) minz = data[ 3*j + 2 ];
                if( data[ 3*j + 2 ] >= maxz ) maxz = data[ 3*j + 2 ];
            }

            std::pair<float, float> minmax;
            minmax.first = min;
            minmax.second = max;
            cells_properties_maxmin.push_back( minmax );

            std::pair<float, float> minmax_X;
            minmax_X.first = minx;
            minmax_X.second = maxx;

            cells_properties_maxmin_X.push_back( minmax_X );

            std::pair<float, float> minmax_Y;
            minmax_Y.first = miny;
            minmax_Y.second = maxy;

            cells_properties_maxmin_Y.push_back( minmax_Y );


            std::pair<float, float> minmax_Z;
            minmax_Z.first = minz;
            minmax_Z.second = maxz;

            cells_properties_maxmin_Z.push_back( minmax_Z );

        }

    }


//    int ncellsvalues = cells_values.size();
    
//    for( int id = 0; id < ncellsvalues;  )
//    {
//        int nvertices = cells_values[ id ];
//        Cell *cell = new Cell();
//        cell->id = id++;


//        for( int j = 0; j < nvertices; ++j )
//        {
//            cell->vertices.push_back( cells_values[ id ] );
//            id++;
//        }

//        vector_cells.push_back( cell );
//    }

}


VTKData::UNSTRUCTUREDGRID_STATES VTKData::nextState( std::string type )
{
     if( type.compare( "STRUCTURED_POINTS" ) == 0 )
     {

     }

     else if( type.compare( "STRUCTURED_GRID" ) == 0 )
     {

     }

     else if ( type.compare( "UNSTRUCTURED_GRID" ) == 0 )
     {
         return UNSTRUCTUREDGRID_STATES::DATATYPE;
     }

     else if ( type.compare( "POLYDATA") == 0 )
     {

     }

     else if ( type.compare( "RECTILINEAR_GRID" ) == 0 )
     {

     }

     else if ( type.compare( "FIELD" ) == 0 )
     {

     }

     else return UNSTRUCTUREDGRID_STATES::ATTRIBUTEDATA;

}


bool VTKData::parseVTKData( UNSTRUCTUREDGRID_STATES& state, QString line )
{

    const QRegExp EXPHEADER0( "(?:\\s*)(?:vtk)(?:\\s*)(?:DataFile)(?:\\s*)(?:Version)(?:\\s*)(\\d*\\.\\d*)" );
    const QRegExp EXPDATASET( "(?:\\s*)(?:DATASET)(?:\\s*)\\b(STRUCTURED_POINTS|STRUCTURED_GRID|UNSTRUCTURED_GRID|POLYDATA|RECTILINEAR_GRID|FIELD)\\b" );
    const QRegExp EXPFORMATFILE( "\\b(ASCII|BINARY)\\b" );
//    const QRegExp EXPDATATYPE( "\\b(POINTS|VERTICES|LINES|POLYGONS|TRIANGLE_STRIPS)\\b(?:\\s*)(\\d+)(?:\\s*)\\b(bit|unsigned_char|char|unsigned_short|short|unsigned_int|int|unsigned_long|long|float|double)\\b" );
    const QRegExp EXPDATATYPE( "\\b(POINTS|VERTICES|LINES|POLYGONS|TRIANGLE_STRIPS)\\b(?:\\s*)(\\d+)" );
    const QRegExp EXPCELLINF( "(?:\\s*)(?:CELLS)(?:\\s*)(\\d+)(?:\\s*)(\\d+)" );
    const QRegExp EXPCELLTYPES( "(?:\\s*)(?:CELL_TYPES)(?:\\s*)(\\d+)(?:\\s*)" );
    const QRegExp EXPATTRIBUTEDATA( "(?:\\s*)(?:POINT_DATA)(?:\\s*)(\\d+)(?:\\s*)" );
    const QRegExp EXPLOOKUPTABLE( "(?:\\s*)(?:LOOKUP_TABLE)(?:\\s*)(\.+)(?:\\s*)" );
    const QRegExp EXPCELLDATAINF( "(?:\\s*)(?:CELL_DATA)(?:\\s*)(\\d+)(?:\\s*)" );
    const QRegExp EXPKVALUES( "\\s+" );

    vector< std::string > data;

    switch( state )
    {
        case UNSTRUCTUREDGRID_STATES::HEADER0:
        {
            bool catched_data = getHeader( EXPHEADER0, line, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NHEADER0 ) )
                return false;

            file_version = data[ 1 ].c_str();
            state = UNSTRUCTUREDGRID_STATES::HEADER1;

        }
        break;

        case UNSTRUCTUREDGRID_STATES::HEADER1:
        {
            file_comments = line.toStdString();
            state = UNSTRUCTUREDGRID_STATES::HEADER2;

        }
        break;

        case UNSTRUCTUREDGRID_STATES::HEADER2:
        {

            bool catched_data = getHeader( EXPFORMATFILE, line, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NHEADER2 ) )
                return false;

            file_format = data[ 1 ].c_str();
            state = UNSTRUCTUREDGRID_STATES::DATASET;

        }
        break;

        case UNSTRUCTUREDGRID_STATES::DATASET:
        {
            bool catched_data = getHeader( EXPDATASET, line, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NDATASET ) )
                return false;

            dataset_type = data[ 1 ].c_str();
            state = nextState( dataset_type );

        }
        break;

        case UNSTRUCTUREDGRID_STATES::DATATYPE:
        {
            bool catched_data = getHeader( EXPDATATYPE, line, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NDATATYPE ) )
                return false;

            data_type = data[ 1 ].c_str();
            data_nvalues = data[ 2 ].c_str();

            state = UNSTRUCTUREDGRID_STATES::DATAVALUES;
            total_datavalues = 0;

        }
        break;

        case UNSTRUCTUREDGRID_STATES::DATAVALUES:
        {
            bool catched_data = getValues( EXPKVALUES, line, data );
            int ndata = (int) data.size();

            if ( catched_data == false )
                return false;


            for( int j = 0; j < ndata; ++j )
            {
                float value = std::atof( data.at( j ).c_str() );
                data_values.push_back( value );
            }
            total_datavalues++;

            if( total_datavalues == std::atoi( data_nvalues.c_str() ) )
                state = UNSTRUCTUREDGRID_STATES::CELLINF;
            else
                return false;

        }
            break;

        case UNSTRUCTUREDGRID_STATES::CELLINF:
        {
            bool catched_data = getHeader( EXPCELLINF, line, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NCELLINF ) )
                return false;

            cells_number = data[ 1 ].c_str();
            listcells_size = data[ 2 ].c_str();

            state = UNSTRUCTUREDGRID_STATES::CELLINFVALUES;
            total_datavalues = 0;

        }
        break;

        case UNSTRUCTUREDGRID_STATES::CELLINFVALUES:
        {
            bool catched_data = getValues( EXPKVALUES, line, data );
            int ndata = (int) data.size();

            if ( catched_data == false )
                return false;


            if( std::atoi( data[ 0 ].c_str() ) != ( ndata - 1 ) )
                    return false;


//            for( int j = 0; j < ndata; ++j )
//            {
//                float value = std::atoi( data.at( j ).c_str() );
//                cells_values.push_back( value );
//            }

            for( int j = 1; j < ndata; ++j )
            {
                float value = std::atoi( data.at( j ).c_str() );
                cells_values.push_back( value );
            }
            total_datavalues++;

            if( total_datavalues == std::atoi( cells_number.c_str() ) )
                state = UNSTRUCTUREDGRID_STATES::CELLTYPES;
            else
                return false;

        }
            break;

        case UNSTRUCTUREDGRID_STATES::CELLTYPES:
        {
            bool catched_data = getHeader( EXPCELLTYPES, line, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NCELLTYPES ) )
                return false;

            cell_types = data[ 1 ].c_str();

            state = UNSTRUCTUREDGRID_STATES::CELLTYPEVALUES;
            total_datavalues = 0;

        }
        break;

        case UNSTRUCTUREDGRID_STATES::CELLTYPEVALUES:
        {
            bool catched_data = getValues( EXPKVALUES, line, data );
            int ndata = (int) data.size();

            if ( catched_data == false )
                return false;

            for( int j = 0; j < ndata; ++j )
            {
                float value = std::atoi( data.at( j ).c_str() );
                celltype_values.push_back( value );
            }
            total_datavalues++;

            if( total_datavalues == std::atoi( cell_types.c_str() ) )
                state = UNSTRUCTUREDGRID_STATES::ATTRIBUTEDATA;
            else
                return false;

        }
            break;

        case UNSTRUCTUREDGRID_STATES::ATTRIBUTEDATA:
        {
            bool catched_data = getHeader( EXPATTRIBUTEDATA, line, data );
            int ndata = (int) data.size() - 1 ;

            if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NATTRIBUTEDATA ) )
                return false;

            attributes_number = data[ 1 ].c_str();

            state = UNSTRUCTUREDGRID_STATES::ATTRIBUTETYPE;
            total_datavalues = 0;

        }
        break;

        case UNSTRUCTUREDGRID_STATES::ATTRIBUTETYPE:
        {
            bool catched_data = getValues( EXPKVALUES, line, data );
            int ndata = (int) data.size();

            if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NATTRIBUTETYPE ) )
                return false;


            point_properties_index[ data[ 1 ].c_str() ] = attribute_name.size();

            attribute_type.push_back( data[ 0 ].c_str() );
            attribute_name.push_back( data[ 1 ].c_str() );
            attribute_format.push_back( data[ 2 ].c_str() );


            properties_type[ properties_id ] = PROPERTYTYPE::POINTS;
            properties_names[ properties_id ] = data[ 1 ].c_str();
            properties_formats[ properties_id ] = data[ 2 ].c_str();


            if( attribute_type.back().compare( "SCALARS" ) == 0 )
            {
                state = UNSTRUCTUREDGRID_STATES::LOOKUPTABLE;

            }else
            {
                state = UNSTRUCTUREDGRID_STATES::ATTRIBUTEVALUES;
            }

            total_datavalues = 0;
            properties_id++;

        }
        break;

    case UNSTRUCTUREDGRID_STATES::LOOKUPTABLE:
    {
        bool catched_data = getHeader( EXPLOOKUPTABLE, line, data );
        int ndata = (int) data.size() - 1 ;

        if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NLOOKUPTABLE ) )
            return false;

        lookuptable.push_back( data[ 1 ].c_str() );

        state = UNSTRUCTUREDGRID_STATES::ATTRIBUTEVALUES;

    }
    break;

    case UNSTRUCTUREDGRID_STATES::ATTRIBUTEVALUES:
    {
        bool catched_data = getValues( EXPKVALUES, line, data );
        int ndata = (int) data.size();

        if ( catched_data == false )
            return false;

        for( int j = 0; j < ndata; ++j )
        {
            float value = std::atof( data.at( j ).c_str() );
            attribute_values.push_back( value );
        }

        total_datavalues++;

        if( attribute_type.back().compare( "SCALARS" ) == 0 && total_datavalues == std::atoi( attributes_number.c_str() ) )
            state = UNSTRUCTUREDGRID_STATES::ATTRIBUTETYPE;

        else if( attribute_type.back().compare( "VECTORS" ) == 0 && ( total_datavalues == std::atoi( attributes_number.c_str() ) ) )
            state = UNSTRUCTUREDGRID_STATES::CELLDATAINF;

        else
            return false;


    }
        break;

    case UNSTRUCTUREDGRID_STATES::CELLDATAINF:
    {
        bool catched_data = getHeader( EXPCELLDATAINF, line, data );
        int ndata = (int) data.size() - 1 ;

        if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NCELLDATAINF ) )
            return false;

        cells_number_data = data[ 1 ].c_str();

        state = UNSTRUCTUREDGRID_STATES::ATTRIBUTECELL;
        total_datavalues = 0;

    }
    break;

    case UNSTRUCTUREDGRID_STATES::ATTRIBUTECELL:
    {
        bool catched_data = getValues( EXPKVALUES, line, data );
        int ndata = (int) data.size();

        if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NATTRIBUTETYPE ) )
            return false;

        cells_properties_index[ data[ 1 ].c_str() ] = attributecell_name.size();

        attributecell_type.push_back( data[ 0 ].c_str() );
        attributecell_name.push_back( data[ 1 ].c_str() );
        attributecell_format.push_back( data[ 2 ].c_str() );


        properties_type[ properties_id ] = PROPERTYTYPE::CELLS;
        properties_names[ properties_id ] = data[ 1 ].c_str();
        properties_formats[ properties_id ] = data[ 2 ].c_str();

        if( attributecell_type.back().compare( "SCALARS" ) == 0 )
            state = UNSTRUCTUREDGRID_STATES::LOOKUPTABLECELL;
        else
            state = UNSTRUCTUREDGRID_STATES::ATTRIBUTECELVALUES;

        total_datavalues = 0;
        properties_id++;

    }
    break;

    case UNSTRUCTUREDGRID_STATES::LOOKUPTABLECELL:
    {
        bool catched_data = getHeader( EXPLOOKUPTABLE, line, data );
        int ndata = (int) data.size() - 1 ;

        if( ( catched_data == false ) || ( ndata != UNSTRUCTUREDGRID_NARGUMENTS::NLOOKUPTABLE ) )
            return false;

        lookuptablecell.push_back( data[ 1 ].c_str() );

        state = UNSTRUCTUREDGRID_STATES::ATTRIBUTECELVALUES;
        total_datavalues = 0;

    }
    break;

    case UNSTRUCTUREDGRID_STATES::ATTRIBUTECELVALUES:
    {
        bool catched_data = getValues( EXPKVALUES, line, data );
        int ndata = (int) data.size();

        if ( catched_data == false )
            return false;

        for( int j = 0; j < ndata; ++j )
        {
            float value = std::atof( data.at( j ).c_str() );
            attributecell_values.push_back( value );
        }

        total_datavalues++;

        if( attributecell_type.back().compare( "SCALARS" ) == 0 && total_datavalues == std::atoi( cells_number_data.c_str() ) )
            return true;

        else if( attributecell_type.back().compare( "VECTORS" ) == 0 && ( total_datavalues == std::atoi( cells_number_data.c_str() ) ) )
            state = UNSTRUCTUREDGRID_STATES::ATTRIBUTECELL;

        else
            return false;


    }
        break;
        default:
            break;
    };

    return true;
}


void VTKData::getPoints( vector< float >& points ) const
{
    points = data_values;
}


void VTKData::getCells( vector< int >& shape, vector< int >& index ) const
{
    shape = celltype_values;
    index = cells_values;
}


void VTKData::getAttributesPoints( vector< std::string >& format, vector< std::string >& name, vector< float >& values ) const
{

    if( attribute_name.empty() == true ) return;

    format = attribute_type;
    name = attribute_name;
    values = attribute_values;

}


void VTKData::getAttributesCells( vector< std::string >& format, vector< std::string >& name, vector< float >& values ) const
{
    format = attributecell_type;
    name = attributecell_name;
    values = attributecell_values;

}


void VTKData::getAttribute( std::string name, std::string type, int& ncomp, vector< float >& values )
{

    if( type == "POINTS" )
    {
        int idlocal = point_properties_index[ name ];
        int nattributes = std::atoi( attributes_number.c_str() );

        int id = 0;
        for( int i = 0; i < idlocal; ++i )
        {
            if( attribute_type[ i ] == "SCALARS" )
                id += nattributes;
            else if( attribute_type[ i ] == "VECTORS" )
                id += 3*nattributes;
        }

        if( attribute_type[ idlocal ] == "SCALARS" )
        {
            ncomp = 1;
            int total = id + nattributes;
            for( int i = id; i < total; ++i )
                values.push_back( attribute_values[ i ] );
        }
        else if( attribute_type[ idlocal ] == "VECTORS" )
        {
            ncomp = 3;
            nattributes *= 3;
            int total = id + nattributes;
            for( int i = id; i < total; ++i )
                values.push_back( attribute_values[ i ] );
        }


    }

    if( type == "CELLS" )
    {
        int idlocal = cells_properties_index[ name ];
        int nattributes = std::atoi( cells_number_data.c_str() );

        int id = 0;
        for( int i = 0; i < idlocal; ++i )
        {
            if( attributecell_type[ i ] == "SCALARS" )
                id += nattributes;
            else if( attributecell_type[ i ] == "VECTORS" )
                id += 3*nattributes;
        }

        if( attributecell_type[ idlocal ] == "SCALARS" )
        {
            ncomp = 1;
            int total = id + nattributes;
            for( int i = id; i < total; ++i )
                values.push_back( attributecell_values[ i ] );
        }
        else if( attributecell_type[ idlocal ] == "VECTORS" )
        {
            ncomp = 3;
            nattributes *= 3;
            int total = id + nattributes;
            for( int i = id; i < total; ++i )
                values.push_back( attributecell_values[ i ] );
        }


    }


}


void VTKData::getMaxMinAttribute( std::string name, std::string type, int& ncomp, vector< float >& maxmin )
{
    if( type == "POINTS" )
    {
        int idlocal = point_properties_index[ name ];


        maxmin.push_back( point_properties_maxmin[ idlocal ].first );
        maxmin.push_back( point_properties_maxmin[ idlocal ].second );

    }

    if( type == "CELLS" )
    {
        int idlocal = cells_properties_index[ name ];
        maxmin.push_back( cells_properties_maxmin[ idlocal ].first );
        maxmin.push_back( cells_properties_maxmin[ idlocal ].second );

    }


}

void VTKData::getMaxMinCoordinates( std::string name, std::string type, vector< float >& values )
{

    if( type == "POINTS" )
    {
        int idlocal = point_properties_index[ name ];
        if( attribute_type[ idlocal ] != "VECTORS" ) return;

        int id = 0;
        for( int i = 0; i < idlocal; ++i )
        {
            if( attribute_type[ i ] == "VECTORS" )
                id++;
        }

        float xmin = point_properties_maxmin_X[ id ].first;
        float xmax = point_properties_maxmin_X[ id ].second;

        float ymin = point_properties_maxmin_Y[ id ].first;
        float ymax = point_properties_maxmin_Y[ id ].second;

        float zmin = point_properties_maxmin_Z[ id ].first;
        float zmax = point_properties_maxmin_Z[ id ].second;


        values.push_back( xmin );
        values.push_back( xmax );
        values.push_back( ymin );
        values.push_back( ymax );
        values.push_back( zmin );
        values.push_back( zmax );

    }

    if( type == "CELLS" )
    {


        int idlocal = cells_properties_index[ name ];
        if( attributecell_type[ idlocal ] != "VECTORS" ) return;

        int id = 0;
        for( int i = 0; i < idlocal; ++i )
        {
            if( attributecell_type[ i ] == "VECTORS" )
                id++;
        }

        float xmin = cells_properties_maxmin_X[ id ].first;
        float xmax = cells_properties_maxmin_X[ id ].second;

        float ymin = cells_properties_maxmin_Y[ id ].first;
        float ymax = cells_properties_maxmin_Y[ id ].second;

        float zmin = cells_properties_maxmin_Z[ id ].first;
        float zmax = cells_properties_maxmin_Z[ id ].second;


        values.push_back( xmin );
        values.push_back( xmax );
        values.push_back( ymin );
        values.push_back( ymax );
        values.push_back( zmin );
        values.push_back( zmax );
    }

}

int VTKData::getNumberofPoints()
{
    return std::atoi( data_nvalues.c_str() );
}

/*
void VTKData::clear()
{
       attribute_type.clear();
       attribute_name.clear();
       attribute_format.clear();
       lookuptable.clear();
        attributecell_type.clear();
        attributecell_name.clear();
        attributecell_format.clear();
        lookuptablecell.clear();

       data_values.clear();
       cells_values.clear();
       celltype_values.clear();
       attribute_values.clear();
       attributecell_values.clear();

       is_empty = true;

        properties_type.clear();
       properties_names.clear();
       properties_formats.clear();
       point_properties_index.clear();
       cells_properties_index.clear();


         point_properties_maxmin.clear();
         point_properties_maxmin_X.clear();
         point_properties_maxmin_Y.clear();
         point_properties_maxmin_Z.clear();
        cells_properties_maxmin.clear();
         cells_properties_maxmin_X.clear();
         cells_properties_maxmin_Y.clear();
         cells_properties_maxmin_Z.clear();

         total_datavalues = 0;
         properties_id = 0;
}
*/
